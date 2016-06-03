//
// COEN 146L lab3
// Yuya Oguchi
// 4/19/2016
// server.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


struct packets{
    int packetNum;
    char data[255];
    int checksum;
};

int main(int argc, char *argv[]){
    ////////// same for UDP TCP //////////////
    //info about IP itself
    struct sockaddr_in si_server;
    bzero((char*)&si_server, sizeof(si_server));
    si_server.sin_family = AF_INET;
    //copy server content to si_server to avoid changing pointer
    si_server.sin_addr.s_addr = INADDR_ANY;
    si_server.sin_port = htons(5555);
    //////////////////////////////////////////
    //////////// UDP Specified ///////////////
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    bind(sockfd,(struct sockaddr *)&si_server, sizeof(si_server));

    //init buffer and sizes server will get from client
    int i=0;
    char string[255];

    //length of name of socket addr
    socklen_t l = sizeof(si_server);	

    //create return packet
    struct packets returnpacket;
    returnpacket.checksum=1;
    returnpacket.packetNum=1;
    strcpy(returnpacket.data,"HI");
    ////////////Start receiving packets///////////
    for(i =0; i< 5; i++){
        ///////Receive packets/////////
        //receive new packet
        recvfrom(sockfd,&returnpacket, sizeof(returnpacket), 0, (struct sockaddr*)&si_server, &l);
        //check for data content
        if(returnpacket.packetNum == i){
            printf("Package received\n");
            returnpacket.packetNum = i+1;
            strcpy(string, returnpacket.data);
            int j =0;
            while(string[j]){
                string[j] = toupper(string[j]);
                j++;
            }
            strcpy(returnpacket.data,string);
            returnpacket.checksum = 1;
            //////////send back the packet//////////////
            sendto(sockfd,&returnpacket,sizeof(returnpacket),0,(struct sockaddr *)&si_server,l);
            printf("packet sent back to client\n");
        }else{
            printf("package dropped\n");
        }
    }
    printf("Connection closed\n");
    return 0;
}

