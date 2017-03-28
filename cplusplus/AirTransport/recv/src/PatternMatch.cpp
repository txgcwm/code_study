#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <iterator> 
#include <string>
#include <vector>

#include "Common.h"
#include "PatternMatch.h"



CPatternMatch::CPatternMatch(char pattern[])
{
	memcpy(m_pattern, pattern, sizeof(m_pattern));
}

CPatternMatch::~CPatternMatch()
{
}

bool CPatternMatch::QueryIdentificationPackage(std::vector<int> data)
{
	bool bget = false;
	std::vector<int>::iterator it;
	int loop = (data.size() > 25) ? 21 : (data.size() - 4);

	for(int i = 0; i < loop; i++) {
		std::list<int>::iterator iter;

		iter = std::find(m_magicList.begin(), m_magicList.end(), data[i]);
		if(iter != m_magicList.end()) {
			continue;
		}

		int count = 0;

		for(int j = 0; j < 4; j++) {
			int magic = data[i] + m_pattern[j];

			it = std::find(data.begin(), data.end(), magic);
		    if(it != data.end()) {
		    	count++;
		    }
		}

		if(count == 4) {
			printf("find a magic num: %d\n", data[i]);
			m_magicList.push_front(data[i]);
			bget = true;
		}
	}

	return bget;
}

bool CPatternMatch::Analyze(std::vector<int> data, int position, int magic, std::string &record)
{
	int length = 0;

    if((((data[position + 3] - magic + 21) & 0xf0) == 0x40)
    	&& (((data[position + 4] - magic + 21) & 0xf0) == 0x50)) {
			length = (((data[position + 4] - magic + 21) & 0xf) << 4) | ((data[position + 3] - magic + 21) & 0xf);
			// printf("data len: %d\n", length);
		}

		if(length > 0 && data.size() >= 2 * length + position + 5) {
  		printf("position(%d), size(%ld), len(%d)\n", position, data.size(), length);
  		char raw[32] = {0};

  		for(int i = position + 5; i < data.size(); i += 2) {
  			// printf("%x %x\n", ((data[i + 1] - magic + 21) & 0xff0), ((data[i] - magic + 21) & 0xff0));
  			printf("%x ", (((data[i + 1] - magic + 21) & 0xf) << 4) | ((data[i] - magic + 21) & 0xf));
  			raw[(i - position - 5)/2] = (((data[i + 1] - magic + 21) & 0xf) << 4) | ((data[i] - magic + 21) & 0xf);
  		}

  		printf("\nraw data: %s\n", raw);

  		int ccrc8 = 0;
  		if(((data[position + 1] - magic + 21) & 0x20) && ((data[position + 2] - magic + 21) & 0x30)) {
  			ccrc8 = (((data[position + 2] - magic + 21) & 0xf) << 4) | ((data[position + 1] - magic + 21) & 0xf);
				printf("get crc8: %d\n", ccrc8);
  		}

  		char dcrc8 = crc8(raw, length);
  		if(dcrc8 == ccrc8) {
  			printf("recv data crc8 right!\n");
  			record = raw;
  			return true;
  		}
  	}

  	return false;
}

bool CPatternMatch::DecodeTransmitRecord(std::vector<int> data, std::string &record)
{
	std::list<int>::iterator it;   
  
    for(it = m_magicList.begin(); it != m_magicList.end(); ++it) {
    	int magic = *it;
    	std::vector<int>::iterator itv;

    	itv = std::find(data.begin(), data.end(), magic + m_pattern[3]);
    	if(itv != data.end()) {
		    int position = std::distance(data.begin(), itv);
		    if(data.size() < position + 1 + 4) {
		    	return false;
		    }

		    if(Analyze(data, position, magic, record)) {
		    	return true;
		    }
		}
    }

	return false;
}

void CPatternMatch::PrintMagicList()
{
	std::list<int>::iterator it;   
  
    for(it = m_magicList.begin(); it != m_magicList.end(); ++it) {
		printf("%d\n", *it);
    }

	return;
}


