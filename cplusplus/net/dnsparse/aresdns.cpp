#include <time.h>  
#include <iostream>  
#include <netdb.h>  
#include <arpa/inet.h>  
#include <ares.h>  
#include <sys/select.h>  
#include <string.h>  
  
#define IP_LEN 32  
  
typedef struct {  
    char host[64];  
    char ip[5][IP_LEN];   
    int count;     
}IpList;  
  
void dns_callback (void* arg, int status, int timeouts, struct hostent* hptr)  //ares  处理完成，返回DNS解析的信息  
{  
    IpList *ips = (IpList*)arg;  
    if(ips == NULL) {
        return;
    }

    if(status == ARES_SUCCESS) {  
        strncpy(ips->host, hptr->h_name, sizeof(ips->host));  
        char **pptr = hptr->h_addr_list;

        for(int i = 0; *pptr != NULL && i < 5; pptr++, ++i) {  
            inet_ntop(hptr->h_addrtype, *pptr, ips->ip[ips->count++], IP_LEN);  
        }   
    } else {  
        std::cout << "lookup failed: " << status << std::endl;  
    }  
}  
  
void dns_resolve(const char* host, IpList &ips)  
{  
    ares_channel channel;    // 创建一个ares_channel  
      
    int res;  
    if((res = ares_init(&channel)) != ARES_SUCCESS) {     // ares 对channel 进行初始化  
        std::cout << "ares feiled: " << res << std::endl;  
        return;  
    }

    //set dns server  
    ares_set_servers_csv(channel, "114.114.114.114");

    //get host by name  
    ares_gethostbyname(channel, host, AF_INET, dns_callback, (void*)(&ips));  
  
    int nfds;    
    fd_set readers, writers;    
    timeval tv, *tvp;

    while (true) {    
        FD_ZERO(&readers);    
        FD_ZERO(&writers);

        nfds = ares_fds(channel, &readers, &writers);   //获取ares channel使用的FD    
        if (nfds == 0) {
            break;    
        }

        tvp = ares_timeout(channel, NULL, &tv);        
        select(nfds, &readers, &writers, NULL, tvp);   //将ares的SOCKET FD 加入事件循环    
        ares_process(channel, &readers, &writers);  // 有事件发生 交由ares 处理  
    }

    ares_destroy(channel);

    return;
}  
  
int main(int argc, char **argv)  
{  
    int res;

    if(argc < 2 ) {
        std::cout << "usage: " << argv[0] << " ip.address"<< std::endl;
        return 1;
    }
  
    IpList ips;

    do {  
        memset(&ips, 0, sizeof(ips));

        dns_resolve(argv[1], ips);

        std::cout <<"hostname:"<< ips.host << std::endl;

        for(int i = 0; i < ips.count; ++i) {
            std::cout<< "address ip:" << ips.ip[i] << std::endl;
        }

        std::cout<< std::endl;
    } while(true);

    return 0;  
}  

 
// g++ aresdns.cpp -lcares -lrt -o test