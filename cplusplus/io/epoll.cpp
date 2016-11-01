#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

#include "common.h"

//最多处理的connect  
#define MAX_EVENTS 500

//当前的连接数  
int currentClient = 0;

#define REVLEN 10
char recvBuf[REVLEN];

int epollfd;
struct epoll_event eventList[MAX_EVENTS];

void AcceptConn(int srvfd)
{
	struct sockaddr_in sin;
	socklen_t len = sizeof(struct sockaddr_in);

	bzero(&sin, len);

	int confd = accept(srvfd, (struct sockaddr *)&sin, &len);
	if (confd < 0) {
		printf("bad accept\n");
		return;
	} else {
		printf("Accept Connection: %d", confd);
	}

	//setnonblocking(confd);  

	//4. epoll_wait  
	//将新建立的连接添加到EPOLL的监听中  
	struct epoll_event event;
	event.data.fd = confd;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, confd, &event);

	return;
}

void RecvData(int fd)
{
	int ret;
	int recvLen = 0;

	memset(recvBuf, 0, REVLEN);
	printf("RecvData function\n");

	if (recvLen != REVLEN) {
		while (1) {
			ret = recv(fd, (char *)recvBuf + recvLen, REVLEN - recvLen, 0);
			if(ret == 0) {
				recvLen = 0;
				break;
			} else if (ret < 0) {
				recvLen = 0;
				break;
			}

			recvLen = recvLen + ret;

			if (recvLen < REVLEN) {
				continue;
			} else {
				printf("buf = %s\n", recvBuf);
				recvLen = 0;
				break;
			}
		}
	}

	printf("content is %s", recvBuf);

	return;
}

int main(int argc, char **argv)
{
	int i, ret, sinSize;
	int recvLen = 0;
	fd_set readfds, writefds;
	int sockListen, sockSvr, sockMax;
	int timeout;
	struct sockaddr_in client_addr;

	sockListen = CreateServiceSock();

	epollfd = epoll_create(MAX_EVENTS);
	struct epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = sockListen;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockListen, &event) < 0) {
		printf("epoll add fail : fd = %d\n", sockListen);
		return -1;
	}

	while (1) {
		timeout = 3000;

		int ret = epoll_wait(epollfd, eventList, MAX_EVENTS, timeout);
		if (ret < 0) {
			printf("epoll error\n");
			break;
		} else if (ret == 0) {
			printf("timeout ...\n");
			continue;
		}

		int n = 0;
		for (n = 0; n < ret; n++) {
			if ((eventList[n].events & EPOLLERR) ||
				(eventList[n].events & EPOLLHUP) ||
				!(eventList[n].events & EPOLLIN)) {
				printf("epoll error\n");
				close(eventList[n].data.fd);
				return -1;
			}

			if (eventList[n].data.fd == sockListen) {
				AcceptConn(sockListen);
			} else {
				RecvData(eventList[n].data.fd);
				//不删除  
				//   epoll_ctl(epollfd, EPOLL_CTL_DEL, pEvent->data.fd, pEvent);  
			}
		}
	}

	close(epollfd);
	close(sockListen);

	return 0;
}


