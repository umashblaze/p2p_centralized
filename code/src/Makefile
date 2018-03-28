CC = g++
CPPFLAGS = -I
INCLUDE = ../lib/RCF-2.2.0.0/include/
SRCLIB = ../lib/RCF-2.2.0.0/src/RCF/RCF.cpp
DEPS_CI = ../include/ci_server.h
DEPS_PR = ../include/peer.h
LIB_OPTIONS = -lboost_system -lboost_filesystem -lpthread -ldl

all: ciServer peer1 peer2 peer3 

ciServer: ci_server_main.cpp ci_server.cpp $(DEPS_CI) 
	$(CC) ci_server_main.cpp ci_server.cpp $(SRCLIB) $(CPPFLAGS) $(INCLUDE) $(LIB_OPTIONS) -o ciServer

peer1: peer1_main.cpp peer.cpp $(DEPS_PR)    
	$(CC) peer1_main.cpp peer.cpp $(SRCLIB) $(CPPFLAGS) $(INCLUDE) $(LIB_OPTIONS) -o peer1

peer2: peer2_main.cpp peer.cpp $(DEPS_PR)
	$(CC) peer2_main.cpp peer.cpp $(SRCLIB) $(CPPFLAGS) $(INCLUDE) $(LIB_OPTIONS) -o peer2 

peer3: peer3_main.cpp peer.cpp $(DEPS_PR)
	$(CC) peer3_main.cpp peer.cpp $(SRCLIB) $(CPPFLAGS) $(INCLUDE) $(LIB_OPTIONS) -o peer3

clean:
	rm -rf *.o
