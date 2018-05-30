#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Log.h"



namespace Rtsp {

int CreateTcpConnect(const char *host, int port)
{
	int reuse = 1, fd = 0;
    struct sockaddr_in server;

    if(host == NULL) {
	    return -1;
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0){
    	ERROR("socket error!\n");
    	return -1;
	}

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	memset(&server, 0, sizeof(struct sockaddr_in));

  	server.sin_family = AF_INET;
  	server.sin_addr.s_addr = inet_addr(host);
  	server.sin_port = htons(port);

  	bind(fd, (struct sockaddr *)&server, sizeof(struct sockaddr));

  	listen(fd, 5);

  	return fd;
}

int CreateUdpConnect(const char *host, int port, int cliport)
{
	int fd, reuse = 1;
	struct sockaddr_in server, rtp_address;
	int result = -1;
	int len = sizeof(struct sockaddr_in);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd < 0){
    	ERROR("socket error!\n");
    	return -1;
	}
 
	/*set address reuse*/	  
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)); 
 
	/*bind local port*/		 
	server.sin_family = AF_INET; 		 
	server.sin_addr.s_addr = htonl(INADDR_ANY);				 
	server.sin_port = htons(port);

	if ((bind(fd, (struct sockaddr *)&server, len)) < 0) {				 
		printf("bind rtsp server port error"); 
		return -1;
	}

	/*  Name the socket, as agreed with the server.  */
	rtp_address.sin_family = AF_INET;
	rtp_address.sin_addr.s_addr = inet_addr(host);
	rtp_address.sin_port = htons(cliport);

	/*  Now connect our socket to the server's socket.  */
	result = connect(fd, (struct sockaddr *)&rtp_address, len);
 
	if(result == -1) {
	 	printf("connect vrtp socket error\n");
		return -1;
	}

	INFO("host: %s, server port: %d, client port: %d\n", host, port, cliport);

	return fd;
}


} // end namespace


