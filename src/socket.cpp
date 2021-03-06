#include "socket.hpp"

void Socket::setup() {
    // SIGPIPE is generated exactly when the write fails with EPIPE
    // Establishes sane default behavior to stop writing process
    sigignore(SIGPIPE);

    // Set socket to non-blocking
    if ((arg = fcntl(sockfd, F_GETFL, NULL)) < 0) {
        throw networkError(ip, "ErrorGETFL");
    }
    arg |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, arg) < 0) {
        throw networkError(ip, "ErrorSETFL)");
    }

    // Trying to connect with timeout
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        if (errno == EINPROGRESS) {
            do {
                tv.tv_sec = 5;  // timeout after 2 tv_sec seconds
                tv.tv_usec = 0;
                FD_ZERO(&myset);
                FD_SET(sockfd, &myset);
                int selectres = select(sockfd + 1, NULL, &myset, NULL, &tv);
                if (selectres < 0 && errno != EINTR) {
                    throw networkError(ip, "ErrorConn1");
                } else if (selectres > 0) {
                    // Socket selected for sending
                    lon = sizeof(int);
                    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   (void *)(&valopt), &lon) < 0) {
                        throw networkError(ip, "ErrorGetSO");
                    }
                    // Check the value returned...
                    if (valopt) {
                        throw networkError(ip, "ErrorDelay");
                    }
                    break;
                } else {
                    throw networkError(ip, "Timeout");
                }
            } while (1);
        } else {
            throw networkError(ip, "ErrorConn2");
        }
    }
    // Set to blocking mode again...
    if ((arg = fcntl(sockfd, F_GETFL, NULL)) < 0) {
        throw networkError(ip, "ErrorGETFL");
    }
    arg &= (~O_NONBLOCK);
    if (fcntl(sockfd, F_SETFL, arg) < 0) {
        throw networkError(ip, "ErrorSETFL");
    }

    // free the linked-list
    freeaddrinfo(res);
}

// Send message in batches of bytes
void Socket::send(int buffers[], int len, int cut) {
    int i;

    cout << ip << ": sending " << len << " through socket " << endl;
    for (i = 0; i != len; ++i) {
        if (::send(sockfd, (char *)&buffers[i], sizeof(buffers[i]), 0) >= 0) {
            // printf("%d ", i);
        } else {
            throw networkError(ip, "ErrorSend");
        }
    }

    // For the final send to work,the buffer length has to be shorter
    if (::send(sockfd, (char *)&buffers[i], sizeof(buffers[i]) - cut, 0) >= 0) {
        // printf("%d\n", i);
    } else {
        throw networkError(ip, "ErrorSend");
    }
}

string Socket::recvVerack() {
    string version;
    char buf[500];
    if (::recv(sockfd, (char *)&buf, sizeof(buf), 0) > 0) {
        string bufferAsStr(buf, 500);
        if (!bufferAsStr.empty()) {
            bufferAsStr = picosha2::bytes_to_hex_string(begin(bufferAsStr),
                                                        end(bufferAsStr));
            conversions::removeChars(&bufferAsStr, "ffffff");
            if (bufferAsStr.find("76657261") != string::npos) {
                cout << "Verack received" << endl;

                std::smatch m;
                const std::regex r(
                    "(ffff){1}[a-z0-9]{12}0{1}[a-z0-9]{1}"
                    "000000000000000000000000000000000000000000000000{1}");
                std::regex_search(bufferAsStr, m, r);
                size_t pos = bufferAsStr.find(
                    "00000000000000000000000000000000000000000000000"
                    "0");
                if (m.position(0) < bufferAsStr.size()) {
                    version = bufferAsStr.substr(m.position(0) + 86, 32);
                    version = conversions::hexToAscii(version);
                }

                cout << "version: " << version << endl;
            }
        }
    }
    return version;
}

vector<string> Socket::recvIp() {
    vector<string> iplist;
    start = time(0);
    auto bufPtr = std::make_unique<char[]>(10000);

    // Read socket for 60 seconds, 10 iterations at a time
    while (time(0) - start < 45) {
        for (int j = 0; j < 10; j++) {
            if (::recv(sockfd, bufPtr.get(), 10000, 0) > 0) {
                string bufferAsStr(bufPtr.get(), 10000);
                hexAsStr += bufferAsStr;
            };
        }

        // Extract and save ip addresses from read material
        if (!hexAsStr.empty()) {
            iplist = parseHexBuffer(hexAsStr, ip);
            if (iplist.size() > 1) return iplist;
        }
        hexAsStr.clear();
    }
    return iplist;
}

void Socket::closeSocket() { close(sockfd); }
