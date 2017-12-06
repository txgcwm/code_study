#ifndef __LINKMANAGER_H__
#define __LINKMANAGER_H__

#include "ILink.h"
#include "ILinkManager.h"



namespace Mqtt {

class CLinkManager : public ILinkManager
{
public:
	static CLinkManager *Instance();

	virtual int Start(std::string clientid, std::string username, std::string password);
	virtual int Stop();

	virtual int Publish(std::string topic, std::string data);
	virtual int Subscribe(std::string topic);

private:
	CLinkManager();
	~CLinkManager();
	
private:
	ILink 			*m_link;
};

}

#endif


