#include "../include/ci_server.h"

int main()
{
    RCF::RcfInitDeinit rcfInit;
    RCF::RcfServer server(RCF::TcpEndpoint(50001));
    
    //Bind the interface to the server
    CentralServer ci_server;
    server.bind<I_CentralServer>(ci_server);
    
    //Create server threads - dynamic
    RCF::ThreadPoolPtr tpPtr( new RCF::ThreadPool(1, 25) );
    server.setThreadPool(tpPtr);
    
    server.start();
    cout << "Central Indexing Server started..." << std::endl;
    
    cout << "Press Enter to exit..." << endl;
    cin.get();
    
    cout << "Central Indexing Server exits..." << std::endl;
    
    return 0;
}
