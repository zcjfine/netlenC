//
// Created by zhoucj on 2019/10/22.
//
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


/* 创建tcp 服务器 */
int main(){
    //1 声明变量,服务器端地址,服务器端
    int listenfd,connectfd; //监听套接字描述符,连接套接字描述符
    socklen_t clielen ; //客户端 套接字调用中长度参数的类型。
    struct sockaddr_in serverAddr,clientAddr;//申明服务器端,客户端 ipv4 地址
    //创建ipv4,tcp socket套接字 返回新套接字的文件描述符，如果错误则返回-1。
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {  //小于零 创建socket套接字出错
        printf("create socket %m",errno);
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr,0, sizeof(serverAddr)); //先把每个字节都初始化为0 ,总长 16字节
    serverAddr.sin_family=AF_INET; //设置地址类型 为 ipv4地址 长度2字节
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //设置监听 本地多个网卡上的IP地址 ,转换为 4位字节无符号int
    serverAddr.sin_port = htons(12345); //设置监听 12345端口 ,转换为2位字节无符号 short int
//    serverAddr.sin_zero
//    sin_zero用来将sockaddr_in结构填充到与struct sockaddr同样的长度， 8字节填充
//    可以用bzero()或memset()函数将其置为零。本例中先全部置零,然后把有用字节赋值.
//    指向sockaddr_in的指针和指向sockaddr的指针可以相互转换，
//    这意味着如果一个函数所需参数类型是sockaddr类型时，
//    你可以在函数调用的时候将一个指向sockaddr_in的指针转换为指向sockaddr的指针；或者相反。

    if(bind(listenfd,(struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
        perror("bind socket %m");
        exit(EXIT_FAILURE);
    }

    //监听socket backlog为1024  表示等待连接的客户端队列最大值为1024,再后来会直接拒绝连接. 0成功 ,-1失败
    if(listen(listenfd, 1024) < 0){
        perror("bind socket ");
        exit(EXIT_FAILURE);
    }

    //循环接收客户端连接请求
    for (;;) {
        clielen = sizeof(clientAddr); //计算 ipv4类型地址长度
        //等待接收客户端连接请求,
        // 连接到达后，打开一个新的套接字与之通信，
        // 将* ADDR（长* ADDR_LEN字节）设置为连接对等方的地址，
        // 并将* ADDR_LEN设置为该地址的实际长度，
        // 然后返回新套接字的描述符，或者-1表示错误。
        connectfd = accept(listenfd,(struct sockaddr *) &clientAddr,&clielen);
        if(connectfd < 0){
            perror("accept connetion");
            exit(0);
        }
        char readchaa[1024];
        for (;;) {
            size_t readlen = sizeof(readchaa);
            char *readChars = &readchaa;
            while (readlen > 0) {
                int len=0;
                len = read(connectfd, readChars, readlen);
                if (len < 0) {
                    if (errno == EINTR)   /* 考虑非阻塞的情况，这里需要再次调用read */
                        continue;
                    else
                        perror("read");
                        return (-1);
                } else {
                    if (len == 0) {  /* EOF(End of File)表示套接字关闭 */
                        break;
                    }
                }
                readlen -= len;
                readChars += len;
            }
            //    bind(socketfd,)
            if(readlen < 1024) {
                fprintf(stdout, "read 1024byte for %s \n", &readchaa);
            } else
                break;
        }

    }
}
