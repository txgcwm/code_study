#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <vector>

#include "AirTransport.h"

using namespace std;



inline char crc8(const char *data, int len)
{
    int i = 0;
    char crc = 0x00;

    while(len-- > 0) {
        char extract = data[i++];

        for (char tempI = 8; tempI != 0; tempI--) {
            char sum = (char) ((crc & 0xFF) ^ (extract & 0xFF));
            sum = (char) ((sum & 0xFF) & 0x01);
            crc = (char) ((crc & 0xFF) >> 1);

            if (sum != 0) {
                crc = (char)((crc & 0xFF) ^ 0x8C);
            }

            extract = (char) ((extract & 0xFF) >> 1);
        }
    }

    return (crc & 0xFF);
}

inline void generateRandom(char *buf, int len)
{
    bzero(buf, len);

    srand((int)time(NULL));

    for (int i = 0; i < len - 1; i++) {
        buf[i] = (int)(255.0 * rand()/(RAND_MAX + 1.0));
    }

    buf[len - 1] = '\0';

    return;
}

void PrintData(int *content, int size)
{
	int j = 0;

	printf("split data len: %d\n", size);

	for(j = 0; j < size; j++) {
		printf("0x%x ", content[j]);
	}

	printf("\n");

	for(j = 0; j < size; j++) {
		printf("%d ", content[j]);
	}

	printf("\n");
}

inline void SetSendData(int *content, int &size, string ssid, string password)
{
	char raw[64] = {0};
	int len = 0;

	if(ssid.size() > 0) {
		len = ssid.size() + password.size() + 1;
		sprintf(raw, "%s\n%s", password.c_str(), ssid.c_str());
	} else {
		len = password.size();
		sprintf(raw, "%s", password.c_str());
	}

	char crc = crc8(raw, len);
	content[0] = 0x20 | (crc & 0x0f);
	content[1] = 0x30 | (crc >> 4);

	content[2] = 0x40 | (len & 0x0f);
	content[3] = 0x50 | (len >> 4);

	for(int i = 0; i < 2 * len; i++) {
		if(i % 2 == 0) {
			content[4 + i] = ((6 + i) << 4) | (raw[i/2] & 0x0f);
		} else {
			content[4 + i] = ((6 + i) << 4) | (raw[i/2] >> 4);
		}
	}

	size = 2 * len + 4;

	PrintData(content, size);

	printf("crc8: %d, len: %d, size: %d\n", crc, len, size);

	return;
}

/****************************************************************************************************/

CAirTransport::CAirTransport()
: CThreadLoop("AirKiss")
{
}

CAirTransport::~CAirTransport()
{
	Stop();
}

bool CAirTransport::Start(std::string ssid, std::string password)
{
	m_ssid = ssid;
	m_password = password;

	return StartThread();
}

bool CAirTransport::Stop()
{
	m_ssid = "";
	m_password = "";

	return StopThread();
}

void SendPacket(int sockfd, int len, struct sockaddr *server_addr, socklen_t addr_len)
{
	char buf[1500] = {0};

	generateRandom(buf, len + 1);

	if(sendto(sockfd, buf, len, 0, server_addr, addr_len) < 0) {
		perror("sendrto");
	}

	return;
}

void CAirTransport::EventHandleLoop()
{
	int sockfd;
	char buf[64];
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return;
	}

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("224.0.0.251");
	server_addr.sin_port = htons(9080);

#if 0
	server_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
	int so_broadcast = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));
#endif

	int size = 0;
	int content[64] = {0};
	SetSendData(content, size, m_ssid, m_password);

	char magic[4] = {21, 22, 25, 27};

	while(1) {
		for(int j = 0; j < 4; j++) {
			SendPacket(sockfd, magic[j], (sockaddr *)&server_addr, addr_len);
		}

		for(int i = 0; i < size; i++) {
			SendPacket(sockfd, content[i], (sockaddr *)&server_addr, addr_len);
		}

		if(WaitForSleep(100) != 0) {
			break;
		}
	}

	close(sockfd);

	return;
}


