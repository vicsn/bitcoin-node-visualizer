#ifndef DB_H
#define DB_H

#include "../lib/leveldb/include/leveldb/db.h"
#include "curlFunctions.hpp"
#include "ipdata.hpp"
#include "networkError.hpp"

using leveldb::Status;

class DB {
   public:
    DB(string path) : skip(), path(path) { options.create_if_missing = true; }
    ~DB() { delete ipdb; }  // TODO is this the correct destructor?

    // Definitions
    Status repair() { return leveldb::RepairDB(path, options); }
    Status open() { return leveldb::DB::Open(options, path, &ipdb); }
    Status deleteKey(string key) {
        return ipdb->Delete(leveldb::WriteOptions(), key);
    }
    void setSkip(int newskip) { skip = newskip; }
    void put(IpData &ipd) {
        Status s = ipdb->Put(leveldb::WriteOptions(), ipd.getIp(),
                             ipd.serializeAsStr());  // TODO take out of class
    }
    void put(string key, string value) {
        Status s = ipdb->Put(leveldb::WriteOptions(), key, value);
    }
    string get(string ip) {
        string input;
        Status s = ipdb->Get(leveldb::ReadOptions(), ip, &input);
        return input;
    }

    // Declarations
    std::vector<IpData> readToVec();
    void init();
    void refresh();
    void view();
    void collect();
    int size();

   private:
    int skip;
    const string path;
    leveldb::Options options;
    leveldb::DB *ipdb;
};

#endif
