//
// Created by 周春吉 on 2019/10/22.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>

int main(){

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_port = htons (12345);
    serverAddr.sin_addr.s_addr = htonl("127.0.0.1");
    serverAddr.sin_family = AF_INET;

    bind(socketfd,(struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if(connect(socketfd,(struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        perror("connection");
        exit(EXIT_FAILURE);
    }

    char * readChars;
    readChars=malloc(1024);
    memset(readChars,0,1024);
    readChars[1024]='\0';


    read(socketfd, readChars, sizeof(readChars));

//    bind(socketfd,)

}