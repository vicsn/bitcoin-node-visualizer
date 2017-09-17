#ifndef IP_DATA_H
#define IP_DATA_H

#include "../lib/leveldb/include/leveldb/db.h"
#include "conversions.hpp"

class IpData {
   public:
    // IpData(string ip, string lon, string lat, string stat, string version)
    //     : ip(ip), lon(lon), lat(lat), status(stat), version(version) {}
    IpData(leveldb::Slice s) : ip(), lon(), lat(), status(), version() {
        deserializeAsStr(s.ToString());
    }
    IpData(string ip, string input) : ip(ip), status("fresh") {
        if (input.size() > 1) deserializeAsStr(input);
    }

    // Definitions
    string toString() {
        string toStr("ip: ");
        toStr += ip + "  \tlon: ";
        toStr += lon + "\tlat: ";
        toStr += lat + "\tstatus: ";
        toStr += status + "\tversion: ";
        toStr += version;
        return toStr;
    }

    string getIp() { return ip; }
    string getLon() { return lat; }
    string getLat() { return lon; }
    string getStatus() { return status; }
    string getVersion() { return status; }
    size_t getSize() { return (ip + lon + lat + status + version).size(); }
    void setLon(const string &lon) { this->lon = lon; }
    void setLat(const string &lat) { this->lat = lat; }
    void setStatus(const string &status) { this->status = status; }
    void setVersion(const string &version) { this->version = version; }

    // Declarations
    string serializeAsStr();
    void deserializeAsStr(string inBuff);

   private:
    string ip;
    string lon;
    string lat;
    string status;
    string version;
};

#endif
