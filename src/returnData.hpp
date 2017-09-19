struct ReturnData {
    ReturnData(vector<string> iplist, string ip, string version)
        : iplist(iplist), ip(ip), version(version) {}

    vector<string> iplist;
    string ip;
    string version;
};
