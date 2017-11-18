#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/socket.h>


void tcpserver_init(int *sockfd);

void tcp_accept(int sockfd, int *new_fd);

struct sockaddr_in tcpclient_init(int *sockfd);

void tcp_connect(int sockfd, struct sockaddr_in dest);

#endif