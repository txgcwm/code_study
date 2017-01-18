#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <string>
#include <list>
#include <map>

using namespace std;

typedef struct {
	char ip[20];
	int  port;
} Channel;

typedef list <Channel> ChannelList;

typedef struct {
	char name[10];
	ChannelList clist;
} DeviceInfo;

typedef map <string, DeviceInfo> DeviceInfoMap;

#define DEVNUM 		8
#define CHANNELNUM 	3



void PrintDeviceInfoMap(DeviceInfoMap devInfoMap)
{
	DeviceInfoMap::iterator iter;

	for (iter = devInfoMap.begin(); iter != devInfoMap.end(); iter++) {
		printf("\nname: %s\n", (*iter).first.c_str());
		
		DeviceInfo devInfo = (DeviceInfo)(*iter).second;
		
		ChannelList::iterator citer;

		for(citer = devInfo.clist.begin(); citer != devInfo.clist.end(); citer++) {
			Channel chl = (Channel)*citer;
			printf("ip: %s, port: %d\n", chl.ip, chl.port);
		}
	}

	return;
}

void PrintDeviceInfo(DeviceInfo devInfo)
{
	ChannelList::iterator citer;

	printf("\nfind device: %s\n", devInfo.name);

	for(citer = devInfo.clist.begin(); citer != devInfo.clist.end(); citer++) {
		Channel chl = (Channel)*citer;
		printf("ip: %s, port: %d\n", chl.ip, chl.port);
	}

	return;
}

void InitDeviceInfo(DeviceInfoMap &devInfoMap)
{
	int i = 0, j = 0;

	for(i = 0; i < DEVNUM; i++) {
		char name[10] = {0};
		DeviceInfo devInfo;

		snprintf(name, sizeof(name), "dev%d", i);
		strncpy(devInfo.name, name, sizeof(name));

		for(j = 0; j < CHANNELNUM; j++) {
			char ip[20] = {0};
			Channel channel;

			snprintf(ip, sizeof(ip), "%d2:34:fb:c5:84:0e", j);
			strncpy(channel.ip, ip, sizeof(ip));
			channel.port = 5847 + i + j;
			devInfo.clist.push_front(channel);
		}

		devInfoMap.insert(DeviceInfoMap::value_type(name, devInfo));
	}

	return;
}

void UnInitDeviceInfo(DeviceInfoMap &devInfoMap)
{
	for(DeviceInfoMap::iterator it = devInfoMap.begin(); it != devInfoMap.end(); ) {
		it->second.clist.clear();  // clear channel list

		devInfoMap.erase(it++);
	}

	return;
}

int main(int argc, char **argv)
{
	DeviceInfoMap devInfoMap;

	InitDeviceInfo(devInfoMap);

	PrintDeviceInfoMap(devInfoMap);

	DeviceInfoMap::iterator iter = devInfoMap.find("dev6");
	if(iter != devInfoMap.end()) {
		PrintDeviceInfo(iter->second);
    } else {
        printf("Not found\n");
    }

    UnInitDeviceInfo(devInfoMap);

    PrintDeviceInfoMap(devInfoMap); // printf nothing
	
	return 0;
}


