#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>



int server(int client_socket)
{
	int length = 0;
	char *text = NULL;

	if(read(client_socket, &length, sizeof(length)) == 0) {	// 获取消息长度，保存到length
		return 0;
	}

	text = (char*)malloc(length);		// 分配用于保存信息的缓冲区
	read(client_socket, text, length);
	printf("(%d)(%s)\n", length, text);

	if(!strcasecmp(text, "quit")) {		// 忽略大小写比较字符串
		free(text);						// 释放缓冲区
		return 1;
	}

	free(text);

	return 0;
}

int main(int argc, char *argv[])
{
	const char* const socket_name = argv[1];
	int sock_fd;
	struct sockaddr_un name;
	int client_sent_quit_message = 0;
	fd_set fds;  
    struct timeval timeout = {3, 0}; //select等待3秒，3秒轮询，要非阻塞就置0

	if((sock_fd = socket(PF_LOCAL, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 1;
	}

	name.sun_family = AF_LOCAL;
	strcpy(name.sun_path, socket_name);
	if(bind(sock_fd, (const struct sockaddr*)&name, SUN_LEN(&name)) == -1) {
		perror("bind");
		return 1;
	}

	if(listen(sock_fd, 5) == -1) {
		perror("listen");
		return 1;
	}

	while(!client_sent_quit_message) {
		FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化  
        FD_SET(sock_fd, &fds); //添加描述符  

        timeout.tv_sec = 3;
		timeout.tv_usec = 0;

        int ret = select(sock_fd + 1, &fds, &fds, NULL, &timeout);
        if (ret < 0) {
			printf("select error\n");
			break;
		} else if (ret == 0) {
			printf("time out...\n");
			continue;
		}

		if(FD_ISSET(sock_fd, &fds)) { //测试sock是否可读，即是否网络上有数据  
            struct sockaddr_un client_name;
			socklen_t client_name_len;
			int client_socket_fd = 0;

			client_socket_fd = accept(sock_fd, (struct sockaddr*)&client_name, &client_name_len);
			client_sent_quit_message = server(client_socket_fd);
			close(client_socket_fd);
        }      
	}

	close(sock_fd);			// 关闭套接字文件
	unlink(socket_name);	// 删除套接字文件

	return 0;
}


// g++ -o server server.cpp
// ./server /tmp/socket