#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "ILinkManager.h"



int main(int argc, char **argv)
{
	int res = 0;
	int type = 0;
	std::string clientid;

	while((res = getopt(argc, argv, "?i:t:h")) != -1) {
        switch(res) {
        case 'i':
        	clientid = optarg;
            break;
        case 't':
        	type = atoi(optarg);
        	break;

        case 'h':
        default:
            return -1;
        }
    }

	Mqtt::ILinkManager::Instance()->Start(clientid, "", "");

	if(type == 0) {
		Mqtt::ILinkManager::Instance()->Publish("mqtt", "afaaaaf");
	} else {
		Mqtt::ILinkManager::Instance()->Subscribe("mqtt");
	}
	
	Mqtt::ILinkManager::Instance()->Stop();

	return 0;
}


