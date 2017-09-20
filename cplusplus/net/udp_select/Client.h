#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>



class CClient
{
public:
	CClient();
	~CClient();

	bool Start(std::string ip, int port);
    bool Stop();
    int SendData(char *pData, int len);

private:
	int 		m_sock;
	int 		m_port;
	std::string m_ip;
};

#endif