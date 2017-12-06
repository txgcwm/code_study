#include <stdio.h>  
#include <stdlib.h>  
#include <strings.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <string>  
#include <iostream>

#include "people.pb.h"  
  
#define PORT 8888  
#define MAXDATASIZE 20  
#define BACKLOG 10  
  
using namespace std;  
  
int main(int argc, char **argv)  
{  
  int listenfd, connectfd, numbytes;  
  char buf[MAXDATASIZE];  
  struct sockaddr_in server;  
  struct sockaddr_in client;  
  socklen_t sin_size = sizeof(struct sockaddr_in);  
  
  listenfd = socket(AF_INET, SOCK_STREAM, 0);  
  
  int opt = SO_REUSEADDR;  
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  
  
  bzero(&server, sizeof(server));  
  server.sin_family = AF_INET;  
  server.sin_port = htons(PORT);  
  server.sin_addr.s_addr = htonl(INADDR_ANY);  
  
  bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr));  
  
  listen(listenfd,BACKLOG);  
  
  while(1){
    connectfd = accept(listenfd, (struct sockaddr *)&client, &sin_size);  
  
    numbytes = recv(connectfd, buf, MAXDATASIZE, 0);  
    buf[numbytes] = '\0';  
    string a = buf;  
    cout << "You got a message from " << inet_ntoa(client.sin_addr) << endl;  
    cout << "Client Message: " << a << endl;  

    if(a == "GET PEOPLE") {  
      string data;  
      People p;

      p.set_name("Hello");  
      p.set_id(123);  
      p.set_email("test@gmail.com");  
      p.SerializeToString(&data);

      char bts[data.length()];  
      strcpy(bts, data.c_str());  
      send(connectfd, bts, sizeof(bts), 0);  
    } else {  
      send(connectfd, "Fucking client!\n", 16, 0);  
    }

    close(connectfd);  
  }  
  
  close(listenfd);  

  return 0;  
}  



// g++ server.cpp people.pb.cc -o server -L/usr/local/lib -lprotobuf