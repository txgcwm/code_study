#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "common.h"



typedef struct _CLIENT {
	int fd;
	struct sockaddr_in addr;
} Client;
 
#define BACKLOG 5

int RecvData(int sockfd)
{
	int ret = -1;
	char buf[256];

	do {
		ret = recv(sockfd, buf, sizeof(buf), 0);
		if(ret == 0) {
			break;
		} else if (ret < 0) {
			break;
		}

		printf("recv data: %s\n", buf);
	} while(0);

	printf("ret: %d\n", ret);

	return ret;
}

int main(int argc, char **argv)
{
	int i, ret;
	int recvLen = 0;
	int currentClient = 0;
	fd_set readfds;
	int sockListen, sockSvr, sockMax;
	struct timeval timeout;
	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	Client client[BACKLOG];

	sockListen = CreateServiceSock();

	for (i = 0; i < BACKLOG; i++) {
		client[i].fd = -1;
	}

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(sockListen, &readfds);
		sockMax = sockListen;

		for (i = 0; i < BACKLOG; i++) {
			if (client[i].fd > 0) {
				FD_SET(client[i].fd, &readfds);
				if (sockMax < client[i].fd) {
					sockMax = client[i].fd;
				}
			}
		}

		timeout.tv_sec = 3;
		timeout.tv_usec = 0;

		ret = select((int)sockMax + 1, &readfds, NULL, NULL, &timeout);
		if(ret < 0) {
			printf("select error\n");
			break;
		} else if (ret == 0) {
			printf("timeout...\n");
			continue;
		}

		for(i = 0; i < BACKLOG; i++) {
			if(client[i].fd > 0 && FD_ISSET(client[i].fd, &readfds)) {
				if(RecvData(client[i].fd) <= 0) {
					client[i].fd = -1;
				}
			}
		}

		if (FD_ISSET(sockListen, &readfds)) {
			sockSvr = accept(sockListen, (struct sockaddr *)&client_addr, &addrlen);
			if(sockSvr == -1) {
				printf("accpet error\n");
			} else {
				currentClient++;
			}

			for(i = 0; i < BACKLOG; i++) {
				if(client[i].fd < 0) {
					client[i].fd = sockSvr;
					client[i].addr = client_addr;
					printf("Got a connection from %s \n", inet_ntoa(client[i].addr.sin_addr));
					break;
				}
			}
		}
	}

	close(sockListen);

	return 0;
}


