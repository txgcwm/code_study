#ifndef __AIRDATA_H__
#define __AIRDATA_H__

#include <vector>



class CAirData
{
public:
	CAirData();
	~CAirData();
	
	void Analysis(int len);

private:
	bool 				m_getMagic;
	int 				m_magicNum;
	int 				m_length;
	char 				m_crc8;
	std::vector<int> 	m_data;
};

#endif