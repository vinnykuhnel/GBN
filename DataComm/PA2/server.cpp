	// Adam Kuhnel
// Date October 23, 2021

#include <stdlib.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <time.h>        // used for random number generation
#include <string.h>
#include <fstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "packet.h" // include packet class
#include <unistd.h>



	int main(int argc, char *argv[]){
    if(argc != 3){
        std::cout << "Please give a valid port and output filename!" << std::endl;
        return 0;
    }
	//Recieves Servers logging info
	std::ofstream LogFile("arrival.log");

	//bind socket and setup random port to initiate file transfer
	int port = atoi(argv[1]);
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock = 0;
	socklen_t clnt = sizeof(client);

    //Create server side socket and bind it to given port

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		std::cout << "Error creating socket";
		return -1;
	}
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		std::cout << "Error binding socket";
		return -1;
	}
    
	//listen until client sends EOT packet
    bool notEOT = true;
	int currPack = 0;
	std::ofstream TxtFile(argv[2]);
	
	while (notEOT) {
		char buffer[512];
		memset(buffer, 0, 512);
		packet pack(0, 0, 0, buffer);
		
		if ((recvfrom(sock, buffer, 512, 0, (struct sockaddr *)&client, &clnt)) == -1){
			std::cout << "failed to recieve message\n";
			return -1;
		}

		pack.deserialize(buffer);
		//Close connection on Client EOT packet
		if(pack.getType() != 3){
			//If expected packet
			if(pack.getSeqNum() == (currPack % 8)){
				//write to output file and send acknowledgement
				TxtFile << pack.getData();
				LogFile << pack.getSeqNum() << '\n';
				packet ACK(0, currPack % 8, 0, buffer);
				char acknowledge[512];
				memset(acknowledge, 0, 512);
				ACK.serialize(acknowledge);
				if((sendto(sock, acknowledge, strlen(acknowledge), 0, (struct sockaddr *)&client, clnt)) == -1){
					std::cout << "Error in failed sendto\n";
					return -1;
				}
				std::cout << currPack << std::endl;
				currPack++;
			}
			//If not expected packet
			else{continue;}
		}
		//End of Transmission case
		else{
			LogFile << pack.getSeqNum();
			//send transmission termintation packet
			packet eotPack(2, pack.getSeqNum(), 0, NULL);
            char eot[512];
            memset(eot, 0, 512);
            eotPack.serialize(eot);

            if((sendto(sock, eot, strlen(eot) - 1, 0, (struct sockaddr *)&client, clnt)) == -1){
                    std::cout << "failed to send termination packet\n";
                    return -1;
            }

			notEOT = false;
		}
		
	}
	//release socket and file handles
	LogFile.close();
	TxtFile.close();
	close(sock);
	return 0;
}

