#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>



#define MCAST_PORT 		5353
#define MCAST_ADDR 		"224.0.0.251"
#define MCAST_INTERVAL 	5
#define BUFF_SIZE 		512


#if 1
int main(int argc, char*argv[])
{
    int s;
    struct sockaddr_in local_addr;
    int err = -1;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        perror("socket()");
        return -1;
    }  

    // memset(&local_addr, 0, sizeof(local_addr));
    // local_addr.sin_family = AF_INET;
    // local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // local_addr.sin_port = htons(MCAST_PORT);

    // err = bind(s, (struct sockaddr*)&local_addr, sizeof(local_addr));
    // if(err < 0) {
    //     perror("bind()");
    //     return -2;
    // }

    int loop = 1;
    err = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if(err < 0) {
        perror("setsockopt():IP_MULTICAST_LOOP");
        return -3;
    }

    struct ip_mreq mreq;

    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    err = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (err < 0) {
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        return -4;
    }

    int times = 0;
    int addr_len = 0;
    char buff[BUFF_SIZE];
    int n = 0;

    for(times = 0;times < 5; times++) {
        addr_len = sizeof(local_addr);

        memset(buff, 0, BUFF_SIZE);

        n = recvfrom(s, buff, BUFF_SIZE, 0,(struct sockaddr*)&local_addr, (socklen_t*)&addr_len);
        if( n== -1) {
            perror("recvfrom()");
        }

        printf("Recv %dst message from server:%s\n", times, buff);

        sleep(MCAST_INTERVAL);
    }

    err = setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));

    close(s);

    return 0;
}
#else
 
int create_udp_socket(char *ip, int port)
{  
    int onOff = 1;
    int sockfd;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;  
    addr.sin_addr.s_addr = inet_addr(ip);  
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    addr.sin_port = htons(port);
      
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {  
        return -1;  
    }  
  
    fcntl(sockfd, F_SETFD, FD_CLOEXEC);  
  
    setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, (const void *)&onOff, sizeof(onOff));

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {  
        close(sockfd);  
        return -1;  
    }

    return sockfd;  
}  
  
void listen_udp(int sock)
{  
    socklen_t sin_len =sizeof(struct sockaddr_in);  
    char msg_buffer[BUF_SIZE];  
    struct sockaddr_in sin;
    fd_set rfds;  
    struct timeval tv;  
    int retval;  
    int rlen = 0;  
  
    while(1) {  
        FD_ZERO(&rfds);  
        FD_SET(sock, &rfds);  
        tv.tv_sec = 1;  
        tv.tv_usec = 0;  
        retval = select(sock + 1, &rfds, NULL, NULL, &tv);  
        if (retval == -1){  
            printf("select error %d %s\n", sock, strerror(errno));  
            break;
        } else if (retval) {  
            memset(msg_buffer, 0, BUF_SIZE);  
            if(FD_ISSET(sock, &rfds)){  
            	rlen = recvfrom(sock, msg_buffer, BUF_SIZE, 0, (struct sockaddr *)&sin, &sin_len);  
                if(rlen > 0){  
                    printf("recv %s \n", msg_buffer);  
                } else {  
                    printf("rlen : %d\n", rlen);  
                }  
            }  
        }  
    }

    close(sock);
    sock = -1;
}  

int main(int argc, char **argv)
{  
    int socket;  
    socket = create_udp_socket("224.0.0.251", 5353);  
    listen_udp(socket);

    return 0; 
}   
#endif



