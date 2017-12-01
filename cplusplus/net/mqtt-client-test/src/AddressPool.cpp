#include "AddressPool.h"



namespace Mqtt{


CAddressPool::CAddressPool()
{

}

CAddressPool::~CAddressPool()
{

}

int CAddressPool::GetHostAndPort(std::string &host, std::string &port)
{
	host = "localhost";
	port = "1883";
	
	return 0;
}

} // end mqtt


