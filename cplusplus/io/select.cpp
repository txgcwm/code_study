#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>



#define PORT 8888

int main(int argc, char *argv[])
{
	int opt = 1;
	int master_socket, new_socket, client_socket[30], max_clients =
		30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;
    socklen_t addrlen = sizeof(struct sockaddr_in);
	char buffer[1025];
	fd_set readfds;
	char message[] = "ECHO Daemon v1.0 \r\n";

	for (i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
	}

	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//set master socket to allow multiple connections , this is just a good habit, it will work without this
	if (setsockopt
		(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		 sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(master_socket, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("Listener on port %d, waiting for connections...\n", PORT);

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];

			if (sd > 0) {
				FD_SET(sd, &readfds);
            }

			if (sd > max_sd) {
				max_sd = sd;
            }
		}

		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)) {
			printf("select error");
		}

		//If something happened on the master socket , then its an incoming connection
		if (FD_ISSET(master_socket, &readfds)) {
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, &addrlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}

			printf("New connection, socket fd(%d), ip(%s), port(%d)\n",
				 new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

			if (send(new_socket, message, strlen(message), 0) != strlen(message)) {
				perror("send");
			}

			puts("Welcome message sent successfully");

			for (i = 0; i < max_clients; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n", i);
					break;
				}
			}
		}

		// some IO operation on some other socket :)
		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];

			if (FD_ISSET(sd, &readfds)) {
				//Check if it was for closing , and also read the incoming message
				if ((valread = read(sd, buffer, 1024)) == 0) {
					//Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr *)&address, &addrlen);
					printf("Host disconnected, ip %s, port %d\n",
						   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					close(sd);
					client_socket[i] = 0;
				} else {		//Echo back the message that came in
					//set the string terminating NULL byte on the end of the data read
					buffer[valread] = '\0';
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}

	return 0;
}


