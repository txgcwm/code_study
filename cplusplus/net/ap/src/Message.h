#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <string>

int FormateBroadcastData(std::string& data, std::string ssid,
                            std::string password, int mode);
int ParseBroadcastData(std::string data, std::string& ssid,
                            std::string& password, int& mode);

int FormateResultData(std::string& data);
int ParseResultData(std::string data, int& result);

#endif