#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>



class CClient
{
public:
	static CClient *Instance();
	bool Start(std::string ip, int port);
    bool Stop();
    int SendData(char *pData, int len);

private:
	CClient();
	~CClient();

private:
	int 		m_sock;
	int 		m_port;
	std::string m_ip;
};

#endif