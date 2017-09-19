#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>  // use for select() construction
#include <fcntl.h>      // use for select() construction
#include <netdb.h>      // use for select() construction
#include <netinet/in.h>
#include <signal.h>  // use for SIGPIPE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "msg.hpp"
#include "networkError.hpp"

class Socket {
   public:
    Socket(const char ip[]) : ip(ip) {
        memset(&hints, 0, sizeof hints);  // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
        status = getaddrinfo(ip, "8333", &hints, &res);
        if (status != 0) throw networkError(ip, "ErrorGetad");
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) throw networkError(ip, "ErrorSock");
    }
    void setup();
    void send(int buffers[], int len, int cut);
    string recvVerack();
    vector<string> recvIp();
    void closeSocket();

    // basic variables to set up socket
    const string ip;
    int sockfd;
    int status;
    struct addrinfo hints;  // optional hints concerning the type of socket
    struct addrinfo *res;   // will point to the results

    // variables related to select()
    long arg;
    fd_set myset;
    struct timeval tv;
    int valopt;
    socklen_t lon;

    // Variables related to writing
    char buffer[10000];  // Note: large buffers lead to padding with 0
    string hexAsStr;
    bool found;
    int start;
};

#endif
