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
#include <arpa/inet.h>


#define MESSAGE_SIZE 102400

int main(int argc,char **argv) {

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;


    if (argc != 2){
        fprintf(stderr,"usage: tcpclient <IPaddress>");
        exit(EXIT_FAILURE);
    }



    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_port = htons(12345);
    //serverAddr.sin_addr.s_addr = htonl();   //0x7f000001 127.0.0.1的16进制 htonl()用于把数字转为网络地址
    inet_pton(AF_INET, argv[1], &serverAddr.sin_addr); //inet_pton() 用于把 字符串值转换为网络地址
    serverAddr.sin_family = AF_INET;

    const struct sockaddr *connsockaddr;
    connsockaddr = &serverAddr;
    socklen_t socklen = sizeof(serverAddr);
    if (connect(socketfd, connsockaddr, socklen) < 0) {
        perror("connection");
        exit(EXIT_FAILURE);
    }
    //连接建立 开始发送数据
    char data[MESSAGE_SIZE];
    memset(data,'a', sizeof(data));
   //字符串常量参数声明
    const char *chars = &data;  //赋值
    size_t remaining = strlen(chars); //计算字符串的长度
    while (remaining) { //如果还有没法送完的长度
        // 向连接套接字发送数据，字符串，字符串长度，最后一个flags，一般为0，用于发送tcp紧急数据
        int n_written = send(socketfd, chars, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written <= 0) {
            fprintf(stderr, ": %s (%d)\n", strerror(errno),"send filed");
            break;
        }
        remaining -= n_written;
        chars +=n_written;
    }
    close(socketfd);
}

