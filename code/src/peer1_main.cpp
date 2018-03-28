#include"../include/peer.h"

int main()
{
    RCF::RcfInitDeinit rcfInit;
    
    //PEER-1 Details:
    int peer_port = 50006;
    string peer_dir_path = "../data/50006/";
    
    //TcpEndpoint of Central Indexing Server
    const RCF::TcpEndpoint ci_server_endpoint = 50001;
    
    Peer p1(peer_port, peer_dir_path);
   
    //Explore peer's directory and record the files
    p1.enlist_my_files();
    
    //Display peer's directory contents
    p1.display_my_files();
    
    //Register
    p1.register_my_files(ci_server_endpoint);
    
    //-----------SERVER-------------
	//Initialize Peer's Server
    Server ps(peer_port, peer_dir_path);
        
    RCF::TcpEndpoint server_endpoint = peer_port;
    RCF::RcfServer tcp_server(server_endpoint);
    
    //Bind the RCF-Interface to the server
    tcp_server.bind<I_PeerServer>(ps); 
        
    //Create server threads - dynamic
    RCF::ThreadPoolPtr tpPtr(new RCF::ThreadPool(1, 25) );
    tcp_server.setThreadPool(tpPtr);
    
    //Start the server
    tcp_server.start();
    cout << "Peer " << peer_port << "'s Server started and threads are set." << endl;
	
    //-----------CLIENT---------------
	//Initialize Peer's Client
    //Client pc = new Client(peer_port, peer_dir_path);
    Client pc(peer_port, peer_dir_path);
    cout << "Peer " << peer_port << "'s Client operation begins..." << endl;
    
    string in_fileName;
    int prev_file_count = 0;
    int loop_over = 1;
    
    for(;;){
         cout << "Enter the name of the file you'd like to download:" << endl;
         
         getline(std::cin, in_fileName);
         if(cin.fail())
         {
            cerr << "Error reading from STDIN! Exiting..." << std::endl;
            exit(EXIT_FAILURE);
         }
         
         //Checking peer's directory if the file exists
         if (p1.check_if_present(in_fileName)){
            cout << in_fileName << " already exists for client " << \
                    peer_port << "! Try another file.." << endl;
                    
         }
         else{
             //Search the file
             if(pc.search_file(in_fileName, ci_server_endpoint)){
                
                //Download the file
                if(pc.download_file(in_fileName)){
                    prev_file_count = p1.get_file_count();
                    
                    //Record the file's presence
                    if(p1.enlist_my_files()){
                        if(p1.check_if_downloaded(prev_file_count)){
                            cout << "Download Success!" << endl;
                        }
                        else{
                            cerr << "It seems there's a problem with the download!" << endl;
                        }
                    }
                    else{
                        cerr << "Error: In opening" << peer_port << "'s directory!" << endl;
                        break;
                    }
                    
                    //Informing the file's presence
                    p1.register_my_files(ci_server_endpoint);
                }
                else{//Download didn't happen!
                    cout << "The requested file isn't downloaded! Try again.." << endl;
                }
             }
             else{
                 cout << "No seed sources for the given file! Try another file.." << endl;
             }
         }
         
         cout << "#---------------------------------------------" << endl;
         cout << "Wanna try a new file? \
                 \n* Enter 1 to continue, Or else\
                 \n* Enter 0 to exit." << endl;
            
         cin >> loop_over;
         cin.ignore();
         
         if(!loop_over){
             break;
         }
    }
   
    //Exit
    cout << "Peer " << peer_port << " exits..!" << endl;
    
    return 0;
}