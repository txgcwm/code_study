#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <string>

#include "Message.h"


int GetRouteInfo(int port, std::string &ssid, std::string& password, int& mode)
{
    int ret = -1;
    int sock = -1;
    struct sockaddr_in server_addr;
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(struct sockaddr_in);
    int count = -1;
    fd_set readfd;
    char buffer[1024] = {0};
    struct timeval timeout;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("sock error");
        return -1;
    }

    memset((void*) &server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY );
    server_addr.sin_port = htons(port);

    ret = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error");
        return -1;
    }

    while (1) {
        timeout.tv_sec = 20;
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
            if (FD_ISSET(sock, &readfd)) {
                count = recvfrom(sock, buffer, 1024, 0, (struct sockaddr*)&from_addr, (socklen_t *)&from_len);
                if(count > 0) {
                    printf("IP: %s, Port: %d\n",
                        (char *)inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port));

                    ParseBroadcastData(buffer, ssid, password, mode);

                    std::string data;
                    FormateResultData(data);

                    count = sendto(sock, data.c_str(), data.size(), 0,
                                (struct sockaddr*) &from_addr, from_len);

                    return 0;
                }     
            }

            break;
        }
    }

    return -1;
}

