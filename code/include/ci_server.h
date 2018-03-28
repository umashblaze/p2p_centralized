#ifndef SERVER_H
#define SERVER_H

#include <RCF/RCF.hpp>
#include <SF/vector.hpp>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <map>

#define SUCCESS 1
#define FAIL 0

using std::cin;
using std::cout;
using std::endl;
using std::iterator;
using std::string;
using std::vector;
using std::map;
using std::make_pair;
using std::find;


//-------------------------------------------------------------------
//RCF Interface for Central Indexing Server and Peer communications
RCF_BEGIN(I_CentralServer, "I_CentralServer")
    RCF_METHOD_R2(bool, register_peer, int, vector<string>&)
    RCF_METHOD_R1(vector<int>, lookup_file, const string&)
RCF_END(I_CentralServer)
//------------------------------------------------------------------


//CENTRAL INDEXING SERVER CLASS
class CentralServer
{
    public:
       CentralServer(){cout << "Central Server is Created" << endl;};
       ~CentralServer(){cout << "Central Server is Destroyed" << endl;};
       
       //RCF-Interface Methods:
       bool register_peer(int, vector<string>&);
       vector<int> lookup_file(const string&);
       
    private:
       RCF::Mutex mapMutex;
       map<string, vector<int> > registry;
};

#endif