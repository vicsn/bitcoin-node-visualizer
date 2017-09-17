#include "conversions.hpp"

// Convert decimal integer to hexstring
string conversions::decToHex(unsigned int dec) {
    stringstream ss;
    ss << hex << dec;
    return reverseHex(ss.str());
}

// Convert from Little-Endian to Big-Endian, or the other way around
string conversions::reverseHex(string rawHex) {
    // Ensure string has an even length
    if (rawHex.size() % 2 != 0) rawHex.insert(begin(rawHex), '0');

    string revHex;
    revHex.reserve(rawHex.size());
    for (unsigned int i = rawHex.size(); i != 0; i -= 2) {
        revHex.append(rawHex, i - 2, 2);
    }
    return revHex;
}

// Convert ip string to hex format
string conversions::ipToHex(string ip) {
    int colcount = std::count(begin(ip), end(ip), ':');
    if (colcount < 1) {                          // Return ipv4
        return decToHex(inet_addr(ip.c_str()));  // TODO deprecated function
    } else {                                     // Return ipv6
        size_t doubleColPos = ip.find("::");
        if (doubleColPos != string::npos) {
            ip.insert(doubleColPos, 32 - (ip.size() - colcount), '0');
        }
        removeChars(&ip, ":");
        return ip;
    }
}

// Convert hexstring to 4 hexnumbers and then to ipv4 format
void conversions::hexToIpv4(string addrAsHexStr, string *ip) {
    ip->clear();
    for (unsigned int i = 0; i < addrAsHexStr.size(); i += 2) {
        string numberAsStr = addrAsHexStr.substr(i, 2);
        *ip += std::to_string(stoi(numberAsStr.c_str(), NULL, 16));
        if (i != 6) ip->push_back('.');
    }
}

// Convert hexstring to ipv6 format
void conversions::hexToIpv6(string addrAsHexStr, string *ip) {
    ip->clear();
    for (unsigned int i = 0; i < addrAsHexStr.size(); i += 4) {
        // if (addrAsHexStr.substr(i, 4) != "0000") // short ipv6 notation
        *ip += addrAsHexStr.substr(i, 4);
        if (i != 28 && (*ip)[ip->size() - 2] != ':') ip->push_back(':');
    }
}

void conversions::removeChars(string *str, string element) {
    size_t pos;
    do {
        pos = str->find(element);
        if (pos != string::npos) str->erase(pos, element.size());
    } while (pos != string::npos);
}

// Convert string to hexnumber to byte
vector<char> conversions::hexToBytes(const string &hex) {
    vector<char> bytes;

    for (unsigned int i = 0; i < hex.size(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = (char)stoi(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

// Double hash a string
string conversions::hash256(string &payload) {
    vector<char> b_payload = conversions::hexToBytes(payload);

    // Takes bytes as an input and as an output
    vector<unsigned char> hash(32);
    picosha2::hash256(begin(b_payload), end(b_payload), begin(hash), end(hash));
    picosha2::hash256(begin(hash), end(hash), begin(hash), end(hash));

    // Takes bytes as an input and hexstring as an output
    string hashed_hexstring =
        picosha2::bytes_to_hex_string(begin(hash), end(hash));
    return hashed_hexstring;
}
