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
#include <stdio.h>  
#include <stdlib.h>  

#include "Client.h"



CClient *CClient::Instance()
{
    static CClient client;
    return &client;
}

CClient::CClient()
{
}
    
CClient::~CClient()
{
}

bool CClient::Start(std::string ip, int port)
{
    m_sock = socket(AF_INET, SOCK_DGRAM, 0);  
    if (m_sock < 0) {  
        printf("socket\n");  
        return false;  
    }

    m_ip = ip;
    m_port = port;

    return true;
}

bool CClient::Stop()
{
    close(m_sock);
    m_sock = -1;

    return true;
}

int CClient::SendData(char *pData, int len)
{
    if(m_sock < 0) {
        return -1;
    }

    sockaddr_in ser;

    ser.sin_family = AF_INET;
    ser.sin_port = htons(m_port);
    ser.sin_addr.s_addr = inet_addr(m_ip.c_str());
    int nLen = sizeof(ser);
      
    return sendto(m_sock, pData, len, 0, (sockaddr*)&ser, nLen);
}

