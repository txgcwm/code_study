#ifndef __LINKSYNC_H__
#define __LINKSYNC_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "MQTTClient.h"
	#include "MQTTClientPersistence.h"

#ifdef __cplusplus
}
#endif

#include "ILink.h"
#include "AddressPool.h"



namespace Mqtt {

class CLinkSync : public ILink
{
public:
	CLinkSync();
	~CLinkSync();
	
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
	MQTTClient 		m_client;
	ILink::Params	m_link_params;

	CAddressPool 	m_addr_port;
};

} // end Mqtt

#endif


