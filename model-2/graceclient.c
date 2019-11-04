//
// Created by 周春吉 on 2019/10/29.
//
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>


int main(int argc, char **argv) {
    //校验参数个数
    if (argc != 3) {
        fprintf(stderr,"usage: graceclient <IPaddress> <port>");
        exit(1);
    }

    //创建sockt
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0){
        perror("socket");
        exit(1);
    }

    //建立连接
    struct sockaddr_in serverAddr_in;
    bzero(&serverAddr_in, sizeof(serverAddr_in));
    serverAddr_in.sin_family= AF_INET;
    serverAddr_in.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &serverAddr_in.sin_addr);

    connect(socketfd,(struct socketaddr *)&serverAddr_in,sizeof(serverAddr_in));




    return 0;
}
