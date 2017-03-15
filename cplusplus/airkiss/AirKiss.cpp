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

#include "AirKiss.h"

using namespace std;



namespace Slink {

inline int crc8(const char *data, int len)
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
        buf[i] = 'a' + (0 + (int)(26.0 * rand()/(RAND_MAX + 1.0)));
    }

    buf[len - 1] = '\0';

    return;
}

/**************************************************************************************************/

CAirKiss::CAirKiss()
: CThreadLoop("AirKiss")
{
}

CAirKiss::~CAirKiss()
{
	Reset();
}

bool CAirKiss::SetRouteInfo(string ssid, string password)
{
	m_encodedData = new int[2 << 14];
	m_length = 0;

	char random = 1 + (int)(127 * rand()/(RAND_MAX + 1.0));

	LeadingPart();
	MagicCode(ssid, password);

	for (int i = 0; i < 15; ++i) {
        int index;
        string data = password + random + ssid;
        
        PrefixCode(password);

        for(index = 0; index < data.size() / 4; ++index) {
            Sequence(index, &data[index * 4], 4);
        }

        int remainder = data.length() % 4;
        if(remainder != 0) {
            Sequence(index, &data[index * 4], remainder);
        }
	}

	for(int i = 0; i < m_length; i++) {
		printf("%d ", m_encodedData[i]);
	}

	return StartThread();
}

bool CAirKiss::Reset()
{
	if(m_encodedData != NULL) {
		delete m_encodedData;
		m_encodedData = NULL;
	}

	m_length = 0;

	return StopThread();
}

void CAirKiss::AppendEncodedData(int length)
{
    m_encodedData[m_length++] = length;
}

void CAirKiss::LeadingPart()
{
    for (int i = 0; i < 50; ++i) {
        for (int j = 1; j <= 4; ++j) {
            AppendEncodedData(j);
        }
    }

    return;
}

void CAirKiss::MagicCode(string ssid, string password)
{
    int length = ssid.size() + password.size() + 1;
    int magicCode[4] = {0};

    magicCode[0] = 0x00 | ((length >> 4) & 0xF);
    if (magicCode[0] == 0) { // why ?
        magicCode[0] = 0x08;
    }

    magicCode[1] = 0x10 | (length & 0xF);

    int crc = crc8(ssid.data(), ssid.size());
    magicCode[2] = 0x20 | (crc >> 4 & 0xF);
    magicCode[3] = 0x30 | (crc & 0xF);

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            AppendEncodedData(magicCode[j]);
        }
    }

    return;
}

void CAirKiss::PrefixCode(string password)
{
    int length = password.size();
    int prefixCode[4] = {0};

    prefixCode[0] = 0x40 | (length >> 4 & 0xF);
    prefixCode[1] = 0x50 | (length & 0xF);

    int crc = crc8(password.data(), password.size());
    prefixCode[2] = 0x60 | (crc >> 4 & 0xF);
    prefixCode[3] = 0x70 | (crc & 0xF);

    for (int j = 0; j < 4; ++j) {
        AppendEncodedData(prefixCode[j]);
    }

    return;
}

void CAirKiss::Sequence(int index, char *data, int len)
{
    char content[5] = {0};

    content[0] = (char)(index & 0xFF);
    memcpy(&content[1], data, len);

    int crc = crc8(content, len + 1);

    AppendEncodedData(0x80 | crc);
    AppendEncodedData(0x80 | index);

    for(int i = 0; i < len; i++) {
        AppendEncodedData(data[i] | 0x100);
    }

    return;
}

void CAirKiss::EventHandleLoop()
{
	int sockfd;
	socklen_t addr_len;
	char buf[64];
	struct sockaddr_in server_addr;

	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(-1);
	}
	printf("sockfd = %d\n",sockfd);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("224.0.0.251");
	server_addr.sin_port = htons(9080);
	addr_len = sizeof(server_addr);

	while(1) {
		for(int i = 0; i < m_length; i++) {
			char buf[1500] = {0};
			int len = m_encodedData[i];

			generateRandom(buf, len + 1);

			if(sendto(sockfd, buf, len, 0, (struct sockaddr*)&server_addr, addr_len) < 0) {
				perror("sendrto");
			}
		}

		if(WaitForSleep(10) != 0) {
			break;
		}
	}

	close(sockfd);

	return;
}

}  // end namespace


