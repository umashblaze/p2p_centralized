#include "../include/peer.h"

//Using POSIX APIs to get the file contents
#include <sys/types.h>
#include <dirent.h>

//Using C libraries for error messages
#include <cstring> 
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>

using std::strerror;

//----------------------------------------------------------------------------

//PEER's METHOD-1: IDENTIFY AND ENLIST THE FILES
bool Peer::enlist_my_files(void)
{
  string dir = get_my_dir_path();
  DIR *dp;
  struct dirent *dirp;

  if((dp  = opendir(dir.c_str())) == NULL) {
      cout << "Error in opening the directory!\n" << strerror(errno) << endl;
      return FAIL;
  }

  while ((dirp = readdir(dp)) != NULL) {
      if(dirp->d_name[0] != '.' && dirp->d_name[strlen(dirp->d_name)-1] != '~'){
        my_files.push_back(string(dirp->d_name));
      }
  }
  closedir(dp);
  
  //Update the file count 
  file_count = my_files.size();
  
  return SUCCESS;
}

//----------------------------------------------------------------------------

//PEER's METHOD-3: SHOW THE FILES
void Peer::display_my_files() const
{
  cout << my_id << "'s Files are:" << endl;
  for(vector<string>::const_iterator c_itr = my_files.begin(); c_itr != my_files.end(); ++c_itr){
      cout << *c_itr << endl;
  }
}

//----------------------------------------------------------------------------

//PEER's METHOD-4: REGISTER THE FILES
void Peer::register_my_files(const RCF::TcpEndpoint& ci_server_endpoint)
{
  RcfClient<I_CentralServer> tcp_client(ci_server_endpoint);
  bool result = FAIL;
   
  //Accessing RCF interface - for registering the peer files
  try{
    result = tcp_client.register_peer(my_id, my_files);
  }
  catch(const RCF::Exception & e){
     std::cout << "Error: " << e.getErrorString() << std::endl;
  }
  
  if(result){
      cout << "Peer " << get_my_id() << "'s files are registered." << endl;
  }
  else{
      cerr << "Cannot register Peer "<< get_my_id() << "'s files..\nPeer Exiting!" << endl;
      exit(EXIT_FAILURE);
  }
}

//----------------------------------------------------------------------------

//PEER's METHOD-5: CHECK IF THE FILE IS ALREADY PRESENT
bool Peer::check_if_present(const string& file_name)
{
  return find(my_files.begin(), my_files.end(), file_name) != my_files.end();
}

//----------------------------------------------------------------------------

//PEER-SERVER's METHOD: TRANSFER THE FILE TO THE REQUESTED CLIENT
void Server::obtain_file(const string& file_name, RCF::FileDownload filedownload)
{
  //Lock for unique access
  RCF::Lock lock(mapMutex);
  
  filedownload = RCF::FileDownload(string(get_my_dir_path() + file_name));
}

//----------------------------------------------------------------------------

//PEER-CLIENT's METHOD-1: SEARCH A FILE AND FIND THE SEED SERVERS
bool Client::search_file(const string& file_name, const RCF::TcpEndpoint& ci_server_endpoint)
{
  RcfClient<I_CentralServer> tcp_client(ci_server_endpoint);
  
  //#### TIME MEASURE BEGINS ####
  clock_t tic = clock();
  
  //Accessing RCF interface - for retrieving the seed servers
  try{
     seeds = tcp_client.lookup_file(file_name);
  }
  catch(const RCF::Exception & e){
     cerr << "Error: " << e.getErrorString() << std::endl;
  }
  
  clock_t toc = clock();
  //#### TIME MEASURE ENDS ####
  
  total_time += (double)(toc - tic) * 1000.0 / CLOCKS_PER_SEC;
 
  if(seeds.empty()){
      return FAIL;
  }
  else{
      cout << "The seed server(s) containing " << file_name << " is/are:" << endl;
      for(vector<int>::const_iterator c_itr = seeds.begin(); c_itr != seeds.end(); ++c_itr){
          cout << *c_itr << endl;
      }
  }
  return SUCCESS;
}

//----------------------------------------------------------------------------

//PEER-CLIENT's METHOD-2: CLIENT DOWNLOADS THE FILE AFTER FINDING THE PEER
bool Client::download_file(const string& file_name)
{
  RCF::FileDownload fileDownload;
  fileDownload.setDownloadToPath(Peer::get_my_dir_path());
  
  //Loop through all the seeds (source servers) and download the file from atleast one
  if(!seeds.empty()){
    for(vector<int>::const_iterator c_itr = seeds.begin(); c_itr != seeds.end(); ++c_itr)
    {
      //Pick one of the endpoints
      RCF::TcpEndpoint serv_endpoint = *c_itr;
      RcfClient<I_PeerServer> tcp_client(serv_endpoint);
      
      
      int result = FAIL;
      //Accessing RCF interface - for downloading the desired file from the chosen server
      try{
        tcp_client.obtain_file(file_name, fileDownload);
        result = SUCCESS;
      }
      catch(const RCF::Exception & e){
        cerr << "Error: " << e.getErrorString() << std::endl;
      }
      
      if(result){
        cout << "$$ Client " << get_my_id() << \
                  " has got " << file_name << " from Peer " << *c_itr << " $$" << endl;
        break;
      }
      else{//The requested server didn't respond!
          cout << "Server: " << *c_itr << \
                  "didn't serve the request! Let's try the next one..." << endl;
          continue;
      }
    }
  }
  else{//No seeds for the requested file
    cerr << "There is no server having the file!" << endl;
    return FAIL;
  }
  
  return SUCCESS;
}

//-----------------------------EOF--------------------------------