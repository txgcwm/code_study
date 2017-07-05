#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>



void write_text(int socket_fd, const char* text)
{
	int length = strlen(text) + 1;

	write(socket_fd, &length, sizeof(length));		// 写入长度信息
	write(socket_fd, text, length);					// 写入消息

	printf("client: (%d) (%s)\n", length, text);

	return;
}

int main(int argc, char *argv[])
{
	const char* const socket_name = argv[1];		// 套接字文件路径
	const char* const message = argv[2];			// 要发送的消息
	int socket_fd;
	struct sockaddr_un name;

	socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);	// 创建本地套接字

	name.sun_family = AF_LOCAL;
	strcpy(name.sun_path, socket_name);

	connect(socket_fd, (const struct sockaddr*)&name, SUN_LEN(&name));		// 连接套接字

	write_text(socket_fd, message);					// 向套接字写入数据

	close(socket_fd);
	
	return 0;
}

// g++ -o client client.cpp

// ./client /tmp/socket "what can you do for me"
// ./client /tmp/socket "what are\nyou doing"
// ./client /tmp/socket "this program is great"
// ./client /tmp/socket "Quit"