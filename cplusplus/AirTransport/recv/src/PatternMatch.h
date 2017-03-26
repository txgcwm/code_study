#ifndef __PATTERNMATCH_H__
#define __PATTERNMATCH_H__

class CPatternMatch
{
public:
	CPatternMatch(char pattern[]);
	~CPatternMatch();

private:
	char m_pattern[4];
};

#endif