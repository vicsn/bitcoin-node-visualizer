#ifndef MSG_VALUES_H
#define MSG_VALUES_H

#include <regex>
#include "db.hpp"

using std::regex;

struct MsgValues {
    MsgValues(string ip) : receivingIp(ip){};

    void makeVersionMsg();
    string calcPayload();
    string calcHeader();

    // intermediary set values
    const string versionIndicator = "7f110100";
    const string services = "0100000000000000";
    const string userAgent = "00";
    const string relay = "01";
    const string magicBytes = "f9beb4d9";
    const string versionCommand = "76657273696f6e0000000000";
    const string btccoreSendingIpPort = "000000000000000000000000000000000000";
    const string btccoreUserAgent = "102f5361746f7368693a302e31342e322f";
    const string altStartHeight = "ea4d0700";
    const string port = "208d";
    const string portAsDec = "8333";

    // intermediary values to be calculated
    string receivingIp;
    string timestamp;
    string sendingIp;
    string nonce;
    string startHeight;
    string payloadSize;
    string checksum;
    string payload;
    string header;

    // full messages
    string version;
    string verack = "f9beb4d976657261636b000000000000000000005df6e0e2";
    string ping =
        "f9beb4d970696e6700000000000000000800000098720a6800a46820489fc0"
        "16";
    string getaddr = "f9beb4d9676574616464720000000000000000005df6e0e2";
};

vector<string> parseHexBuffer(string, string);
std::pair<vector<string>, vector<string>> getAddrsFromString(string &);
void stringToIntBuffer(string *msg, vector<int> *vec);

#endif
