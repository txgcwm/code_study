#include "PatternMatch.h"



CPatternMatch::CPatternMatch(char pattern[])
{
	memcpy(m_pattern, pattern, sizeof(m_pattern));
}

CPatternMatch::~CPatternMatch()
{
}

bool CPatternMatch::QueryIdentificationPackage(vector<int> data)
{
	bool bget = false;

	std::vector<int>::iterator it;

	for(int i = 0; i < data.size(); i++) {
		int magic[4] = {0};

		for(int j = 0; j < 4; j++) {
			magic[j] = data[i] + m_pattern[j];

			it = find(data.begin(), data.end(), magic[j]);
		    if(it != data.end()) {
		    	int position = distance(data.begin(), magic);
		        std::cout << "element is found." << '\n';
		    }
		}

	}
    

	return bget;
}


