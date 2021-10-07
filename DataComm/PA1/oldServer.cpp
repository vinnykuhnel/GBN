// Adam Kuhnel Server Application
// vak58 Programmin  assignment 1
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include <cctype>
#include <netinet/in.h>
#include <time.h>
#include <algorithm>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){
	if (argc != 2)
	{
		cout << "Give a valid port to listen" << endl;
		return 0;
	}
	//bind socket and setup random port to initiate file transfer
	int list = atoi(argv[1]);
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock = 0;
	socklen_t clnt = sizeof(client);
	char buffer[512];
	srand(time(NULL));
	int RandPort = rand() % 64510 + 1024;
	int converted = htons(RandPort);
	int *conv = &converted;

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		cout << "Error creating socket";
	}
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(list);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		cout << "Error binding socket";
	}

	if (recvfrom(sock, buffer, 512, 0, (struct sockaddr *)&client, &clnt) == -1){
		cout << "failed to recieve message\n";
	}

	//After recieving the clients message, transmit over random port for fileshare

	if ((sendto(sock, conv, sizeof(int), 0, (struct sockaddr *)&client, clnt)) == -1){ cout << "Failed in sendto";}

	cout << "The random port chosen is <" << RandPort << ">" << endl;

	//close socket and establish a new connection for the file transfer
	close(sock);


	/*
	 Stage 2: File transfer
	 Initialize all variables to recieve and acknowledge	 
	 */
	
	memset((char *) &server, 0, sizeof(server));
	memset((char *) &client, 0, sizeof(client));
	sock = 0;
	char recieved[5];
	char response[5];
	ofstream TxtFile("blah.txt");
	//open socket on random port
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		cout << "Error creating socket";}
	
	server.sin_family = AF_INET;
	server.sin_port = htons(RandPort);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1){
			cout << "Failed to bind socket\n";}
	
	//recieve 4 characters of file  file until end is reached
	while(1){
		if ((recvfrom(sock, recieved, 5, 0, (struct sockaddr*)&client, &clnt)) == -1){ cout << "Failed to recieve data";} 
	
		if((strcmp(recieved, "StOP")) == 0){break;}
		TxtFile << recieved;
		for(int i = 0; i < 5; i++){
			response[i] = toupper(recieved[i]);
		}
		if((sendto(sock, response, 5, 0, (struct sockaddr*)&client, clnt)) == -1){ cout << "Failed to send to client\n";}

		
		}
	TxtFile.close();
	close(sock);
	return(0);
		
}
