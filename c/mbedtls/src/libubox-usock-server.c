#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "libubox/usock.h"



int main(int argc, char** argv)
{
  struct sockaddr_in cli_addr;
  socklen_t len = sizeof(struct sockaddr);
  int type = USOCK_TCP | USOCK_SERVER | USOCK_NOCLOEXEC | USOCK_IPV4ONLY;
  const char *host = "127.0.0.1";
  const char *service = "8000";
  char recv_buf[1024] = {0};
  int connect_fd, u_fd;

  u_fd = usock(type, host, service);    
  if(u_fd < 0) {
    perror("usock");
    return -1;
  }

  while (1) {
    connect_fd = accept(u_fd, (struct sockaddr *)(&cli_addr), &len);
    if (connect_fd < 0) {
      perror("accept");
      return -1;
    }

    printf("client_addr: %s\n", inet_ntoa(cli_addr.sin_addr));
    recv(connect_fd, recv_buf, 1024, 0);
    printf("recv %s\n", recv_buf);
    close(connect_fd);
  }

  return 0;
}