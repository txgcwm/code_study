#ifndef __CLIENT_H__
#define __CLIENT_H__

int SendBroadcastData(int& result, char* ifname, int port,
                        char* ssid, char* password, int mode);

#endif