//
// COEN 146L lab3
// Yuya Oguchi
// 4/19/2016
// Client.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct packets{
    int packetNum;
    char data[255];
    int checksum;
};

//arg input filename hostname
int main(int argc, char *argv[]){
    if (argc<2) {
        printf("Usage: client <address>\n");
        return 1;
    }
    ////////// same for UDP TCP //////////////
    struct hostent *server;
    //info about IP itself
    struct sockaddr_in si_server;
    //local host name
    server = gethostbyname(argv[1]);
    bzero((char*)&si_server, sizeof(si_server));
    si_server.sin_family = AF_INET;
    //copy server content to si_server to avoid changing pointer
    bcopy((char*)server->h_addr,(char*)&si_server.sin_addr,server->h_length);
    //address of the local host port
    si_server.sin_port = htons(5555);
    //////////////////////////////////////////
    //////////// UDP Specified ///////////////
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    socklen_t socklength = sizeof(si_server);

    //init buffer and sizes client will get from server
    int i = 0;
    ////////////Start sending packet //////////////
    //create packet
    struct packets packet;
    struct packets buff;

    // packet to send
    for(i = 0; i < 5; i++){
        ///////////Send packet///////////
        char message[255];
        //read in message/set struct
        printf("Type in message to send\n");
        fgets(message,100,stdin);
        printf("message read\n");
        packet.packetNum = i;
        strcpy(packet.data,message);			

        packet.checksum =1;
        //send the packet
        printf("packet sending\n");
        sendto(sockfd,&packet,sizeof(packet),0,(struct sockaddr *)&si_server,socklength);
        printf("packet sent successfuly\n\n");

        ////////////Retrieve packet//////////////
        //receive new string 
        recvfrom(sockfd,&buff,sizeof(buff),0,(struct sockaddr *)&si_server, &socklength);
        if(buff.packetNum == i+1){
            printf("The received Packet has content: %s\n\n",buff.data);
        }else{
            printf("Packet dropped due to sequence number\n");
        }

    }	
    printf("Connection closed\n");

    return 0;
}
