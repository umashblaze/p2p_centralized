########### Peer to Peer System - Steps to follow ###############

1) Uncompress the provided p2p package submitted as part of the assignment. 

2) The p2p system code is written in c++ and it requires RCF and Boost libraries. 
	a) The RCF is an open source library that is already included in the zipped p2p package submitted. You can download separately by running the following commands:
		wget -i http://www.deltavsoft.com/downloads/RCF-2.2.0.0.tar.gz
		tar -xzvf -C lib
	b) Boost library is usually installed in ubuntu machines. If not in yours, it can be installed by using the following command:
		sudo apt-get install libboost-all-dev 

3) Note that for RCF to handle file transfer operations, RCF_FEATURE_FILETRANSFER has to be enaled explicitly in the configuration. This was already done in the provided p2p package, so only for a fresh download of RCF library the following step is required.
	Comment out the lines (134 to 141) of /lib/RCF-2.2.0.0/include/RCF/Config.hpp in the following way:
		// File transfer feature
		//#ifndef RCF_FEATURE_FILETRANSFER
		//#ifdef RCF_USE_BOOST_FILESYSTEM
		#define RCF_FEATURE_FILETRANSFER    1
		//#else
		//#define RCF_FEATURE_FILETRANSFER    0
		//#endif
		//#endif
	[Note: The line "#define RCF_FEATURE_FILETRANSFER  1" must be uncommented]

4) The server and peer binaries are provided as part of the p2p package. You can play around with them by following the steps given in the points 7, 8 and 9 below. If you want to do your own compilation, follow the steps as provided in the point 5 and 6.
	Further, the files for each peer is available at data folder, under the names as given by their respective endpoints. Each one has 10 files of sizes 1 KB, 2 KB,..., 10 KB.

5) Go to the source (src) folder and run the makefile.  
	[Note: Compilation in general takes time, as it links with 2 big libraries. This again depends on the system performance. In general, it might take upto 4 mins for compiling all the files. Also, we ain't using -std=c++11 constructs so to include it.]

6) If the makefile fails for some reasons, then you need to get the following:

	* Central Indexing Server Files: 
		../src/ci_server_main.cpp, ../src/ci_server.cpp and ../include/ci_server.h

	* Peer Server and Client Files:  
		../src/peer1_main.cpp, ../src/peer2_main.cpp, ../peer3_main.cpp, ../src/peer.cpp and ../include/peer.h

	* Data Files: 
		../data/50006/, ../data/50007/, ../data/50008/
	
	[Note: The peer endpoints (50006, 50007, 50008) are mapped as data folder names. Each peer's folder has 10 text files of varying file sizes (from 1 KB to 10 KB). The Central Indexing Server has its endpoint as "50001", but it has no folders.]

	Goto ../src folder and run the above files with the following commands, on 4 different terminal windows:
		(1) For Central Indexing Server: 
				g++ ci_server_main.cpp ci_server.cpp ../lib/RCF-2.2.0.0/src/RCF/RCF.cpp -I ../lib/RCF-2.2.0.0/include -lboost_system -lboost_filesystem -lpthread -ldl -o ciServer

		(2) For Peer1 (Server and Client):
				g++ peer1_main.cpp peer.cpp ../lib/RCF-2.2.0.0/src/RCF/RCF.cpp -I ../lib/RCF-2.2.0.0/include -lboost_system -lboost_filesystem -lpthread -ldl -o peer1

		(3) For Peer2 (Server and Client):
				g++ peer2_main.cpp peer.cpp ../lib/RCF-2.2.0.0/src/RCF/RCF.cpp -I ../lib/RCF-2.2.0.0/include -lboost_system -lboost_filesystem -lpthread -ldl -o peer2

		(4) For Peer3 (Server and Client):
				g++ peer3_main.cpp peer.cpp ../lib/RCF-2.2.0.0/src/RCF/RCF.cpp -I ../lib/RCF-2.2.0.0/include -lboost_system -lboost_filesystem -lpthread -ldl -o peer3

7) Run the binaries(ciServer, peer1, peer2 and peer3) in their respective windows, by executing as: ./<binary>
   [Note: Begin by executing "ciServer" binary first. Rest can be executed in any order]

8) Once you start the ciServer, it will wait for the peers to connect and register files with it. Execute the Peers too.

9) Executing any peer binary will register its files and wait for you to enter the file to lookup and download.
	a) Enter any filename as you want, but as long as you are entering the right filename as in the directories the requested file will be downloaded.
	b) Ensure the ciServer is always running. Just don't press any key as long as you want the operations to happen.
	c) Follow the instructions as given by executing the peer binaries on the terminals and play with it.

10) The modified code of the peers for performance evaluation (Average response time for search requests) and their binaries are available at the test folder. Their compilation can be done in a similar way as the original peer files (as given in point 6 above).
All the three peers can be executed simultaneously using the command: ./peer1Test & ./peer2Test & ./peer3Test



Thanks!

Author: Umashankar
Email: urajaram@hawk.iit.edu





	 

