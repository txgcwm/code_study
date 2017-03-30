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


#define CRC_LEN_TOTAL       6


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

bool CPatternMatch::AnalyticSequenceData(std::vector<int> data, std::string &record)
{
  	int length = -1;
  	unsigned short ccrc8 = 0;

  	if(((data[0] & 0xf0) == 0x20) && ((data[1] & 0xf0) == 0x30)
      	&& ((data[2] & 0xf0) == 0x40) && ((data[3] & 0xf0) == 0x50)) {
    	ccrc8 = ((data[3] & 0xf) << 12) | ((data[2] & 0xf) << 8) | ((data[1] & 0xf) << 4) | (data[0] & 0xf);
    	// printf("get crc8: %d\n", ccrc8);
	} else {
	    return false;
	}

    if(((data[4] & 0xf0) == 0x60)
      	&& ((data[5] & 0xf0) == 0x70)) {
    	length = ((data[5] & 0xf) << 4) | (data[4] & 0xf);
    	// printf("data len: %d\n", length);
  	}

  	if(length >= 0 && data.size() >= 2 * length + CRC_LEN_TOTAL) {
      	char raw[32] = {0};

      	for(int i = CRC_LEN_TOTAL; i < 2 * length + CRC_LEN_TOTAL; i += 2) {
        	// printf("%x %x\n", (data[i + 1] & 0xff0), (data[i] & 0xff0));
        	// printf("%x ", ((data[i + 1] & 0xf) << 4) | (data[i] & 0xf));
        	raw[(i - CRC_LEN_TOTAL)/2] = ((data[i + 1] & 0xf) << 4) | (data[i] & 0xf);
      	}

      	// printf("size(%ld), len(%d), raw data: %s\n", data.size(), length, raw);

      	unsigned short dcrc8 = crc_ccitt((unsigned char*)raw, length);
      	if(dcrc8 == ccrc8) {
        	printf("recv data crc16(ccrc16(%d), dcrc16(%d)) right!\n", ccrc8, dcrc8);
        	record = raw;
        	return true;
      	}
    }

  	return false;
}

bool CPatternMatch::Analyze(std::vector<int> data, int position, int magic, std::string &record)
{
	bool res = false;
	int hlen = data.size() - position - 1;
	std::vector<int> hybrid;

	hybrid.resize(hlen);

	for(int i = 0; i < hlen; i++) {
		hybrid[i] = data[position + 1 + i] - magic + 21;
	}

  	typedef TupleHelper<std::vector<int> >::Type Tuple;
  	std::vector<Tuple> result = GetFirstCombination(hybrid.begin(), hybrid.end());

  	int count = 0;

  	printf("\n*********************************************\n");

  	do {
  		std::vector<int> vec;

  		for(std::vector<Tuple>::iterator it = result.begin();
  			it != result.end(); ++it) {
      		vec.push_back(*boost::get<1>(*it));
  		}

  		count++;

  		if(vec.size() >= CRC_LEN_TOTAL) {
  			// for(int i = 0; i < vec.size(); i++) {
	  		// 	printf("%3x ", vec[i]);
	  		// }

	  		// printf("\nmagic: %d\n", magic);

  			if(AnalyticSequenceData(vec, record)) {
  				// res = true;
  				// break;
  			}
  		}
  	} while (NextCombination(result.begin(), result.end()));

  	for(int i = 0; i < hybrid.size(); i++) {
		printf("%3x ", hybrid[i]);
	}

  	printf("\n***********************  count(%d) **********************\n", count);

  	// printf("size: %d, position: %d\n", data.size(), position);

	return res;
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


