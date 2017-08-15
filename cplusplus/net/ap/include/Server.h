#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>

int GetRouteInfo(int port, std::string &ssid, std::string& password, int& mode);

#endif