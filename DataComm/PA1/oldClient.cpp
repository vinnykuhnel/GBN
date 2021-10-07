// Adam Kuhnel Client Application
// vak58 Programming Assignment 1

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

using namespace std;

int main(int argc, char *argv[]){
	if (argc != 4){
		cout << "Hostname, port, and textfile must be specified\n";
		return 0;
	}
	int list = atoi(argv[2]);
	
	struct hostent *s;
	s = gethostbyname(argv[1]);
	struct sockaddr_in server;
	int sock = 0;
	socklen_t slen = sizeof(server);

	char buffer[512] = "1234";
	int port;
	int *random = &port;
	

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		cout << "Error creating socket.\n";
	}

	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(list);
	bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);
	if((sendto(sock, buffer, 8, 0, (struct sockaddr *)&server, slen)) == -1){
		cout << "failed to send message\n";

}

	
	if((recvfrom(sock, random, sizeof(int), 0, (struct sockaddr *)&server, &slen)) == -1) { cout << "Failed in the recvfrom function\n";}

	port = ntohs(port);
       	//close socket initialize a new socket on the randomly generated port		
	cout << port << endl;
	close(sock);

	//Create and Bind a socket to send file to server on random port
  	struct sockaddr_in fileReciever;
	sock = 0;
	char buff[5];
	char capital[5];
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		cout << "Error creating socket.\n";
	}
	memset((char *) &fileReciever, 0, sizeof(fileReciever));
	fileReciever.sin_family = AF_INET;
	fileReciever.sin_port = htons(port);
	bcopy((char *)s->h_addr, (char *)&fileReciever.sin_addr.s_addr, slen);
 	//set up variables for file to be read in
	ifstream in(argv[3]);
	if(!in){
	cout << "Error! File does not exist";
	return 0;}
	//read file and transmit it 4 characters at a time

	while(in.eof() == 0){
	in.read(buff, 4);
	//handle if remainder of 4 is nonzero
	if(in.gcount() < 4){
		for(int i = 3; i >= in.gcount(); i--){
			buff[i] = '\0';
		}	
	}
	buff[4] = '\0';
	//transmit and recieve capitalized response
	if((sendto(sock, buff, sizeof(buff) , 0, (struct sockaddr *)&fileReciever, slen)) == -1){ cout << "Failed to send message!\n";}
	
	if((recvfrom(sock, capital, sizeof(capital), 0, (struct sockaddr *)&fileReciever, &slen)) == -1){ cout << "Failed to recieve the message!\n";}
	cout << "Data Recieved: " << capital << endl;
	
	}
	//send tremination message at the end of the file
char str[] = "StOP";	
if((sendto(sock, str, sizeof(str), 0, (struct sockaddr *)&fileReciever, slen)) == -1){ cout << "Failed to send message!\n";}
	
	close(sock);
	in.close();	
}
	
