
nodeviz – A small Bitcoin node visualition tool
=============================================================

<img src="https://github.com/vicsn/bitcoin-node-visualizer/blob/master/Example.jpg?raw=true" width="550">

[Dependencies](#dependencies)
[Building steps](#building-steps)


##### Dependencies:
	curl
	leveldb
	boost_system
	boost_thread
	boost_chrono
	cxxopts (header-only included)
	catch test framework (header-only included)
	PicoSHA2 (header-only included)

##### Building steps:
	clone this repository
	clone leveldb into nodeviz/lib
	make all

##### get ip addresses
	./nodeviz --init $ARGS
	./nodeviz 		 $ARGS

##### get longitude and latitude
	./nodeviz --collectip $ARGS

##### $ARGS
	--version ipv6, ipv4
	--status finished, ErrorConn1, ErrorConn2, ErrorGetSO, ErrorDelay, Timeout, ErrorSend 
