#ifndef __PATTERNMATCH_H__
#define __PATTERNMATCH_H__

#include <vector>
#include <list>



class CPatternMatch
{
public:
	CPatternMatch(char pattern[]);
	~CPatternMatch();

	bool QueryIdentificationPackage(std::vector<int> data);
	bool DecodeTransmitRecord(std::vector<int> data, std::string &record);
	void PrintMagicList();

private:
	char m_pattern[4];
	std::list<int> m_magicList;
};

#endif