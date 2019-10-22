//
// Created by 周春吉 on 2019/10/22.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = htonl(0x7f000001);
    serverAddr.sin_family = AF_INET;
//
//    if(bind(socketfd,(struct sockaddr *)&serverAddr, sizeof(serverAddr)) <0 ){
//        perror("bind");
//        exit(EXIT_FAILURE);
//    }

    const struct sockaddr *connsockaddr;
    connsockaddr = &serverAddr;
    socklen_t socklen = sizeof(serverAddr);
    if (connect(socketfd, connsockaddr, socklen) < 0) {
        perror("connection");
        exit(EXIT_FAILURE);
    }

    char readchaa[1024];
    ssize_t readlen;
    for (;;) {
        memset(&readchaa,"\0",1024);
        char *readChars = &readchaa;
        readlen=0;
        while (readlen < 1024) {
            int len=0;
            len = read(socketfd, readChars, 1024-readlen);
            if (len < 0) {
                if (errno == EINTR)
                    continue;
                else
                    return (-1);
            } else {
                if (len == 0) {
                    break;
                }
            }
            readlen += len;
            readChars += len;
        }
    //    bind(socketfd,)
        fprintf(stdout, "read 1024byte for %s \n",readchaa );
    }

}