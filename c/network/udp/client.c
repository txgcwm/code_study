#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


#define PORT 3000

int main(int argc, char** argv)
{
	struct sockaddr_in serv;
	int s = -1;

	s = socket(AF_INET,SOCK_DGRAM,0);
	if(s==-1) {
		perror("create socket error: ");
		exit(1);
	}
	
	bzero(&serv,sizeof(serv));

	serv.sin_family = AF_INET;   
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");    //htonl(INADDR_ANY); 
	serv.sin_port = htons(PORT);

	while(1) {
		sendto(s, "test", strlen("test"), 0, (struct sockaddr *)&serv, sizeof(serv));
             
        sleep(3);     
	}

	close(s);
	s = -1;

	return 0;
}

// gcc -o client client.c

