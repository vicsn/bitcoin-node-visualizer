#include "msg.hpp"

// Calculate elements needed to construct payload
string MsgValues::calcPayload() {
    timestamp = conversions::decToHex(time(nullptr));
    timestamp.insert(timestamp.size(), "00000000");

    receivingIp = conversions::ipToHex(receivingIp);
    receivingIp.insert(0, "00000000000000000000ffff");

    nonce = conversions::decToHex(rand() % 7993589098607472366 +
                                  1229782938247303441);

    startHeight = conversions::decToHex(479366);  // fixed height
    startHeight.insert(startHeight.size(), "00");

    return versionIndicator + services + timestamp + services + receivingIp +
           port + services + btccoreSendingIpPort + nonce + nonce +
           btccoreUserAgent + altStartHeight + relay;
}

// Calculate elements needed to construct header
string MsgValues::calcHeader() {
    payloadSize = conversions::decToHex(payload.size() / 2);
    payloadSize.insert(payloadSize.size(), "000000");
    checksum = conversions::hash256(payload).substr(0, 8);
    return magicBytes + versionCommand + payloadSize + checksum;
}

// Constructs the version message
void MsgValues::makeVersionMsg() {
    payload = calcPayload();
    header = calcHeader();
    version = header + payload;
}

// Convert bytes to hexstring, parse as
vector<string> parseHexBuffer(string hexAsStr, string srcAddr) {
    hexAsStr = picosha2::bytes_to_hex_string(begin(hexAsStr), end(hexAsStr));

    // Remove "ffffff" padding of received messages
    conversions::removeChars(&hexAsStr, "ffffff");

    // Create regex pattern to find and retrieve ip addresses
    pair<vector<string>, vector<string>> addrs = getAddrsFromString(hexAsStr);

    vector<string> iplist;
    string ip;

    // Get ipv4 addresses
    for (string addr : addrs.first) {
        conversions::hexToIpv4(addr, &ip);
        iplist.push_back(ip);
        cout << "addr: " << addr << " : " << ip << endl;
    }

    // Get ipv6 addresses
    for (string addr : addrs.second) {
        conversions::hexToIpv6(addr, &ip);
        iplist.push_back(ip);
        cout << "addr: " << addr << " : " << ip << endl;
    }
    return iplist;
}

// Use regular expressions to find ipv4 and ipv6 pattern
pair<vector<string>, vector<string>> getAddrsFromString(string &str) {
    vector<string> ipv4addrs;
    vector<string> ipv6addrs;

    // size_t because pos will be compared to string::npos
    if (str.find("616464720000") != string::npos) {
        cout << "addrs: " << str << endl;
        // Note: only lowercase
        const std::regex r4("ffff{1}[a-z0-9]{8}(208d){1}");
        const std::regex r6(
            "2{1}[a06]{1}[a-z0-9]{1}[0-9]{1}[a-z0-9]{28}(208d){1}");

        // find and hold ipv4 addresses
        for (std::sregex_iterator it(begin(str), end(str), r4), end_it;
             it != end_it; ++it) {
            ipv4addrs.push_back((it->str()).substr(4, 8));
        }

        // find and hold ipv6 addresses
        for (std::sregex_iterator it(begin(str), end(str), r6), end_it;
             it != end_it; ++it) {
            ipv6addrs.push_back((it->str()).substr(0, 32));
        }
    }

    return std::make_pair(ipv4addrs, ipv6addrs);
}

// Prepare string to be written by socket. Using integers as input leads to
// correct conversion to bytes
void stringToIntBuffer(string *msg, vector<int> *vec) {
    string bytestring;
    int left = msg->size() % 8;

    for (string::size_type i = 0; i < (msg->size() - left); i = i + 8) {
        for (string::size_type j = i; j < i + 8; ++j) {
            bytestring = bytestring + (*msg)[j];
        }
        vec->push_back(
            (int)stol(conversions::reverseHex(bytestring), nullptr, 16));
        bytestring.clear();
    }

    if (left) {
        for (string::size_type i = 0; i < left; ++i) {
            bytestring = bytestring + (*msg)[msg->size() - left + i];
        }
        vec->push_back(
            (int)stol(conversions::reverseHex(bytestring), nullptr, 16));
        bytestring.clear();
    }
}
