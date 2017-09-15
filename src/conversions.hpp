#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <typeinfo>
#include <vector>
#include "../lib/PicoSHA2/picosha2.h"

using std::string;
using std::cin;
using std::cout;
using std::copy;
using std::endl;
using std::vector;
using std::ofstream;
using std::ostream;
using std::istream;
using std::stringstream;
using std::ifstream;
using std::hex;
using std::getline;
using std::pair;
using std::cerr;
using std::string;
using std::stoi;
using picosha2::hash256_hex_string;

namespace conversions {
void removeChars(string *str, string element);
string decToHex(unsigned int dec);
string reverseHex(string);
string ipToHex(string ip);
void hexToIpv4(string, string *ip);
void hexToIpv6(string, string *ip);
string strToUpper(string);
string hash256(string &payloadString);
vector<char> hexToBytes(const string &hex);

}  // namespace conversions

#endif
