
nodeviz – A fast and small Bitcoin node vizualition tool
=============================================================

##### Dependencies:
	curl	
	leveldb
	boost_system
	boost_thread 
	boost_chrono
	cxxopts (header-only included)
	catch test framework (header-only included)
	PicoSHA2 (header-only included)
	
##### Compilation Ubuntu 16.04:
	clone this repository
	clone leveldb into nodeviz/lib 
	make all 

##### usage
	./nodeviz --init
	./nodeviz

	other options: help, repair, refresh, view, collectip, remove, skip
