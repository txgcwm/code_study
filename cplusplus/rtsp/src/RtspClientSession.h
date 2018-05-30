#ifndef __RTSPCLIENTSESSION_H__
#define __RTSPCLIENTSESSION_H__

#include "ThreadLoop.h"

#include "Rtsp.h"
#include "Rtp.h"
#include "Rtcp.h"



namespace Rtsp {

class CRtspClientSession : public Base::CThreadLoop
{
public:
	CRtspClientSession();
	~CRtspClientSession();
	
	bool Start(int fd);
	bool Stop();

private:
	void EventHandleLoop();

	int Options(int clifd, const char *in);
	int Describe(int clifd, const char *in);
	int Setup(int clifd, const char *in);
	int Play(int clifd, const char *in);
	int Teardown(int clifd, const char *in);

private:
	int 		m_clifd;
	CRtp 		m_rtp;
	CRtcp 		m_rtcp;
	RtspInfo 	m_info;
};

} // end namespace

#endif


