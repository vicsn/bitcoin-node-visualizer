#include <boost/chrono/include.hpp>
#include <boost/thread/thread.hpp>
#include <chrono>
#include <future>
#include <thread>

#include "cxxopts.hpp"
#include "msg.hpp"
#include "returnData.hpp"
#include "socket.hpp"

using std::promise;

// Constructing and sending all messages in seperate threads
ReturnData makeAndSendMessages(Socket &sock) {
    // initialize msg object
    MsgValues msg(sock.ip);
    vector<string> iplist;

    // Make version msg
    msg.makeVersionMsg();
    vector<int> ibuffers;
    int buffers1[msg.version.size() / 8];
    stringToIntBuffer(&(msg.version), &ibuffers);
    copy(begin(ibuffers), end(ibuffers), buffers1);

    // Make verack msg
    int buffers2[msg.verack.size() / 8];
    ibuffers.clear();
    stringToIntBuffer(&(msg.verack), &ibuffers);
    copy(begin(ibuffers), end(ibuffers), buffers2);

    // Make ping msg
    int buffers3[msg.ping.size() / 8];
    ibuffers.clear();
    stringToIntBuffer(&(msg.ping), &ibuffers);
    copy(begin(ibuffers), end(ibuffers), buffers3);

    // Make getaddr msg
    int buffers4[msg.getaddr.size() / 8];
    ibuffers.clear();
    stringToIntBuffer(&(msg.getaddr), &ibuffers);
    copy(begin(ibuffers), end(ibuffers), buffers4);

    sock.send(buffers1, sizeof(buffers1) / sizeof(buffers1[0]), 2);
    // If Verack message is received, send other messages
    string version = sock.recvVerack();
    if (!version.empty()) {
        // boost::thread recvThread(recvT, sock, ipdb);
        sock.send(buffers2, (sizeof(buffers2) / sizeof(buffers2[0])) - 1, 0);
        sock.send(buffers3, (sizeof(buffers3) / sizeof(buffers3[0])) - 1, 0);
        sock.send(buffers4, (sizeof(buffers4) / sizeof(buffers4[0])) - 1, 0);
        iplist = sock.recvIp();
    }
    ReturnData data(iplist, sock.ip, version);
    return data;
}

// Optional asynchronous function
void concurrentExecution(IpData ipd, promise<ReturnData> prms) {
    cout << "___________________initiating ip: " << ipd.getIp() << endl;
    try {
        Socket sock(ipd.getIp().c_str());
        sock.setup();                               // open socket
        prms.set_value(makeAndSendMessages(sock));  // get data
        sock.closeSocket();                         // close socket
    } catch (...) {
        prms.set_exception(std::current_exception());
    }
}

int main(int argc, char *argv[]) {
    DB ipdb("nodevizdb");  // TODO omgevingsvariabelen in bestand / enums

    cxxopts::Options options(argv[0], "Bitcoin Node Visualizer");
    options.add_options()("help", "Print help")("repair", "Repair leveldb")(
        "init", "Init leveldb")("refresh", "Refresh leveldb")(
        "view", "View entries")("collectip", "Collect ip addresses")(
        "remove", "Remove entry", cxxopts::value<string>())(
        "skip", "Skip entries and continue search", cxxopts::value<int>());

    options.parse(argc, argv);

    if (options.count("help")) {
        cout << options.help({""}) << endl;
        exit(0);
    } else if (options.count("repair")) {
        Status s = ipdb.repair();
        cout << "repair: " << s.ToString() << endl;
        exit(0);
    } else {
        Status s = ipdb.open();  // Open database
        if (!s.ok()) {
            cout << s.ToString() << endl;
        }
    }

    if (options.count("init")) {
        ipdb.init();
        exit(0);
    } else if (options.count("refresh")) {
        ipdb.refresh();
        exit(0);
    } else if (options.count("view")) {
        ipdb.view();
        exit(0);
    } else if (options.count("collectip")) {
        ipdb.collect();
        exit(0);
    } else if (options.count("remove")) {
        auto &key = options["remove"].as<string>();
        Status s = ipdb.deleteKey(key);
        cout << s.ToString() << endl;
        exit(0);
    } else if (options.count("skip")) {
        auto &skip = options["skip"].as<int>();
        ipdb.setSkip(skip);
    }

    // In each iteration, try to get iplists from all addresses
    while (true) {
        vector<IpData> ipdVec = ipdb.readToVec(10);
        vector<std::future<ReturnData>> ftrvec;
        vector<std::thread> workers;
        vector<ReturnData> sv;

        // Initiate threads
        for (int i = 0; i != ipdVec.size(); ++i) {
            promise<ReturnData> prms;  // make promise
            ftrvec.push_back(std::move(prms.get_future()));
            std::thread th(&concurrentExecution, ipdVec[i], std::move(prms));
            workers.push_back(std::move(th));  // add thread to vector
        }

        // Wait for all threads to be done
        for (auto &th : workers) {
            th.join();
        }

        // check each thread for errors
        for (auto &ftr : ftrvec) {
            try {
                sv.push_back(std::move(ftr.get()));
            } catch (networkError &e) {
                string input = ipdb.get(e.getIp());
                IpData ipd(e.getIp(), input);
                ipd.setStatus(e.getMsg());
                ipdb.put(ipd);
                e.printError();
            } catch (std::runtime_error &e) {
                e.what();
            }
        }

        // For each thread
        for (auto data : sv) {
            // put captured ip addresses into database
            for (string ip : data.iplist) {
                string input = ipdb.get(ip);
                IpData ipd(ip, input);
                ipdb.put(ipd);
            }
            // Save in database that reading is finished
            cout << "_______Finished_reading: " << data.ip << endl;
            string input = ipdb.get(data.ip);
            IpData ipd(data.ip, input);
            ipd.setVersion(data.version);
            ipd.setStatus("finished");
            ipdb.put(ipd);
        }
    }

    return 0;
}
