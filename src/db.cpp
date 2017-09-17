#include "db.hpp"

// Iterate through all existing ip addresses and delete them. Then re-init
void DB::init() {
    leveldb::Iterator *it = ipdb->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        Status s = deleteKey(it->key().ToString());
        cout << "d ";  // output to signal delete
    }

    ifstream input("lib/init.txt");
    string ip;
    while (input >> ip) {
        string input = get(ip);
        IpData ipd(ip, input);
        put(ipd);
        cout << "i ";  // output to signal new init
    }
    cout << endl;
    input.close();
}

// Refresh all ip addresses
void DB::refresh() {
    leveldb::Iterator *it = ipdb->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        IpData ipd(it->value());  // Initialize ipd from slice
        ipd.setStatus("fresh");
        put(ipd);
        cout << "r ";  // output to signal refresh
    }
    cout << endl;
}

// Put fresh ip addresses in a vector
vector<IpData> DB::readToVec() {
    vector<IpData> vec;
    leveldb::Iterator *it = ipdb->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        IpData ipd(it->value());  // Initialize ipd from slice
        if (skip > 0) {
            --skip;
            cout << "skipped" << endl;
        } else {
            if (ipd.getStatus() == "fresh") {
                vec.push_back(ipd);
            }
        }
    }
    return vec;
}

// View status of all ip addresses
void DB::view() {
    int i = 0, j = 0;
    leveldb::Iterator *it = ipdb->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        IpData ipd(it->value());  // Initialize ipd from slice
        if (ipd.getStatus() == "finished") {
            cout << ++i << ":\t" << ipd.toString() << endl;
        }
        ++j;
    }
    cout << "total captured: " << j << endl;
}

// Collect and print the coordinates of all ip addresses
void DB::collect() {
    string lat, lon, target, res;
    leveldb::Iterator *it = ipdb->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        IpData ipd(it->value());
        // If lon yet found and ip is finished
        if (ipd.getLon() == "000000000" && ipd.getStatus() == "finished") {
            // Retrieve info about this ip from website api
            target = "http://freegeoip.net/json/" + ipd.getIp();
            res = simple_curl(target.c_str());

            // unravel JSON longitude and latitude
            if (res.find("fail") == string::npos) {
                lat = res.substr(res.find("latitude"), 20);
                lon = res.substr(res.find("longitude"), 20);
                lat = lat.substr(10, lat.find(",") - 10);
                lon = lon.substr(11, lon.find(",") - 11);
                ipd.setLat(lat);
                ipd.setLon(lon);
                put(ipd);
            }
            cout << "c " << std::flush;  // output to signal new collect
            usleep(250000);  // sleep to prevent too many server requests
        }
    }
    cout << endl;
}

// Find the size of the database by iteratoring over all keys. Note that there
// is no built-in function which provide this
int DB::size() {
    int i = 0;
    leveldb::Iterator *it = ipdb->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        ++i;
    }
    return i;
}
