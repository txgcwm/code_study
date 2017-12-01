#ifndef __ILINKMANAGER_H__
#define __ILINKMANAGER_H__

#include <string>



namespace Mqtt{

class ILinkManager
{
public:
	static ILinkManager *Instance();
	
	ILinkManager(){}
	virtual ~ILinkManager(){}

	virtual int Start(std::string clientid, std::string username, std::string password) = 0;
	virtual int Stop() = 0;

	virtual int Publish(std::string topic, std::string data) = 0;
	virtual int Subscribe(std::string topic) = 0;
};

} // end mqtt

#endif


