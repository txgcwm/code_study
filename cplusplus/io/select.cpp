#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common.h"



typedef struct _CLIENT {
	int fd;
	struct sockaddr_in addr;	/* client's address information */
} Client;
 
#define BACKLOG 5
Client client[BACKLOG];
int currentClient = 0;
#define REVLEN 10
char recvBuf[REVLEN];

void showClient()
{
	int i;
	printf("client count = %d\n", currentClient);

	for (i = 0; i < BACKLOG; i++) {
		printf("[%d] = %d", i, client[i].fd);
	}

	printf("\n");
}

int main(int argc, char **argv)
{
	int i, ret, sinSize;
	int recvLen = 0;
	fd_set readfds, writefds;
	int sockListen, sockSvr, sockMax;
	struct timeval timeout;
	struct sockaddr_in client_addr;

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
				if (sockMax < client[i].fd)
					sockMax = client[i].fd;
			}
		}

		timeout.tv_sec = 3;
		timeout.tv_usec = 0;

		ret = select((int)sockMax + 1, &readfds, NULL, NULL, &timeout);
		if (ret < 0) {
			printf("select error\n");
			break;
		} else if (ret == 0) {
			printf("timeout ...\n");
			continue;
		}

		for (i = 0; i < BACKLOG; i++) {
			if (client[i].fd > 0 && FD_ISSET(client[i].fd, &readfds)) {
				if (recvLen != REVLEN) {
					while (1) {
						ret = recv(client[i].fd, (char *)recvBuf + recvLen,
								 REVLEN - recvLen, 0);
						if (ret == 0) {
							client[i].fd = -1;
							recvLen = 0;
							break;
						} else if (ret < 0) {
							client[i].fd = -1;
							recvLen = 0;
							break;
						}

						recvLen = recvLen + ret;
						if (recvLen < REVLEN) {
							continue;
						} else {
							printf("%s, buf = %s\n", inet_ntoa(client[i].addr.sin_addr), recvBuf);
							//close(client[i].fd);  
							//client[i].fd = -1;  
							recvLen = 0;
							break;
						}
					}
				}
			}
		}

		if (FD_ISSET(sockListen, &readfds)) {
			printf("isset\n");
			sockSvr = accept(sockListen, NULL, NULL);	//(struct sockaddr*)&client_addr  
			if (sockSvr == -1) {
				printf("accpet error\n");
			} else {
				currentClient++;
			}

			for (i = 0; i < BACKLOG; i++) {
				if (client[i].fd < 0) {
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


