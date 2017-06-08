#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
  
#include "RegMessage.pb.h"  



void* connHandler(void *pConn)
{  
    int* _pConn = static_cast<int*> (pConn);

    for (;;) {  
        void* buf = malloc(1024);  
        int n = read(*_pConn, buf, 1024);
        if(n <=0 ) {
            break;
        } 
          
        cn::vicky::model::seri::RegMessage regMessage;  
        regMessage.ParseFromArray(buf, n);  
          
        std::cout << regMessage.id() << std::endl;  
        std::cout << regMessage.username() << std::endl;  
        std::cout << regMessage.password() << std::endl;  
        std::cout << regMessage.email() << std::endl;  
          
        close(*_pConn);  
        free(buf);  
    }

    return pConn;  
}  
 
int main(void)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);  
    if (sock == -1) {  
        perror("创建Socket失败!\n");  
        return EXIT_FAILURE;  
    }  
  
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7070);  
    inet_pton(AF_INET, "192.168.0.110", &addr.sin_addr);  
  
    int result;  
    result = bind(sock, (struct sockaddr*) &addr, sizeof (struct sockaddr_in));  
    if (result != 0) {  
        perror("绑定地址失败！\n");  
        return EXIT_FAILURE;  
    }  
  
    result = listen(sock, 10);  
    if (result != 0) {  
        perror("监听失败！\n");  
        return EXIT_FAILURE;  
    }  
  
    for (;;) {  
        int conn = accept(sock, NULL, NULL);  
        if (conn == -1) {  
            perror("与客户端建立连接失败!\n");  
            continue;  
        }  
        pthread_t thread;  
        pthread_create(&thread, NULL, connHandler, (void*) &conn);  
    }  
  
    return 0;  
}

// g++ -o server *.cpp *.cc -lpthread -lprotobuf