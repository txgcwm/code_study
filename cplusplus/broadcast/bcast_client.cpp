#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define IFNAME "eth0"
#define MCAST_PORT 9999

int main(int argc, char*argv[])
{
    int ret = -1;
    int sock = -1;
    int j = -1;
    int so_broadcast = 1;
    struct ifreq *ifr;
    struct ifconf ifc;
    struct sockaddr_in broadcast_addr; //广播地址
    struct sockaddr_in from_addr; //服务端地址
    socklen_t from_len = sizeof(from_addr);
    int count = -1;
    fd_set readfd; //读文件描述符集合
    char buffer[1024];
    struct timeval timeout;

    // timeout.tv_sec = 2; //超时时间为2秒
    // timeout.tv_usec = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("create socket failed:");
        return -1;
    }

    ifc.ifc_len = sizeof(buffer);
    ifc.ifc_buf = buffer;
    if (ioctl(sock, SIOCGIFCONF, (char *) &ifc) < 0) {
        perror("ioctl-conf:");
        return -1;
    }
    ifr = ifc.ifc_req;

#if 0
    for (j = ifc.ifc_len / sizeof(struct ifreq); --j >= 0; ifr++) {
        if(!strcmp(ifr->ifr_name, "eth0")) {
            if (ioctl(sock, SIOCGIFFLAGS, (char *)ifr) < 0) {
                perror("ioctl-get flag failed:");
            }
            break;
        }

		if(!strcmp(ifr->ifr_name, "wlan0")) {
            if (ioctl(sock, SIOCGIFFLAGS, (char *)ifr) < 0) {
                perror("ioctl-get flag failed:");
            }
            break;
        }
    }
#endif

    //将使用的网络接口名字复制到ifr.ifr_name中，由于不同的网卡接口的广播地址是不一样的，因此指定网卡接口
    //strncpy(ifr.ifr_name, IFNAME, strlen(IFNAME));
    //发送命令，获得网络接口的广播地址
    if (ioctl(sock, SIOCGIFBRDADDR, ifr) == -1) {
        perror("ioctl error");
        return -1;
    }
    //将获得的广播地址复制到broadcast_addr
    memcpy(&broadcast_addr, (char *)&ifr->ifr_broadaddr, sizeof(struct sockaddr_in));
    //设置广播端口号
    printf("\nBroadcast-IP: %s\n", inet_ntoa(broadcast_addr.sin_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(MCAST_PORT);

    //默认的套接字描述符sock是不支持广播，必须设置套接字描述符以支持广播
    ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast,
            sizeof(so_broadcast));

    //发送多次广播，看网络上是否有服务器存在
    int times = 10;
    int i = 0;

    for (i = 0; i < times; i++) {
        timeout.tv_sec = 2;  //超时时间为2秒
        timeout.tv_usec = 0;
        ret = sendto(sock, IP_FOUND, strlen(IP_FOUND), 0,
                (struct sockaddr*) &broadcast_addr, sizeof(broadcast_addr));
        if (ret < 0) {
            continue;
        }
   
        FD_ZERO(&readfd);  //文件描述符清0
        FD_SET(sock, &readfd);  //将套接字文件描述符加入到文件描述符集合中
        
        ret = select(sock + 1, &readfd, NULL, NULL, &timeout); //select侦听是否有数据到来
        switch (ret) {
        case -1:
            break;

        case 0:
            perror("select timeout\n");
            break;

        default:
            if (FD_ISSET(sock,&readfd)) {
                count = recvfrom(sock, buffer, 1024, 0,
                        (struct sockaddr*) &from_addr, (socklen_t *)&from_len); //from_addr为服务器端地址
                printf("\trecvmsg is %s\n", buffer);
                if (strstr(buffer, IP_FOUND_ACK)) {
                    printf("\tfound server IP is %s, Port is %d\n",
                            inet_ntoa(from_addr.sin_addr),
                            htons(from_addr.sin_port));
                }
                return -1;

            }

            break;
        }
    }

    return 0;
}



