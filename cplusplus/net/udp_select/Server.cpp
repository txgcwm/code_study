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
#ifdef __linux__
    #include <sys/prctl.h>
#endif

#include "Server.h"



CServer::CServer()
: CThreadLoop("Server")
{
}

CServer::~CServer()
{
}

bool CServer::Start(int port)
{
    m_port = port;

    return StartThread();
}

bool CServer::Stop()
{
    return StopThread();
}

void CServer::EventHandleLoop()
{
    int sock = -1; 
    int nRet;
    char recv_buf[1024];
    fd_set rfd;
    timeval timeout;
    sockaddr_in addr;
    socklen_t nRecLen;
    sockaddr_in cli;

    sock = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sock <= 0) {  
        printf("socket\n");  
        return;  
    }  

    memset(&addr, 0, sizeof(addr));  
      
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {  
        printf("bind\n");  
        return;  
    }

#ifdef __linux__
    char name[20] = {0};

    prctl(PR_GET_NAME, name);
#endif

    while(1) {
        FD_ZERO(&rfd);
        FD_SET(sock, &rfd);

        timeout.tv_sec = 30;
        timeout.tv_usec = 0;
      
        memset(recv_buf, 0, sizeof(recv_buf));

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

        if(WaitForSleep(0) < 0) {
            break;
        }
    }

    close(sock);
    sock = -1;

    return;
}

