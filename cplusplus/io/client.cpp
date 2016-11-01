#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>


#define PORT 59000

int CreateConnect(char *ipaddr)
{
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;

    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("Create Socket Failed!\n");
        return -1;
    }

    if(bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr))) {
        printf("Client Bind Port Failed!\n"); 
        return -1;
    }
    
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_aton(ipaddr, &server_addr.sin_addr) == 0) {
        printf("Server IP Address Error!\n");
        return -1;
    }

    server_addr.sin_port = htons(PORT);

    if(connect(sockfd, (struct sockaddr*)&server_addr, 
        sizeof(struct sockaddr_in)) < 0) {
        printf("Can Not Connect To %s!\n", ipaddr);
        return -1;
    }

    return sockfd;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: %s ServerIPAddress cmd\n", argv[0]);
        exit(1);
    }

    char buf[] = "Just for test!";

    int sockfd = CreateConnect(argv[1]);

    int ret = send(sockfd, buf, sizeof(buf), 0);
    if(ret < 0) {
        printf("send msg error!\n");
    }

    close(sockfd);

    return 0;
}


