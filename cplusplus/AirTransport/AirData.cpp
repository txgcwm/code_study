#include <stdio.h>

#include "AirData.h"



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

void PrintData(std::vector<int> data)
{
	for(int i = 0; i < data.size(); i++) {
  		printf("%x ", data[i]);
  	}

  	printf("\n");

  	return;
}

/*********************************************************************************************/

CAirData::CAirData()
: m_getMagic(false)
, m_length(0)
, m_crc8(0)
{
}

CAirData::~CAirData()
{
}

void CAirData::Analysis(int len)
{
	std::vector<int>::iterator lowIter;
  	lowIter = std::lower_bound(m_data.begin(), m_data.end(), len);

  	// if get magic success, exclude inconsistent data
  	if(m_getMagic) {
  		int index = ((len - m_magicNum + 21) >> 4) & 0x3f;
  		if(index <= 0 || index > 63) {
  			printf("(%d) is non conformity rule!\n", len);
  			return;
  		}
  	}

  	if(lowIter == m_data.begin() || *lowIter != len) {
  		m_data.insert(lowIter, len);
  	} else {
  		// do nothing
  		printf("no need to insert (%d)!\n", len);
  	}

  	if(m_data.size() >= 4 && !m_getMagic) {
  		if(m_data[3] - m_data[2] == 2 && m_data[2] - m_data[1] == 3
  			&& m_data[1] - m_data[0] == 1) {
  			printf("find data channel!\n");
  			m_getMagic = true;
  			m_magicNum = m_data[0];
  		}
  	}

  	if(m_data.size() >= 8 && m_length <= 0) {
  		if((((m_data[6] - m_magicNum + 21) & 0xf0) == 0x40) && (((m_data[7] - m_magicNum + 21) & 0xf0) == 0x50)) {
  			m_length = (((m_data[7] - m_magicNum + 21) & 0xf) << 4) | ((m_data[6] - m_magicNum + 21) & 0xf);
  			printf("data len: %d\n", m_length);
  		}
  	}

  	if(m_length > 0 && m_data.size() >= 2 * m_length + 8) {
  		printf("get all data, size(%ld)\n", m_data.size());
  		char data[32] = {0};

  		for(int i = 8; i < m_data.size(); i += 2) {
  			// printf("%x %x\n", ((m_data[i + 1] - m_magicNum + 21) & 0xff0), ((m_data[i] - m_magicNum + 21) & 0xff0));
  			printf("%x ", (((m_data[i + 1] - m_magicNum + 21) & 0xf) << 4) | ((m_data[i] - m_magicNum + 21) & 0xf));
  			data[(i - 8)/2] = (((m_data[i + 1] - m_magicNum + 21) & 0xf) << 4) | ((m_data[i] - m_magicNum + 21) & 0xf);
  		}

  		printf("\ndata: %s\n", data);

  		if(((m_data[4] - m_magicNum + 21) & 0x20) && ((m_data[5] - m_magicNum + 21) & 0x30)) {
  			m_crc8 = (((m_data[5] - m_magicNum + 21) & 0xf) << 4) | ((m_data[4] - m_magicNum + 21) & 0xf);
  			printf("get crc8: %d\n", m_crc8);
  		}

  		char dcrc8 = crc8(data, m_length);
  		if(dcrc8 == m_crc8) {
  			printf("recv data crc8 right!\n");
  		}
  	}

  	// PrintData(m_data);

  	return;
}


