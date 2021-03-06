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

#if 1
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
        	printf("ccrc16(%d)=dcrc16(%d) size(%ld), len(%d), raw data: %s\n",
        			ccrc8, dcrc8, data.size(), length, raw);
        	record = raw;
        	return true;
      	}
    }

  	return false;
}
#else
static bool lessmark(const int& ele1, const int& ele2)
{
    return ele1 > ele2;
}

bool CPatternMatch::AnalyticSequenceData(std::vector<int> data, std::string &record)
{
  int length = -1;
  unsigned short ccrc8 = 0;

  if(((data[0] & 0xff0) == 0x3f0) && ((data[1] & 0xff0) == 0x3e0)
      && ((data[2] & 0xff0) == 0x3d0) && ((data[3] & 0xff0) == 0x3c0)) {
    ccrc8 = ((data[3] & 0xf) << 12) | ((data[2] & 0xf) << 8) | ((data[1] & 0xf) << 4) | (data[0] & 0xf);
  } else {
    return false;
  }

  if(((data[4] & 0xff0) == 0x3b0)
      && ((data[5] & 0xff0) == 0x3a0)) {
    length = ((data[5] & 0xf) << 4) | (data[4] & 0xf);
  }

  if(length >= 0 && data.size() >= 2 * length + CRC_LEN_TOTAL) {
      char raw[32] = {0};

      for(int i = CRC_LEN_TOTAL; i < 2 * length + CRC_LEN_TOTAL; i += 2) {
        // printf("%x %x\n", (data[i + 1] & 0xff0), (data[i] & 0xff0));
        // printf("%x ", ((data[i + 1] & 0xf) << 4) | (data[i] & 0xf));
        raw[(i - CRC_LEN_TOTAL)/2] = ((data[i + 1] & 0xf) << 4) | (data[i] & 0xf);
      }

      unsigned short dcrc8 = crc_ccitt((unsigned char*)raw, length);
      if(dcrc8 == ccrc8) {
        printf("ccrc16(%d) = dcrc16(%d) size(%ld), len(%d), raw data(%s)\n",
                ccrc8, dcrc8, data.size(), length, raw);
        record = raw;
        return true;
      }
    }

  return false;
}
#endif

bool CPatternMatch::Analyze(std::vector<int> data, int position, int magic, std::string &record)
{
	bool res = false;
	int hlen = data.size() - position - 1;
	std::vector<int> hybrid;

#if 1
	int expect = 2;

	for(int i = 0; i < hlen; i++) {
		int value = data[position + 1 + i] - magic + 21;
		int index = (value & 0x0ff0) >> 4;

		if(index < 2) {
			continue;
		}

		// printf("raw: %d, value: %d, index: %d, expect: %d\n", data[position + 1 + i], value, index, expect);

		if(index == expect || index == (expect + 1)) {
			hybrid.push_back(value);
			if(index > expect) {
				expect++;
			}
		} else {
			break;
		}
	}
#else
	int expect = 0x3f;

  	for(int i = data.size() - 1; i > position; i--) {
	    int value = data[i] - magic + 21;
	    int index = (value & 0x0ff0) >> 4;

	    if(index < 2) {
	      break;
	    }

	    printf("raw: %d, value: %d, index: %d, expect: %d\n", data[i], value, index, expect);

	    if(index == expect || index == (expect - 1)) {
	      	hybrid.push_back(value);
	      	if(index < expect) {
	        	expect--;
	      	}
	    } else {
	      	break;
	    }
  	}

  	std::sort(hybrid.begin(), hybrid.end(), lessmark);
#endif

	if(hybrid.size() < CRC_LEN_TOTAL) {
		return false;
	}

  	typedef TupleHelper<std::vector<int> >::Type Tuple;
  	std::vector<Tuple> result = GetFirstCombination(hybrid.begin(), hybrid.end());

  	// int count = 0;

  	// printf("\n*********************************************\n");

  	do {
  		std::vector<int> vec;

  		for(std::vector<Tuple>::iterator it = result.begin();
  			it != result.end(); ++it) {
      		vec.push_back(*boost::get<1>(*it));
  		}

  		// count++;

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

 //  	for(int i = 0; i < hybrid.size(); i++) {
	// 	printf("%3x ", hybrid[i]);
	// }

 //  	printf("\n***********************  count(%d) **********************\n", count);

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


