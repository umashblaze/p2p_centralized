#ifndef PEER_H
#define PEER_H

#include <RCF/RCF.hpp>
#include <SF/vector.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

#include <unistd.h>
#include <sys/wait.h>

#define SUCCESS 1
#define FAIL 0
#define SEMANTICS_ON

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::getline;


//------------------------------------------------------------------
//RCF Interface for Central Indexing Server and Peer communications
RCF_BEGIN(I_CentralServer, "I_CentralServer")
    RCF_METHOD_R2(bool, register_peer, int, vector<string>&)
    RCF_METHOD_R1(vector<int>, lookup_file, const string&)
RCF_END(I_CentralServer)
//------------------------------------------------------------------
//RCF Interface for Peer to Peer File transfer
RCF_BEGIN(I_PeerServer, "PeerServer")
    RCF_METHOD_V2(void, obtain_file, const string&, RCF::FileDownload)
RCF_END(I_PeerServer)
//------------------------------------------------------------------


//CLASS: PEER
class Peer
{
  public:
    Peer(){}
    ~Peer(){}
    
    Peer(int value, string path): my_id(value), my_dir(path), my_files(1), file_count(0){};

    int get_my_id(void){return my_id;}
    string get_my_dir_path(void){return my_dir;}
    bool enlist_my_files(void);
    vector<string> get_my_files(void){return my_files;}
    int get_file_count(void){return file_count;}
    bool check_if_downloaded(int prev_count){return file_count > prev_count;}
    bool check_if_present(const string&);
    void display_my_files() const;
    void register_my_files(const RCF::TcpEndpoint&);
  
  protected:
    int my_id; //Peer's port number
    string my_dir; //Peer's directory
    vector<string> my_files; //Peer's files
    int file_count;
    RCF::Mutex mapMutex;
};


//CLASS: PEER's SERVER
class Server : public Peer
{
  public:
    Server(){}
    ~Server(){}
    
    Server(int value, string path): Peer(value, path){};
    
    //An RCF-Interface method:
    void obtain_file(const string&, RCF::FileDownload);
};


//CLASS: PEER's CLIENT
class Client : public Peer
{
  public:
    Client(){}
    ~Client(){}
    
    Client(int value, string path): Peer(value, path){}
    
    bool search_file(const string&, const RCF::TcpEndpoint&);
    bool download_file(const string& file_name);
    double get_total_search_time(void){return total_time;}
    void set_total_search_time(double val){total_time = val;}
    
  private:
    vector<int> seeds; //To store the server-ids that has the requested file
    double total_time;
  
};

#endif 