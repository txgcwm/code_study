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

	for(int i = 0; i < data.size(); i++) {
		int magic[4] = {0};

		for(int j = 0; j < 4; j++) {
			magic[j] = data[i] + m_pattern[j];

			it = std::find(data.begin(), data.end(), magic[j]);
		    if(it != data.end()) {
		    	int position = std::distance(data.begin(), it);
		        // std::cout << "element is found." << '\n';
		    }
		}
	}

	return bget;
}


