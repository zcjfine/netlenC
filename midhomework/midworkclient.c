//
// Created by zhoucj on 2019/11/2.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>


/**************
tcp客户端
************/

#define port 34567

#define msgHeadLeng 8

#define maxBuflen  4096

ssize_t readn(int fd, void *buf, size_t nbytes) {
	char *buftemp = buf;
	int readlen = nbytes;
	while (readlen > 0){
		int len = read(fd, buftemp, readlen);
		if (len < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				perror("readn return -1");
				return (-1);
			}
		}else if (len == 0) {
			perror("readn return 0 EOF");
			break;
			//EOF
		} else {
			readlen -= len;
			buftemp += len;
		}
	}
	return nbytes-readlen;
}

//int readHead(int socketFd,void *buf,size_t nbytes){
//	int readlen = readn(socketFd, buf, nbytes);
//	if(readlen < 0){
//		perror("readn error");
//		return -1;
//	} else if(readlen != msgHeadLeng){
//		fprintf(stderr, "read msg head len != %d :readlen= %d", msgHeadLeng,readlen);
//		return -1;
//	}
//
//}

int sendn(int fd,void *buf,size_t size,int headSize){
	int sizetemp=size;
	int headvalue=1;
	while(sizetemp/10>0){
		headvalue++;
		sizetemp=sizetemp/10;
	}
	char headbuf[headSize];
	memset(&headbuf, 0,headSize);
	memset(&headbuf, '0', headSize - headvalue);
	sprintf(&headbuf, "%s%d",&headbuf, sizetemp);
	if(send(fd, &buf, headSize, 0) < 0){
		fprintf(stderr, "send msg head error");
		return -1;
	}
	if(send(fd, buf, size, 0) < 0){
		fprintf(stderr, "send msg body error");
		return -1;
	}
}

int main(int argc, char **argv){
    //判断入参
    if(argc != 2){
        fprintf(stderr, "usage:midworkclient <IPaddress>");
        exit(1);
    }
    //创建tcp socket
    int socketFd= socket(PF_INET, SOCK_STREAM, 0);
    if(socketFd < 0 ){
        perror("socket");
        exit(1);
    }
    //连接服务器套接字地址
    struct sockaddr_in sockaddrv4;
    bzero(&sockaddrv4, sizeof(sockaddrv4));
    sockaddrv4.sin_family=AF_INET;
    sockaddrv4.sin_port = htons(port);
    inet_pton(AF_INET,argv[1],&sockaddrv4.sin_addr);

    //建立连接
    int result = connect(socketFd,(const struct sockaddr *)&sockaddrv4,sizeof(sockaddrv4));
    if(result < 0){
        perror("connect");
        exit(1);
    }

    fd_set fdSet;
    fd_set redSet;
    FD_ZERO(&fdSet);
    FD_SET(socketFd,&fdSet);
    FD_SET(0,&fdSet);

    char headbuf[msgHeadLeng]={'\0'};

    char inputbuf[1024]={'\0'};
    while(1) {
        redSet=fdSet;
        int fdmaks = select(socketFd + 1, &redSet, NULL, NULL, NULL);
        if(fdmaks < 0) {
            fprintf(stderr,"select error %m");
            continue;
        }
        if(FD_ISSET(socketFd,&redSet)){
            //可以读取数据
			int readlen=readn(socketFd, &headbuf, msgHeadLeng);
			if(readlen < 0){
				perror("readn error");
				FD_CLR(socketFd,&fdSet);
			} else if(readlen != msgHeadLeng){
				fprintf(stderr, "read msg head len != %d :readlen= %d", msgHeadLeng,readlen);
				FD_CLR(socketFd,&fdSet);
			}
			int msgbodylen = atoi(headbuf);
			char bodybuf[msgbodylen+1];
			memset(bodybuf, '\0', msgbodylen);
			int bodylen = readn(socketFd, &bodybuf, msgbodylen);
			if(bodylen < 0){
				perror("readn error");
				FD_CLR(socketFd,&fdSet);
			}
			fprintf(stdout,"server echo: \n%s\n please intput:",&bodybuf);
        }
		if (FD_ISSET(0, &redSet)) {

			if(fgets(&inputbuf,maxBuflen,stdin) !=NULL){
				if(strncmp(&inputbuf,"quit",4)==0){
					fprintf(stdout, "client exit for quit");
//					shutdown(socketFd,SHUT_RDWR);
					exit(0);
				} else if(strncmp(&inputbuf,"pwd",3)==0){
					if(sendn(socketFd, &inputbuf, 3, msgHeadLeng) < 0){
						perror("send error");
						exit(1);
					}
				} else if(strncmp(&inputbuf,"ls",2)==0){
					if(sendn(socketFd, &inputbuf, 2, msgHeadLeng) < 0){
						perror("send error");
						exit(1);
					}
				} else if(strncmp(&inputbuf,"cd",2)==0){
					if (strlen(&inputbuf) > maxBuflen) {
						fprintf(stderr, "input max length %d",maxBuflen);
						continue;
					}
					if(sendn(socketFd, &inputbuf, strlen(&inputbuf), msgHeadLeng) < 0){
						perror("send error");
						exit(1);
					}
				}
			}
		}
    }

    return 0;
}