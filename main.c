#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>


int main() {
    printf("Hello, World!\n");
    struct sockaddr_in inetSocketAddr, clientSocketAddr;
    socklen_t clilen;
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        puts("error");
    }

    inetSocketAddr.sin_family = AF_INET;
    inetSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inetSocketAddr.sin_port = htons(12345);

    struct sockaddr *sockaddr = &inetSocketAddr;
    int sockaddr_len = sizeof(inetSocketAddr);
    int bindsockfd = bind(socketfd, sockaddr, sockaddr_len);
    if (bindsockfd < 0) {
        perror(errno);
        exit(EXIT_FAILURE);
    }

    if (listen(socketfd, 1024) < 0) {
        perror(errno);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        clilen = sizeof(clientSocketAddr);
        int acceptsocketfd = accept(socketfd, clientSocketAddr, sockaddr_len);
        if (acceptsocketfd > 0) {
            recvfrom()
        }
    }


    return 0;
}