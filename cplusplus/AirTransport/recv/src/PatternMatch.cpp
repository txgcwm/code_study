#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <iterator> 
#include <string>
#include <vector>

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
		    	// int position = std::distance(data.begin(), it);
		    }
		}

		if(count == 4) {
			printf("find a magic num: %d\n", data[i]);
			m_magicList.push_front(data[i]);
		}
	}

	return bget;
}

void CPatternMatch::PrintMagicList()
{
	std::list<int>::iterator it;   
  
    for(it = m_magicList.begin(); it != m_magicList.end(); ++it) {
		printf("%d\n", *it);
    }

	return;
}

