#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>


#define MCAST_PORT 8888



void send_multicast_data(int sock)
{
	struct sockaddr_in mcast_addr;     

    memset(&mcast_addr, 0, sizeof(mcast_addr));

    char *ipaddr[8] = {"224.0.0.10",
    					"224.0.0.20",
    					"224.0.0.30",
    					"224.0.0.40",
    					"224.0.0.50",
    					"224.0.0.60",
    					"224.0.0.70",
    					"224.0.0.80" };

    mcast_addr.sin_family = AF_INET;
    mcast_addr.sin_port = htons(MCAST_PORT);

	while(1) {
		int i = 0;

		for(i = 0; i < 8; i++) {
			mcast_addr.sin_addr.s_addr = inet_addr(ipaddr[i]);

	        int n = sendto(sock, "1", 1, 0, (struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
	        if( n < 0) {
	            perror("sendto error!\n");
	            return;
	        }

	        usleep(300 * 1000);
		}	
    }

	return;
}

int main(int argc, char **argv)
{
	int sock = -1;
	struct in_addr interface_addr;
	int addr_size = sizeof(struct in_addr);

	if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket fail!\n");
	}

	if((getsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &interface_addr, &addr_size)) < 0) {
		perror("getsockopt fail!\n");
	}

	u_char loop = 0;
	u_char ttl = 0;
	int size;

	getsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, &size);
	getsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, &size);

	printf("default interface: %s, loop: %d, ttl: %d\n", inet_ntoa(interface_addr), loop, ttl);

	// ttl = 0;
	// setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

	send_multicast_data(sock);

	close(sock);
	sock = -1;

	return 0;
}


