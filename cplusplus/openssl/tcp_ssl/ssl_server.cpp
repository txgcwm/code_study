#include <stdio.h>  
#include <stdlib.h>  
#include <signal.h>  
#include <errno.h>  
#include <string.h>  
#include <netdb.h>  
#include <resolv.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <arpa/inet.h>  
  
#include <openssl/bio.h>  
#include <openssl/err.h>  
#include <openssl/ssl.h>

#include "common.h"


// printf("1.:SSL Protocol Channel\n");  
// printf("2.:TCP Protocol Channel\n");

int main(int argc, char** argv)  
{  
    int sockfd = -1, client_fd = -1;  
    socklen_t len;
    SSL_CTX *ctx = NULL; 
    char serverbuf[MAXSIZE];
    int res = 0;
    int mode = 0, port = -1;
    char cert[128] = {0};
    char key[128] = {0};

    while((res = getopt(argc, argv, "?m:p:c:k:h")) != -1) {
        switch(res) {
        case 'm':
            mode = atoi(optarg);
            break;

        case 'p':
            port = atoi(optarg);
            break;

        case 'c':
            memcpy(cert, optarg, strlen(optarg));
            break;

        case 'k':
            memcpy(key, optarg, strlen(optarg));
            break;

        case 'h':
        default:
            return -1;
        }
    }
  
    ERR_load_BIO_strings();  
    SSL_library_init();  
    OpenSSL_add_all_algorithms();  
    SSL_load_error_strings();

    ctx = SSL_CTX_new(SSLv23_server_method());  
    if(ctx == NULL) {  
        ERR_print_errors_fp(stdout);  
        exit(1);  
    }

    if(!SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM)) {  
        ERR_print_errors_fp(stdout);  
        exit(1);  
    }

    if(!SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM)) {  
        ERR_print_errors_fp(stdout);  
        exit(1);  
    }

    if(!SSL_CTX_check_private_key(ctx)) {  
        ERR_print_errors_fp(stdout);  
        exit(1);  
    }
 
    signal(SIGPIPE, SIG_IGN);  
    tcpserver_init(&sockfd, port);

    while(1) {
        tcp_accept(sockfd, &client_fd);
        bzero(serverbuf, MAXSIZE);

        if(mode == 1) {  
            SSL *ssl = SSL_new(ctx);  
            SSL_set_fd(ssl, client_fd);

            int ret = SSL_accept(ssl);
            if(ret == -1) {
                perror("accept");  
                close(client_fd);  
                break;  
            }  
             
            len = SSL_read(ssl, serverbuf, MAXSIZE);
             
            SSL_shutdown(ssl);  
            SSL_free(ssl); 
        } else if(mode == 2) { 
            len = recv(client_fd, serverbuf, MAXSIZE, 0);        
        }

        if(client_fd > 0) {
            if (len > 0) {
                printf("接收消息成功:'%s'，共%d个字节的数据\n", serverbuf, len);  
            } else {
                printf("消息接收失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
            }

            close(client_fd);
            client_fd = -1;
        }
    }

    close(sockfd);  
    SSL_CTX_free(ctx);

    return 0;  
}

