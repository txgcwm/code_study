#include <string.h>
#include <stdio.h>
#include <string>

#include "RouteInfo.h"



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


CRouteInfo::CRouteInfo()
: m_len(0)
, m_count(0)
, m_rlen(0)
, m_bgetlen(false)
{
	memset(m_bIndex, 0, sizeof(m_bIndex));
}

CRouteInfo::~CRouteInfo()
{

}

void CRouteInfo::SetSsidPassword(std::string password, std::string ssid)
{
	int len = 0;
	char raw[64] = {0};

	if(ssid.size() > 0) {
		len = ssid.size() + password.size() + 1;
		sprintf(raw, "%s\n%s", password.c_str(), ssid.c_str());
	} else {
		len = password.size();
		sprintf(raw, "%s", password.c_str());
	}

	unsigned char crc = (unsigned char)crc8(raw, len);
	m_data[0] = crc;
	m_data[1] = len;

	memcpy(m_data + 2, raw, len);

	m_len = len + 2;

	printf("len(%d), crc(%d)\n", m_len, crc);

	return;
}

int CRouteInfo::RecvData(int index, unsigned char data1, unsigned char data2)
{
	int realIndex = 64 - index;

	if(m_bIndex[realIndex * 2]) {
		return 0;
	}

	m_bIndex[realIndex * 2] = true;
	m_rdata[realIndex * 2] = data1;
	m_rdata[realIndex * 2 + 1] = data2;
	m_count += 2;

	if(realIndex == 0) {
		m_rlen = m_rdata[1];
		m_bgetlen = true;
	}

	printf("count: %d\n", m_count);

	if(m_bgetlen && m_count - 2 > m_rlen) {
		printf("try to check data\n");
		unsigned char crc = (unsigned char)crc8((char *)m_rdata + 2, m_rlen);
		if(crc == m_rdata[0]) {
			printf("get success\n");
		}
	}

	return 0;
}

int CRouteInfo::SendData()
{
	int index = 64;

	for(int i = 0; i < m_len; i += 2, index--) {
		// index, m_data[i] m_data[i + 1]
		printf("index(%x), data[%u, %u]\n", index, m_data[i], m_data[i + 1]);
		RecvData(index, m_data[i], m_data[i + 1]);
	}

	return 0;
}


