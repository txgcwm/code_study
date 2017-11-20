#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "libubox/usock.h"



int main(int argc, char** argv)
{
  struct sockaddr cli_addr;
  socklen_t len = sizeof(struct sockaddr);
  int type = USOCK_TCP | USOCK_NOCLOEXEC | USOCK_IPV4ONLY;
  const char *host = "127.0.0.1";
  const char *service = "8000";
  char recv_buf[1024] = {0};

  // printf("port: %s\n", usock_port(8888));

  int c_fd = usock(type, host, service);
  if(c_fd < 0) {
    perror("usock");
    return -1;
  }

  send(c_fd, "helloworld", 10, 0);
  sleep(2);
  close(c_fd);

  return 0;
}