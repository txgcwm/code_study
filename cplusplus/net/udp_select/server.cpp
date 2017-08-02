#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/in.h>
#include <net/if.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>


#define PORT_A  11111  

int main(int argc, char **argv)
{  
    int sock = -1; 
    int nPortA = PORT_A;
    fd_set rfd;
    timeval timeout;
    sockaddr_in addr;
    char recv_buf[1024];
    socklen_t nRecLen;
    sockaddr_in cli;
    int nRet;

    sock = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock <= 0) {  
        printf("socket\n");  
        return -1;  
    }  

    memset(&addr, 0, sizeof(addr));  
      
    addr.sin_family = AF_INET;
    addr.sin_port = htons(nPortA);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {  
        printf("bind\n");  
        return -1;  
    }  

    timeout.tv_sec = 6;   
    timeout.tv_usec = 0;  
      
    memset(recv_buf, 0, sizeof(recv_buf));

    while(true) {  
        FD_ZERO(&rfd);
        FD_SET(sock, &rfd);
          
        nRet = select(sock + 1, &rfd, NULL, NULL, &timeout);
        if (nRet < 0) {
            printf("select\n");
            continue;
        } else if (nRet == 0) {
            printf("timeout\n");
            continue;
        }

        if (FD_ISSET(sock, &rfd)) {
            nRecLen = sizeof(cli);

            ssize_t nRecEcho = recvfrom(sock, recv_buf, sizeof(recv_buf), 0,
                                        (struct sockaddr*)&cli, &nRecLen);
            if (nRecEcho <= 0) {
                printf("recvfrom\n");
            } else {
                printf("%s\n", recv_buf);
            }
        }
    }

    close(sock);
    sock = -1;

	return 0;
}  


// g++ -o server server.cpp