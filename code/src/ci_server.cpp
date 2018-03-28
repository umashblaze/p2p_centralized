#include "../include/ci_server.h"


//CENTRAL_SERVER's METHOD-1: REGISTER THE PEER AND HASH THE PEER IDs
bool CentralServer::register_peer(int peer_id, vector<string>& files)
{   
    //Lock for unique access
    RCF::Lock lock(mapMutex);
    
    cout << "I_CentralServer service: Connected to " << peer_id  << endl;
    
    if(!files.empty()){
        
        for(vector<string>::const_iterator c_itr = files.begin(); c_itr != files.end(); ++c_itr){
           //Avoid duplicates 
           if(find(registry[*c_itr].begin(), registry[*c_itr].end(), peer_id) != registry[*c_itr].end()){
               continue;
           }
           else{
               registry[*c_itr].push_back(peer_id);
           }
        }
        cout << peer_id << "'s files mapped successfully!" << endl;
       
        /* 
        cout << "The mapped files are: " << endl;
        for(vector<string>::const_iterator c_itr = files.begin(); c_itr != files.end(); ++c_itr){
            cout << *c_itr << endl;
        }
        */
        
        return SUCCESS;
    }
    else{
        cout << "No Files exist!" << endl;
        return FAIL;
    }
}

//----------------------------------------------------------------------------

//CENTRAL_SERVER's METHOD-2: SEARCH THE FILE AND RETURN THE PEER LIST
vector<int> CentralServer::lookup_file(const string& file_name)
{   
    //Lock for unique access
    RCF::Lock lock(mapMutex);
    
    return registry[file_name];
}

//-------------------------------EOF------------------------------------------