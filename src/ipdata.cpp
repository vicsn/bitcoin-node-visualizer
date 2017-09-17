#include "ipdata.hpp"

// Note, there is a bit of redundancy here: ip is incluced in the serial, while
// it is also used to retrieve the serial.
string IpData::serializeAsStr() {
    // add padding so the serial has a predetermined length
    string f1(39 - ip.size(), '*');
    string f2(9 - lon.size(), '0');
    string f3(8 - lat.size(), '0');
    string f4(10 - status.size(), '*');
    string f5(32 - version.size(), '*');

    return ip + f1 + lon + f2 + lat + f3 + status + f4 + version + f5;
}

void IpData::deserializeAsStr(string inBuff) {
    if (inBuff.size() < 98) {
        inBuff.append(98 - inBuff.size(), ' ');
    }
    ip = inBuff.substr(0, 39);
    lon = inBuff.substr(39, 9);
    lat = inBuff.substr(48, 8);
    status = inBuff.substr(56, 10);
    version = inBuff.substr(66, 32);

    // Remove padding
    conversions::removeChars(&ip, "*");
    conversions::removeChars(&status, "*");
    conversions::removeChars(&version, "*");
}
