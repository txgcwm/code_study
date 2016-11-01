#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>

#include "common.h"



#define BACKLOG 5
#define REVLEN 10
#define OPEN_MAX 1024

int currentClient = 0;
char recvBuf[REVLEN];

int main(int argc, char **argv)
{
	int i, ret, sinSize;
	int recvLen = 0;
	fd_set readfds, writefds;
	int sockListen, sockSvr, sockMax;
	int timeout;
	struct sockaddr_in client_addr;
	struct pollfd clientfd[OPEN_MAX];
 
	sockListen = CreateServiceSock();

	clientfd[0].fd = sockListen;
	clientfd[0].events = POLLIN;	//POLLRDNORM;  
	sockMax = 0;
	for (i = 1; i < OPEN_MAX; i++) {
		clientfd[i].fd = -1;
	}

	while (1) {
		timeout = 3000;

		ret = poll(clientfd, sockMax + 1, timeout);
		if (ret < 0) {
			printf("select error\n");
			break;
		} else if (ret == 0) {
			printf("timeout ...\n");
			continue;
		}

		if (clientfd[0].revents & POLLIN) {	//POLLRDNORM  
		
			sockSvr = accept(sockListen, NULL, NULL);	//(struct sockaddr*)&client_addr  
			if (sockSvr == -1) {
				printf("accpet error\n");
			} else {
				currentClient++;
			}

			for (i = 0; i < OPEN_MAX; i++) {
				if (clientfd[i].fd < 0) {
					clientfd[i].fd = sockSvr;
					break;
				}
			}

			if (i == OPEN_MAX) {
				printf("too many connects\n");
				return -1;
			}

			clientfd[i].events = POLLIN;	//POLLRDNORM;  
			if (i > sockMax) {
				sockMax = i;
			}
		}

		for (i = 1; i <= sockMax; i++) {
			if (clientfd[i].fd < 0) {
				continue;
			}

			if (clientfd[i].revents & (POLLIN | POLLERR)) {	//POLLRDNORM  
				if (recvLen != REVLEN) {
					while (1) {
						ret = recv(clientfd[i].fd, (char *)recvBuf + recvLen,
								 REVLEN - recvLen, 0);
						if (ret == 0) {
							clientfd[i].fd = -1;
							recvLen = 0;
							break;
						} else if (ret < 0) {
							clientfd[i].fd = -1;
							recvLen = 0;
							break;
						}

						recvLen = recvLen + ret;
						if (recvLen < REVLEN) {
							continue;
						} else {
							printf("buf = %s\n", recvBuf);
							//close(client[i].fd);  
							//client[i].fd = -1;  
							recvLen = 0;
							break;
						}
					}
				}
			}
		}
	}

	return 0;
}


