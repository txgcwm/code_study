#ifndef __LIVERTMPSTREAM_H__
#define __LIVERTMPSTREAM_H__

#include <string>

#include "base/ThreadLoop.h"



enum RTMPSENDMODE {
	RTMP_SEND_MODE_PACKET,
	RTMP_SEND_MODE_WRITE
};

class CLiveRtmpStream : public Base::CThreadLoop
{
public:
	CLiveRtmpStream();
	~CLiveRtmpStream();

	void SetParams(RTMPSENDMODE mode, std::string filename, std::string url);

	bool Start();
	bool Stop();
	
	virtual void EventHandleLoop();

private:
	RTMPSENDMODE 	m_mode;
	std::string 	m_filename;
	std::string 	m_url;
};

#endif