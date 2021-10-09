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
#include <errno.h>
#include <signal.h>
#include <chrono>

void CatchAlarm(int ignored);
//Create global ints to hold window position, allows changes to be made upon interupts
int sendBase, nextSeq = 0;
bool timedOut = false;

struct frame
{
    char data[512];
};


int main(int argc, char *argv[]){
    if(argc != 4){
        std::cout << "Please give the DNS name, port number, and file to be transmitted!" << std::endl;
    }

    //create signal handler for timeout condition
    struct sigaction timeAction;
    timeAction.sa_handler = CatchAlarm;
	if(sigfillset(&timeAction.sa_mask) < 0){
      std::cout << "sigfilset() failed" << std::endl;
      return -1;
    }

   timeAction.sa_flags = 0;

   if(sigaction(SIGALRM, &timeAction, 0) < 0){
      std::cout << "sigaction() failed" << std::endl;
      return -1;
   }
   //associate timeout interrupt with our action handler function


    //prepare a sockaddr_in for the server being transmitted to
    struct hostent *s;
	s = gethostbyname(argv[1]);
    int port = atoi(argv[2]);
    struct sockaddr_in server;
	int sock = 0;
	socklen_t slen = sizeof(server);

    //create socket and get handle
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		std::cout << "Error creating socket.\n";
        return -1;
	}

    //zero out server memory and set host/port fields
    memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
    bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);

    //Data to be sent from given input file
    char buff[512];
    std::ifstream in(argv[3]);
	if(!in){
        std::cout << "Error! File does not exist";
        return -1;
    }
    //keep track of current iteration and create window data structure
    frame window[8];
    
    //loop while true
    bool transmitting = true;

    while(transmitting){
        if(timedOut && nextSeq != ((sendBase + 7) % 8)){
            if((sendto(sock, window[nextSeq].data, strlen(window[nextSeq].data), 0, (struct sockaddr *)&server, slen)) == -1){
                std::cout << "failed to send message\n";
                return -1;
            }
            if(sendBase == nextSeq){alarm(2);}
            std::cout << nextSeq << std::endl;
            nextSeq = ((nextSeq + 1) % 8); 
            continue;
        }
        //set timeout to false after all retransmissions have been handled
        timedOut = false;

        if(in.eof() == 0 && nextSeq != ((sendBase + 7) % 8)){
            //read 30 characters from file until and handle the final modulo case
            in.read(buff, 30);
            
            //handle if remainder of 4 is nonzero
            if(in.gcount() < 30){
                for(int i = 29; i >= in.gcount(); i--){
                    buff[i] = '\0';
                }	
            }
            //create packet for newly read data
            packet pack(1, nextSeq, strlen(buff) + 1, buff);
            //prepare frame state and data buffer
            memset(window[nextSeq].data, 0, 512);

            //give serialized data to current frame
            pack.serialize(window[nextSeq].data);
            if((sendto(sock, window[nextSeq].data, strlen(window[nextSeq].data), 0, (struct sockaddr *)&server, slen)) == -1){
                std::cout << "failed to send message\n";
                return -1;
            }
            //If sendbase equals next sequence number then start a timer for 2 seconds
            if(sendBase == nextSeq){alarm(2);}
            
            std::cout << nextSeq << std::endl;
            std::cout << window[nextSeq].data << std::endl;
            nextSeq = ((nextSeq + 1) % 8);          
            continue;
        }
        //Window is filled so read socket buffer for Acks
        packet recievedACK(0, 0, 0, buff);
        char temp[512];
        memset(temp, 0, 512);
        if ((recvfrom(sock, temp, 512, 0, (struct sockaddr *)&server, &slen)) == -1){
                    std::cout << "failed to recieve message\n";
        }
        //Continue next loop execution upon timeout
        if(timedOut){continue;}

        recievedACK.deserialize(temp);
        if(sendBase != ((recievedACK.getSeqNum() + 7) % 8)){
            sendBase = ((recievedACK.getSeqNum() + 1) % 8);
            //reset alarm and restart for another 2 second window if SB has not caught up to nextseq
            alarm(0);
            alarm(2);
            if(sendBase == nextSeq){
                transmitting = false;
                alarm(0);
            }
            

        }
        
    }
    //Create and send eot packet
    packet eotPack(3, 0, 0, buff);
            char eot[512];
            memset(eot, 0, 512);
            eotPack.serialize(eot);

            if((sendto(sock, eot, strlen(eot) - 1, 0, (struct sockaddr *)&server, slen)) == -1){
                    std::cout << "failed to send message\n";
                    return -1;
            }
    

    close(sock);
    return 0;

}


void CatchAlarm(int ignored){
   alarm(0);
   nextSeq = sendBase;
   timedOut = true;
}
