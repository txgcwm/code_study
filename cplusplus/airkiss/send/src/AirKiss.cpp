#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "AirKiss.h"



namespace Slink {

static inline unsigned char crc8(const unsigned char *data, int len)
{
    int i = 0;
    unsigned char crc = 0x00;

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

static inline void generate_random_data(char *buf, int len)
{
    bzero(buf, len);

    srand((int)time(NULL));

    for (int i = 0; i < len - 1; i++) {
        buf[i] = (int)(255.0 * rand()/(RAND_MAX + 1.0));
    }

    buf[len - 1] = '\0';

    return;
}

static inline char get_random()
{
    srand((int)time(NULL));
    char random = 1 + (int)((float)127 * rand()/(RAND_MAX + 1.0));
    printf("random: %d\n", random);

    return random;
}

template<typename T>
static inline void print_data(const char* msg, T data, int len)
{
    printf("%s: ", msg);

    for(int i = 0; i < len; ++i) {
        printf("%d ", data[i]);
    }
    printf("\n");

    return;
}

/**************************************************************************************************/

CAirKiss::CAirKiss()
: CThreadLoop("AirKiss")
, m_encodedData(NULL)
, m_length(0)
{
}

CAirKiss::~CAirKiss()
{
	Reset();
}

bool CAirKiss::SetRouteInfo(std::string ssid, std::string password)
{
	m_encodedData = new int[2 << 14];
	m_length = 0;

	char random = get_random();

	LeadingPart();
	MagicCode(ssid, password);

    for (int i = 0; i < 8; ++i) {
        int index = 0;
        std::string data = password + random + ssid;
        
        PrefixCode(password);

        for(index = 0; index < data.size() / 4; ++index) {
            Sequence(index, &data[index * 4], 4);
        }

        int remainder = data.length() % 4;
        if(remainder != 0) {
            Sequence(index, &data[index * 4], remainder);
        }
    }

    print_data("data", m_encodedData, m_length);

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

    return;
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

void CAirKiss::MagicCode(std::string ssid, std::string password)
{
    int length = ssid.size() + password.size() + 1;
    int magicCode[4] = {0};

    magicCode[0] = 0x00 | ((length >> 4) & 0xF);
    if (magicCode[0] == 0) { // why ?
        magicCode[0] = 0x08;
    }

    magicCode[1] = 0x10 | (length & 0xF);

    unsigned char crc = crc8((const unsigned char *)ssid.data(), ssid.size());
    magicCode[2] = 0x20 | (crc >> 4 & 0xF);
    magicCode[3] = 0x30 | (crc & 0xF);

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            AppendEncodedData(magicCode[j]);
        }
    }

    return;
}

void CAirKiss::PrefixCode(std::string password)
{
    unsigned char length = password.size();
    int prefixCode[4] = {0};

    prefixCode[0] = 0x40 | (length >> 4 & 0xF);
    prefixCode[1] = 0x50 | (length & 0xF);

    unsigned char crc = crc8((const unsigned char *)&length, 1);
    prefixCode[2] = 0x60 | (crc >> 4 & 0xF);
    prefixCode[3] = 0x70 | (crc & 0xF);

    for (int i = 0; i < 4; ++i) {
        AppendEncodedData(prefixCode[i]);
    }

    return;
}

void CAirKiss::Sequence(int index, char *data, int len)
{
    unsigned char content[5] = {0};

    content[0] = (char)(index & 0xFF);
    memcpy(&content[1], data, len);

    unsigned char crc = crc8((const unsigned char *)content, len + 1);

    AppendEncodedData(0x80 | crc);
    AppendEncodedData(0x80 | index);

    for(int i = 0; i < len; i++) {
        AppendEncodedData(data[i] | 0x100);
    }

    return;
}

void CAirKiss::EventHandleLoop()
{
	int sockfd = -1;
	struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

	if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(-1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9080);
#if 1
    int broadcast = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    server_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
#else
	server_addr.sin_addr.s_addr = inet_addr("224.0.0.251");
#endif

	while(1) {
		for(int i = 0; i < m_length; i++) {
			char buf[1500] = {0};
			int len = m_encodedData[i];

			generate_random_data(buf, len + 1);

			if(sendto(sockfd, buf, len, 0, (struct sockaddr*)&server_addr, addr_len) < 0) {
				perror("send to");
			}

            WaitForSleep(5);
		}

		if(WaitForSleep(10) != 0) {
			break;
		}
	}

	close(sockfd);
    sockfd = -1;

	return;
}

}  // end namespace


// http://yk8900.blog.163.com/blog/static/12318354420166261432939/
// https://github.com/pannzh/Airkiss