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

#define SERVER_PORT_A 11111


int SendData(int s, char *ip, int nPort, char *pData)  
{  
    sockaddr_in ser;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(nPort);
    ser.sin_addr.s_addr = inet_addr(ip);
    int nLen = sizeof(ser);
      
    return sendto(s, pData, strlen(pData) + 1, 0, (sockaddr*)&ser, nLen);
}

int main(int argc, char **argv)  
{
    int sClient;
    char send_buf[] = "SNO=06060734";
    int nSend;

    sClient = socket(AF_INET, SOCK_DGRAM, 0);  
    if (sClient < 0) {  
        printf("socket\n");  
        return 0;  
    }

    nSend = SendData(sClient, "127.0.0.1", SERVER_PORT_A, send_buf);
    if (nSend == 0) {  
        return 0;  
    } else if (nSend < 0) {  
        printf("sendto\n");  
        return 0;  
    }

    close(sClient);

    return 0;  
}


// g++ -o client client.cpp