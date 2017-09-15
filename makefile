.POSIX:
.SUFFIXES:

CC = clang++
CFLAGS = -std=c++14 -L/usr/lib/x86_64-linux-gnu -lcurl -pthread -lleveldb -lsnappy -lboost_system -lboost_thread -lboost_chrono

all: nodeviz tests #conversionsTest curlFunctionsTest dbTest ipdataTest msgTest networkErrorTest socketTest
install: nodeviz
nodeviz: src/main.cpp src/curlFunctions.cpp src/db.cpp src/ipdata.cpp src/msg.cpp src/socket.cpp src/conversions.cpp
	$(CC) -o nodeviz src/main.cpp src/curlFunctions.cpp src/db.cpp src/ipdata.cpp src/msg.cpp src/socket.cpp src/conversions.cpp $(CFLAGS)
tests: conversionsTest curlFunctionsTest dbTest ipdataTest msgTest networkErrorTest socketTest
conversionsTest: src/tests/conversionsTest.cpp src/conversions.cpp
	$(CC) -o conversionsTest src/tests/conversionsTest.cpp src/conversions.cpp $(CFLAGS)
curlFunctionsTest: src/tests/curlFunctionsTest.cpp src/curlFunctions.cpp
	$(CC) -o curlFunctionsTest src/tests/conversionsTest.cpp src/curlFunctions.cpp $(CFLAGS)
dbTest: src/tests/dbTest.cpp src/curlFunctions.cpp src/ipdata.cpp src/conversions.cpp
	$(CC) -o dbTest src/tests/dbTest.cpp src/curlFunctions.cpp src/ipdata.cpp src/conversions.cpp $(CFLAGS)
ipdataTest: src/tests/ipdataTest.cpp src/conversions.cpp
	$(CC) -o ipdataTest src/tests/ipdataTest.cpp src/conversions.cpp $(CFLAGS)
msgTest: src/tests/msgTest.cpp src/msg.cpp src/curlFunctions.cpp src/ipdata.cpp src/conversions.cpp
	$(CC) -o msgTest src/tests/msgTest.cpp src/msg.cpp src/curlFunctions.cpp src/ipdata.cpp src/conversions.cpp $(CFLAGS)
networkErrorTest: src/tests/networkErrorTest.cpp
	$(CC) -o networkErrorTest src/tests/networkErrorTest.cpp $(CFLAGS)
socketTest: src/tests/socketTest.cpp src/socket.cpp src/msg.cpp src/curlFunctions.cpp src/ipdata.cpp src/conversions.cpp
	$(CC) -o socketTest src/tests/socketTest.cpp src/socket.cpp src/msg.cpp src/curlFunctions.cpp src/ipdata.cpp src/conversions.cpp $(CFLAGS)
clean:
	rm -f nodeviz conversionsTest curlFunctionsTest dbTest ipdataTest msgTest networkErrorTest socketTest
cleantests:
	rm -f conversionsTest curlFunctionsTest dbTest ipdataTest msgTest networkErrorTest socketTest
