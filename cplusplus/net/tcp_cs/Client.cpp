#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

 
#define PORT            9009 
#define BUFFER_SIZE     1024

 
int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./%s ServerIPAddress\n",argv[0]);
        exit(1);
    }

    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;

    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0) {
        printf("Create Socket Failed!\n");
        exit(1);
    }

    if( bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr))) {
        printf("Client Bind Port Failed!\n"); 
        exit(1);
    }
    
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_aton(argv[1], &server_addr.sin_addr) == 0) {
        printf("Server IP Address Error!\n");
        exit(1);
    }

    server_addr.sin_port = htons(PORT);

    if(connect(sockfd, (struct sockaddr*)&server_addr, 
        sizeof(struct sockaddr_in)) < 0) {
        printf("Can Not Connect To %s!\n",argv[1]);
        exit(1);
    }
     
    int length = 0;
    char buffer[BUFFER_SIZE];

    strcpy(buffer, "Test data from client to server!\n");

    send(sockfd, buffer, BUFFER_SIZE, 0);

    length = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if(length < 0) {
        printf("Recieve Data From Server Failed!\n");
    }

    printf("%s\n", buffer);

    close(sockfd);

    return 0;
}


