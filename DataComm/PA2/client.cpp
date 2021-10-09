// Author: Adam Kuhnel
// Due:    October 20, 2021

#include <stdlib.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>   // if you want to use inet_addr() function
#include "packet.h" // include packet class
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
using namespace std;



int main(int argc, char *argv[]){
    if(argc != 4){
        cout << "Please give the DNS name, port number, and file to be transmitted!" << endl;
    }
    //prepare a sockaddr_in for the server being transmitted to
    struct hostent *s;
	s = gethostbyname(argv[1]);
    int port = atoi(argv[2]);
    struct sockaddr_in server;
	int sock = 0;
	socklen_t slen = sizeof(server);

    //create socket and get handle
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		cout << "Error creating socket.\n";
	}

    //zero out server memory and set host/port fields
    memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
    bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);

    //Data to be sent
    char buff[512];
    ifstream in(argv[3]);
	if(!in){
        cout << "Error! File does not exist";
        return -1;
    }
    //keep track of current iteration and hold list of serialized packets already transmitted
    int packetNum = 0;
    char *sent[8];

    while(in.eof() == 0){
        //read 30 characters from file until and handle the final modulo case
        in.read(buff, 30);
        packetNum++;
        //handle if remainder of 4 is nonzero
        if(in.gcount() < 30){
            for(int i = 29; i >= in.gcount(); i--){
                buff[i] = '\0';
            }	
        }

        packet pack(1, packetNum % 8, strlen(buff), buff);
        char serial[512];
        memset(serial, 0, 512);
        pack.serialize(serial);
        sent[packetNum % 8] = serial;
        if((sendto(sock, serial, strlen(serial) - 1, 0, (struct sockaddr *)&server, slen)) == -1){
            cout << "failed to send message\n";
        }
    }
    

    close(sock);
    return 0;

}
