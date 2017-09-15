#ifndef NETWORK_ERROR_H
#define NETWORK_ERROR_H

#include <iostream>
#include <stdexcept>
#include <string>
using std::string;

class networkError : public std::runtime_error {
   public:
    explicit networkError(const string &ip, const string &msg)
        : std::runtime_error(ip), ip(ip), msg(msg) {}
    void printError() { std::cerr << ip << ": " << msg << std::endl; }
    const string getIp() { return ip; }
    const string getMsg() { return msg; }

   private:
    const string ip;
    const string msg;
};

#endif
