#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>


uint32_t to_s_addr(uint8_t addr1, uint8_t addr2, uint8_t addr3, uint8_t addr4)
{
    union {
        uint8_t     u8[4];
        uint32_t    u32;
    } addr;

    addr.u8[0] = addr1;
    addr.u8[1] = addr2;
    addr.u8[2] = addr3;
    addr.u8[3] = addr4;

    return addr.u32;
}

int SendPacket(int s, int addr1, int addr2, int addr3, int addr4, int port)
{ 
    struct sockaddr_in mcast_addr;

    memset(&mcast_addr, 0, sizeof(mcast_addr));

    mcast_addr.sin_family = AF_INET;
    mcast_addr.sin_addr.s_addr = to_s_addr(addr1, addr2, addr3, addr4);
    mcast_addr.sin_port = htons(port);

    int n = sendto(s, "hello world", strlen("hello world"), 0,
                    (struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
    if (n < 0) {
        return -1;
    }

    return 0;
}

int SendData(int addr4)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        return -1;
    }

    SendPacket(sock, 224, 0, 0, addr4, 67819);

    close(sock);
    sock = -1;

	return 0;
}

int main(int argc, char **argv)
{
	while(1) {
		for(int i = 23; i <= 255; i++) {
			SendData(i);
		}

		sleep(1);
	}

	return 0;
}


