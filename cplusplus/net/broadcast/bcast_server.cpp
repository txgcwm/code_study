#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define IP_FOUND 		"IP_FOUND"
#define IP_FOUND_ACK 	"IP_FOUND_ACK"
#define PORT 			9999


int main(int argc, char*argv[])
{
    int ret = -1;
    int sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(struct sockaddr_in);
    int count = -1;
    fd_set readfd;
    char buffer[1024];
    struct timeval timeout;
    //timeout.tv_sec = 2;
    //timeout.tv_usec = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("sock error");
        return -1;
    }

    memset((void*) &server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY );
    server_addr.sin_port = htons(PORT);

    ret = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error");
        return -1;
    }

    while (1) {
        timeout.tv_sec = 100;
        timeout.tv_usec = 0;

        FD_ZERO(&readfd);
        FD_SET(sock, &readfd);

        ret = select(sock + 1, &readfd, NULL, NULL, &timeout);
        switch (ret) {
        case -1:
            perror("select error:");
            break;

        case 0:
            printf("select timeout\n");
            break;

        default:
            if (FD_ISSET(sock,&readfd)) {
                count = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&from_addr, (socklen_t *)&from_len);
                if (strstr(buffer, IP_FOUND)) {
                    printf("\nClient connection information:\n\t IP: %s, Port: %d\n",
                            (char *)inet_ntoa(from_addr.sin_addr),
                            ntohs(from_addr.sin_port));

                    memcpy(buffer, IP_FOUND_ACK, strlen(IP_FOUND_ACK) + 1);
                    count = sendto(sock, buffer, strlen(buffer), 0,
                            (struct sockaddr*) &from_addr, from_len);
                }
            }

            break;
        }
    }

    return 0;
}



