#ifndef __LINKASYNC_H__
#define __LINKASYNC_H__

extern "C" {

#include "MQTTClient.h"
#include "MQTTAsync.h"
#include "MQTTClientPersistence.h"

}

#include "ILink.h"



namespace Mqtt {

class CLinkAsync : public ILink
{
public:
	CLinkAsync();
	~CLinkAsync();
	
	virtual int Create(std::string clientid, std::string username, std::string password);
	virtual int Connect();
	virtual int Disconnect();
	virtual int Destroy();

	virtual int Publish(std::string topic, std::string data);
	virtual int Subscribe(std::string topic);

private:
	int MessageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* m);
	static int MessageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* m);

private:
	MQTTClient m_client;
};

} // end Mqtt

#endif


