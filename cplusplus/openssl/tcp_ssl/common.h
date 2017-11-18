#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/socket.h>


#define MAXSIZE 1024 //每次最大数据传输量

void tcpserver_init(int *sockfd, int port);

void tcp_accept(int sockfd, int *new_fd);

struct sockaddr_in tcpclient_init(int *sockfd, char* ipaddr, int port);

void tcp_connect(int sockfd, struct sockaddr_in dest);

#endif