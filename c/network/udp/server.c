#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>


#define PORT 		3000
#define BUFFER_SIZE 2*1024*1024

int main(int argc, char** argv)
{ 
    int s = -1;
	int addr_len = sizeof(struct sockaddr_in);
	struct sockaddr_in serv, client;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0) {
        perror("create socket error: ");
        exit(1);
    }

	int fd = open("test.yuv", O_WRONLY|O_CREAT, S_IRUSR | S_IWUSR);

    bzero(&serv, sizeof(serv));

    serv.sin_family = AF_INET;   
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    if(bind(s, (struct sockaddr*)&serv, sizeof(serv)) == -1) {
        perror("bind error: ");
        exit(1);
    }

	char* buff = (char*)malloc(BUFFER_SIZE);

    while(1) {
        int ret = recvfrom(s, buff, BUFFER_SIZE, 0, (struct sockaddr *)&client, &addr_len);
        if(ret == -1) {
            perror("recvfrom error: ");
            break;
        }

		write(fd, buff, ret);

		printf("%s\n", buff);
    }

	free(buff);
    close(s);
	close(fd);

    return 0;
}


// gcc -o server server.c

