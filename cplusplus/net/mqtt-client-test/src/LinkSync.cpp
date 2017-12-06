#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <memory.h>

#include "LinkSync.h"



namespace Mqtt{

CLinkSync::CLinkSync()
{

}

CLinkSync::~CLinkSync()
{

}
	
int CLinkSync::Create(std::string clientid, std::string username, std::string password)
{
	int rc = 0;
	std::string url;
	std::string host, port;

	m_link_params.clientid = clientid;
	m_link_params.nodelimiter = 0;
	m_link_params.delimiter = '\n';
	m_link_params.qos = 2;
	m_link_params.username = username;
	m_link_params.password = password;
	m_link_params.keepalive = 10;

	m_addr_port.GetHostAndPort(host, port);

	url.append(host.c_str());
	url.append(":");
	url.append(port.c_str());

	do {
		rc = MQTTClient_create(&m_client, url.c_str(), clientid.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
		if(rc != 0) {
			printf("mqtt client create fail!\n");
			break;
		}

		rc = MQTTClient_setCallbacks(m_client, this, NULL, MessageArrivedCallback, NULL);
	} while(0);
	
	printf("url: %s, ret value: %d\n", url.c_str(), rc);

	return rc;
}

int CLinkSync::Connect()
{
	int rc = 0;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;

	conn_opts.keepAliveInterval = 10;
	conn_opts.reliable = 0;
	conn_opts.cleansession = 1;
	conn_opts.username = m_link_params.username.c_str();
	conn_opts.password = m_link_params.password.c_str();
	ssl_opts.enableServerCertAuth = 0;
	conn_opts.ssl = &ssl_opts;

	if ((rc = MQTTClient_connect(m_client, &conn_opts)) != 0) {
		printf("Failed to connect\n");
	}

	printf("connect ret value: %d\n", rc);

	return rc;
}

int CLinkSync::Disconnect()
{
	MQTTClient_disconnect(m_client, 0);
}

int CLinkSync::Destroy()
{
	MQTTClient_destroy(&m_client);
}

int CLinkSync::Publish(std::string topic, std::string data)
{
	int rc = 0;

	rc = MQTTClient_publish(m_client, topic.c_str(), data.size(), (void *)data.c_str(), m_link_params.qos, m_link_params.retained, NULL);
	if (rc != 0) {
		Connect();
		rc = MQTTClient_publish(m_client, topic.c_str(), data.size(), (void *)data.c_str(), m_link_params.qos, m_link_params.retained, NULL);
	}

	if (m_link_params.qos > 0)
		MQTTClient_yield();

	printf("publish ret value: %d\n", rc);

	return rc;
}

int CLinkSync::Subscribe(std::string topic)
{
	int rc = 0;

	rc = MQTTClient_subscribe(m_client, topic.c_str(), m_link_params.qos);

	// while (1) {
	// 	char* topicName = NULL;
	// 	int topicLen;
	// 	MQTTClient_message* message = NULL;
		
	// 	rc = MQTTClient_receive(m_client, &topicName, &topicLen, &message, 1000);
	// 	if (message) {
			
	// 		printf("%d %s", message->payloadlen, (char*)message->payload);
			
	// 		MQTTClient_freeMessage(&message);
	// 		MQTTClient_free(topicName);
	// 	}

	// 	if (rc != 0)
	// 		Connect(opts);
	// }

	printf("subscribe ret value: %d\n", rc);

	return rc;
}

int CLinkSync::MessageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
	int rc = 0;

	printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %s\n", (char*)m->payload);

    MQTTClient_freeMessage(&m);
    MQTTClient_free(topicName);

	return rc;
}

int CLinkSync::MessageArrivedCallback(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
	int rc = 0;

	if(context != NULL) {
		CLinkSync *thiz = (CLinkSync *)context;

		rc = thiz->MessageArrived(context, topicName, topicLen, m);
	}

	return rc;
}

} // end mqtt


