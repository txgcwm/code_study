#ifndef __ROUTEINFO_H__
#define __ROUTEINFO_H__

#include <string>



class CRouteInfo
{
public:
	CRouteInfo();
	~CRouteInfo();

	void SetSsidPassword(std::string password, std::string ssid);
	int SendData();

	int RecvData(int index, unsigned char data1, unsigned char data2);

private:
	unsigned char m_data[40];
	int  m_len;

	bool m_bIndex[40];
	int  m_count;
	unsigned char m_rdata[40];
	int  m_rlen;
	bool m_bgetlen;
};

#endif