#ifndef __ADDRESSPOOL_H__
#define __ADDRESSPOOL_H__

#include <string>



namespace Mqtt {

class CAddressPool
{
public:
	CAddressPool();
	~CAddressPool();

	int GetHostAndPort(std::string &host, std::string &port);
	
};
	
}

#endif


