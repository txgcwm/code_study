#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Log.h"

#include "Socket.h"
#include "RtspClientSession.h"
#include "rtsp/RtspServerSession.h"



namespace Rtsp {


CRtspServerSession::CRtspServerSession()
: Base::CThreadLoop("RtspServer")
{
}

CRtspServerSession::~CRtspServerSession()
{
}

bool CRtspServerSession::Start(const char *host, int port)
{
    m_rtspfd = CreateTcpConnect(host, port);

  	DEBUG("rtsp server(%s) listen port(%d) sucess\n", host, port);

	return StartThread();
}

bool CRtspServerSession::Stop()
{
	close(m_rtspfd);

	return StopThread();
}

void CRtspServerSession::EventHandleLoop()
{
	int clifd = 0;
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr_in);
    CRtspClientSession clientses;

	while(1) {

  		clifd = accept(m_rtspfd, (struct sockaddr *)&client, &len);
  		
  		clientses.Start(clifd);

		if(WaitForSleep(10000) < 0) {
			break;
		}
	}

	return;
}

} // end namespace


