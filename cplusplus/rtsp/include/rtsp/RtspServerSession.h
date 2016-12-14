#ifndef __RTSPSERVERSESSION_H__
#define __RTSPSERVERSESSION_H__


#include "base/ThreadLoop.h"



namespace Rtsp {

class CRtspServerSession : public Base::CThreadLoop
{
public:
	CRtspServerSession();
	~CRtspServerSession();

	bool Start(const char *host, int port);
	bool Stop();

private:
	void EventHandleLoop();

private:
	int m_rtspfd;
	
};

} // end namespace

#endif


