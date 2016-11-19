#ifndef __SOCKET_H__
#define __SOCKET_H__

namespace Rtsp {

int CreateTcpConnect(const char *host, int port);

int CreateUdpConnect(const char *host, int port, int cliport);

} // end namespace


#endif