#ifndef __AIRTRANSPORT_H__
#define __AIRTRANSPORT_H__

#include <string>

#include "ThreadLoop.h"


class CAirTransport: public CThreadLoop
{
public:
	CAirTransport();
	~CAirTransport();
	
	bool Start(std::string ssid, std::string password);
	bool Stop();

	virtual void EventHandleLoop();

private:
	std::string m_ssid;
	std::string	m_password;
};

#endif