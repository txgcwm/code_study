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



#define MAX_EVENTS 500
#define REVLEN 10

char recvBuf[REVLEN];

//将新建立的连接添加到EPOLL的监听中
int AddSock2EpollEvent(int epollfd, int addfd)
{
	struct epoll_event event;

	event.data.fd = addfd;
	event.events = EPOLLIN | EPOLLET;

	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, addfd, &event) < 0) {
		printf("epoll add fd(%d) fail!\n", addfd);
		return -1;
	}

	return 0;
}

void AcceptConn(int epollfd, int srvfd)
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

	AddSock2EpollEvent(epollfd, confd);

	return;
}

void RecvData(int fd)
{
	int ret;
	int recvLen = 0;

	memset(recvBuf, 0, REVLEN);

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
	int i, ret;
	int sockListen;
	int epollfd;
	int timeout = 3000;
	struct epoll_event eventList[MAX_EVENTS];

	sockListen = CreateServiceSock();

	epollfd = epoll_create(MAX_EVENTS);

	AddSock2EpollEvent(epollfd, sockListen);

	while (1) {
		int ret = epoll_wait(epollfd, eventList, MAX_EVENTS, timeout);
		if (ret < 0) {
			printf("epoll error\n");
			break;
		} else if (ret == 0) {
			printf("timeout...\n");
			continue;
		}

		int n = 0;

		for(n = 0; n < ret; n++) {
			if((eventList[n].events & EPOLLERR) ||
				(eventList[n].events & EPOLLHUP) ||
				!(eventList[n].events & EPOLLIN)) {
				printf("epoll error\n");
				close(eventList[n].data.fd);
				return -1;
			}

			if(eventList[n].data.fd == sockListen) {
				AcceptConn(epollfd, sockListen);
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


