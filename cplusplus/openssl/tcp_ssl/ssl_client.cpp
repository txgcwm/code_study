#include <stdio.h>
#include <stdlib.h>
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



void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if(cert != NULL) {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
    } else {
        printf("无证书信息！\n");
    }

    return;
}

int main(int argc, char** argv)
{
    int sockfd, len;
    char clientbuf[MAXSIZE];
    struct sockaddr_in serv_addr;
    SSL_CTX *ctx;
    SSL *ssl;
    int res = -1, mode = 0, port = -1;
    char hostname[32] = {0};

    while((res = getopt(argc, argv, "?m:p:s:h")) != -1) {
        switch(res) {
        case 'm':
            mode = atoi(optarg);
            break;

        case 'p':
            port = atoi(optarg);
            break;

        case 's':
            memcpy(hostname, optarg, strlen(optarg));
            break;

        case 'h':
        default:
            return -1;
        }
    }

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    serv_addr = tcpclient_init(&sockfd, hostname, port);

    tcp_connect(sockfd, serv_addr);

    bzero(clientbuf, MAXSIZE);
    strcpy(clientbuf, "id:am3517&pw:am3517");

    if(mode == 1) {
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, sockfd);
       
        if (SSL_connect(ssl) == -1) {
            ERR_print_errors_fp(stderr);
        } else {
            printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
            ShowCerts(ssl);
        }

        len = SSL_write(ssl, clientbuf, strlen(clientbuf));

        SSL_shutdown(ssl);
        SSL_free(ssl);
    } else if(mode == 2) {
        len = send(sockfd, clientbuf, strlen(clientbuf),0);
    }

    if (len < 0) {
        printf("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n",
                clientbuf, errno, strerror(errno));
    } else {
        printf("消息'%s'发送成功，共发送了%d个字节！\n",clientbuf, len);
    }

    close(sockfd);
    SSL_CTX_free(ctx);

    return 0;
}

