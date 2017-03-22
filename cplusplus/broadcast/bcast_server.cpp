#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define PORT 9999


int main(int argc, char*argv[])
{
    int ret = -1;
    int sock;
    struct sockaddr_in server_addr; //服务器端地址
    struct sockaddr_in from_addr; //客户端地址
    socklen_t from_len = sizeof(struct sockaddr_in);
    int count = -1;
    fd_set readfd; //读文件描述符集合
    char buffer[1024];
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0); //建立数据报套接字
    if (sock < 0) {
        perror("sock error");
        return -1;
    }

    memset((void*) &server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY );
    server_addr.sin_port = htons(PORT);

    ret = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)); //将地址结构绑定到套接字上
    if (ret < 0) {
        perror("bind error");
        return -1;
    }

    while (1) {
        timeout.tv_sec = 100;
        timeout.tv_usec = 0;

        FD_ZERO(&readfd);  //文件描述符集合清0 
        FD_SET(sock, &readfd);  //将套接字描述符加入到文件描述符集合

        ret = select(sock + 1, &readfd, NULL, NULL, &timeout); //select侦听是否有数据到来
        switch (ret) {
        case -1: //发生错误
            perror("select error:");
            break;

        case 0: //超时
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



