#ifndef __ILINK_H__
#define __ILINK_H__

#include <string>



namespace Mqtt {

class ILink
{
public:
	typedef struct {
		std::string clientid;
		int nodelimiter;
		char delimiter;
		int qos;
		int retained;
		std::string username;
		std::string password;
		int keepalive;
	} Params;

public:
	ILink() {}
	virtual ~ILink(){}
	
	virtual int Create(std::string clientid, std::string username, std::string password) = 0;
	virtual int Connect() = 0;
	virtual int Disconnect() = 0;
	virtual int Destroy() = 0;

	virtual int Publish(std::string topic, std::string data) = 0;
	virtual int Subscribe(std::string topic) = 0;
};

} // end mqtt

#endif


