#include <stdio.h>   
#include <string.h>   
#include <unistd.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <sys/time.h>   
#include <stdlib.h>
#include <errno.h>
  
#define PORT            9009
#define BUFFER_SIZE     1024



int main(int argc ,char **argv)
{
    int listen_fd, accept_fd;
    struct sockaddr_in client_addr;
    int n = 1;

    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket Error:%s\n\a", strerror(errno));
        exit(1);
    }

    bzero(&client_addr, sizeof(struct sockaddr_in));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int));

    if(bind(listen_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        printf("Bind Error:%s\n\a",strerror(errno));
        exit(1);
    }

    listen(listen_fd, 5);

    while(1) {
        accept_fd = accept(listen_fd, NULL, NULL);
        if((accept_fd < 0) && (errno == EINTR)) {
            continue;
        } else if(accept_fd < 0) {
            printf("Accept Error:%s\n\a", strerror(errno));
            continue;
        }
        
        int length = 0;
        char buffer[BUFFER_SIZE];

        strcpy(buffer, "Test data from server to client!\n");

        send(accept_fd, buffer, BUFFER_SIZE, 0);

        length = recv(accept_fd, buffer, BUFFER_SIZE, 0);
        if(length < 0) {
            printf("Recieve Data From Server Failed!\n");
            break;
        }

        printf("%s\n", buffer);

        close(accept_fd);
    }

    close(listen_fd);

    return 0;
} 


