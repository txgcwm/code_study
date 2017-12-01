#include "LinkAsync.h"



namespace Mqtt {

CLinkAsync::CLinkAsync()
{

}

CLinkAsync::~CLinkAsync()
{

}
	
int CLinkAsync::Create(std::string clientid, std::string username, std::string password)
{
	int rc = 0;

	// rc = MQTTAsync_create(&m_client, url, opts.clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL);

	// rc = MQTTAsync_setCallbacks(m_client, client, connectionLost, messageArrived, NULL);

	return rc;
}

int CLinkAsync::Connect()
{

}

int CLinkAsync::Disconnect()
{
	int rc = 0;
	// MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
	
	// // disc_opts.onSuccess = onDisconnect;
	// if ((rc = MQTTAsync_disconnect(m_client, &disc_opts)) != MQTTASYNC_SUCCESS)
	// {
	// 	printf("Failed to start disconnect, return code %d\n", rc);
	// }

	return rc;
}

int CLinkAsync::Destroy()
{
	// MQTTAsync_destroy(&m_client);
}

int CLinkAsync::Publish(std::string topic, std::string data)
{
	return 0;
}

int CLinkAsync::Subscribe(std::string topic)
{
	return 0;
}

int CLinkAsync::MessageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
	int rc = 0;

	printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %s\n", (char*)m->payload);

    MQTTClient_freeMessage(&m);
    MQTTClient_free(topicName);

	return rc;
}

int CLinkAsync::MessageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
	int rc = 0;

	if(context != NULL) {
		CLinkAsync *thiz = (CLinkAsync *)context;

		rc = thiz->MessageArrived(context, topicName, topicLen, m);
	}

	return rc;
}

} // end mqtt


