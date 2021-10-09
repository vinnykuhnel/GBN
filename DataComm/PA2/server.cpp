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
using namespace std;


	int main(int argc, char *argv[]){
    if(argc != 3){
        cout << "Please give a valid port and output filename!" << endl;
        return 0;
    }

	//bind socket and setup random port to initiate file transfer
	int port = atoi(argv[1]);
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock = 0;
	socklen_t clnt = sizeof(client);

    //Create server side socket and bind it to given port

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		cout << "Error creating socket";
	}
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		cout << "Error binding socket";
	}
    //client transmits a serialized packet object
    
	while (1) {
		char buffer[512];
		memset(buffer, 0, 512);
		packet pack(0, 0, 0, buffer);
		
		if (recvfrom(sock, buffer, 512, 0, (struct sockaddr *)&client, &clnt) == -1){
			cout << "failed to recieve message\n";
		}
		cout << buffer << endl;
		pack.deserialize(buffer);
		pack.printContents();
		
	}
	close(sock);
	return 0;
}

