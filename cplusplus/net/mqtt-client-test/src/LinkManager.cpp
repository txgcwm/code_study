#include "LinkSync.h"
#include "LinkManager.h"



namespace Mqtt{

ILinkManager *ILinkManager::Instance()
{
	return CLinkManager::Instance();
}

CLinkManager* CLinkManager::Instance()
{
    static CLinkManager inst;

    return &inst;
}

CLinkManager::CLinkManager()
{

}

CLinkManager::~CLinkManager()
{
	Stop();
}
	
int CLinkManager::Start(std::string clientid, std::string username, std::string password)
{
	int rc = 0;

	m_link = new CLinkSync();
	if(m_link == NULL) {
		return -1;
	}

	m_link->Create(clientid, username, password);
	m_link->Connect();

	return rc;
}

int CLinkManager::Stop()
{
	int rc = 0;

	m_link->Disconnect();
	m_link->Destroy();

	return rc;
}

int CLinkManager::Publish(std::string topic, std::string data)
{
	int rc = 0;

	rc = m_link->Publish(topic, data);

	return rc;
}

int CLinkManager::Subscribe(std::string topic)
{
	int rc = 0;

	rc = m_link->Subscribe(topic);

	return rc;
}

} // end mqtt


