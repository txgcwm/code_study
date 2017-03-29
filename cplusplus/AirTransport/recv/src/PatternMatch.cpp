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

#if 0
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
#endif

bool CPatternMatch::AnalyticSequenceData(std::vector<int> data, std::string &record)
{
	int length = 0;

	int ccrc8 = 0;
	if(((data[0] & 0xf0) == 0x20) && ((data[1] & 0xf0) == 0x30)) {
		ccrc8 = ((data[1] & 0xf) << 4) | (data[0] & 0xf);
		printf("get crc8: %d\n", ccrc8);
	} else {
		return false;
	}

    if(((data[2] & 0xf0) == 0x40)
    	&& ((data[3] & 0xf0) == 0x50)) {
		length = ((data[3] & 0xf) << 4) | (data[2] & 0xf);
		// printf("data len: %d\n", length);
	}

	if(length > 0 && data.size() >= 2 * length + 4) {
  		char raw[32] = {0};

  		for(int i = 4; i < data.size(); i += 2) {
  			// printf("%x %x\n", (data[i + 1] & 0xff0), (data[i] & 0xff0));
  			printf("%x ", ((data[i + 1] & 0xf) << 4) | (data[i] & 0xf));
  			raw[(i - 4)/2] = ((data[i + 1] & 0xf) << 4) | (data[i] & 0xf);
  		}

  		printf("\nsize(%ld), len(%d), raw data: %s\n", data.size(), length, raw);

  		char dcrc8 = crc8(raw, length);
  		if(dcrc8 == ccrc8) {
  			printf("recv data crc8 right!\n");
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

  	do {
  		std::vector<int> vec;

  		for(std::vector<Tuple>::iterator it = result.begin();
  			it != result.end(); ++it) {
      		vec.push_back(*boost::get<1>(*it));
  		}

  		// for(int i = 0; i < vec.size(); i++) {
  		// 	printf("%3x ", vec[i]);
  		// }

  		// printf("\n");

  		if(vec.size() >= 4) {
  			if(AnalyticSequenceData(vec, record)) {
  				res = true;
  				break;
  			}
  		}
  	} while (NextCombination(result.begin(), result.end()));

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


