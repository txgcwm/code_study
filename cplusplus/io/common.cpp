#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>



#define PORT 59000

int CreateServiceSock()
{
	int sockfd = -1;
	struct sockaddr_in server_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error\n");
		return -1;
	}

	int opt = 1;

	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	// Avoid TIME_WAIT
	if(ret < 0) {
		printf("Error while setting flag SO_REUSEADDR\n");
	}

	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&server_addr, addrlen) < 0) {
		printf("bind error\n");
		close(sockfd);
		return -1;
	}

	if(listen(sockfd, 5) < 0) {
		printf("listen error\n");
		close(sockfd);
		return -1;
	}

	printf("create socket(%d) success, start to listen...\n", sockfd);

	return sockfd;
}

int RecvData(int sockfd)
{
	int ret = -1;
	char buf[256] = {0};

	do {
		ret = recv(sockfd, buf, sizeof(buf), 0);
		if(ret == 0) {
			break;
		} else if (ret < 0) {
			break;
		}

		printf("recv data: %s\n", buf);
	} while (0);

	printf("ret: %d\n", ret);

	return ret;
}


