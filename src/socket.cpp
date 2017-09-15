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
                tv.tv_sec = 5;
                tv.tv_usec = 0;
                FD_ZERO(&myset);
                FD_SET(sockfd, &myset);
                int selectres = select(sockfd + 1, NULL, &myset, NULL, &tv);
                if (selectres < 0 && errno != EINTR) {
                    throw networkError(ip, "ErrorConn");
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
            throw networkError(ip, "ErrorConn");
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

void Socket::send(int buffers[], int len) {
    int i;

    cout << endl << "sending through socket: ";
    for (i = 0; i != len; ++i) {
        if (::send(sockfd, (char *)&buffers[i], sizeof(buffers[i]), 0) >= 0) {
            printf("%d ", i);
        } else {
            throw networkError(ip, "ErrorSend");
        }
    }

    // For the final send to work, the buffer length has to be shorter
    if (::send(sockfd, (char *)&buffers[i], sizeof(buffers[i]) - 2, 0) >= 0) {
        printf("%d\n", i);
    } else {
        throw networkError(ip, "ErrorSend");
    }
}

void Socket::recv(DB *ipdb) {
    start = time(0);
    auto bufPtr = std::make_unique<char[]>(10000);

    // Read socket for 60 seconds, 10 iterations at a time
    while (time(0) - start < 60) {
        for (int j = 0; j < 1000; j++) {
            if (::recv(sockfd, bufPtr.get(), sizeof bufPtr, 0) > 0) {
                string bufferAsStr(bufPtr.get());
                hexAsStr += bufferAsStr;
            };
        }

        // Extract and save ip addresses from read material
        if (!hexAsStr.empty()) parseHexBuffer(hexAsStr, ipdb, ip);
        hexAsStr.clear();
    }

    closeSocket(ipdb);
}

void Socket::closeSocket(DB *ipdb) {
    // Save in database that reading is finished
    cout << "________________________________Finished_reading: " << ip << endl;
    string input = ipdb->get(ip);
    IpData ipd(ip, input);
    ipd.setStatus("finished");
    ipdb->put(ipd);
    close(sockfd);
}
