#include <boost/chrono/include.hpp>
#include <boost/thread/thread.hpp>
#include <chrono>

#include "cxxopts.hpp"
#include "msg.hpp"
#include "socket.hpp"

// call the receive function
void recvT(Socket &sock, DB *ipdb) { sock.recv(ipdb); }

// Constructing and sending all messages in seperate threads
void makeAndSendMessages(Socket &sock, DB *ipdb) {
    // initialize msg object
    MsgValues msg(sock.ip);

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

    // Read and send over sockets
    boost::thread recvThread(recvT, sock, ipdb);  // TODO std::thread / async ?

    sock.send(buffers1, sizeof(buffers1) / sizeof(buffers1[0]));
    usleep(500000);
    sock.send(buffers2, sizeof(buffers2) / sizeof(buffers2[0]));
    sock.send(buffers3, sizeof(buffers3) / sizeof(buffers3[0]));
    sock.send(buffers4, sizeof(buffers4) / sizeof(buffers4[0]));
    usleep(500000);
    sock.send(buffers4, sizeof(buffers4) / sizeof(buffers4[0]));
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

    // In each iteration, try to get iplists of current iplist
    // TODO update vector and then add to the database in a batch
    while (true) {
        vector<IpData> ipdVec = ipdb.readToVec();
        for (IpData &ipd : ipdVec) {
            usleep(1000000);  // Slow connections to ensure checking
            cout << "___________________initiating ip: " << ipd.getIp() << endl;
            try {
                Socket sock(ipd.getIp().c_str());
                sock.setup();
                makeAndSendMessages(sock, &ipdb);
                // TODO The DB object has to be send as a reference because the
                // socket thread can't return values while it keeps running. In
                // order to prevent delays, a pointer to the database ensures
                // that simultaneous asynchronous receiving can conitinue.
            } catch (networkError &e) {
                ipd.setStatus(e.getMsg());
                ipdb.put(ipd);
                e.printError();
            } catch (std::runtime_error &e) {
                e.what();
            }
        }
    }

    return 0;
}
