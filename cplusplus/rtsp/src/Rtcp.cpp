#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "Log.h"

#include "Socket.h"
#include "Common.h"
#include "Rtcp.h"



namespace Rtsp {

CRtcp::CRtcp()
: Base::CThreadLoop("Rtcp")
{
}

CRtcp::~CRtcp()
{
	Stop();
}

bool CRtcp::Start(int port, int cliport)
{
	m_host = "127.0.0.1";
	m_rtcp_fd = CreateUdpConnect(m_host.c_str(), port, cliport);

	return StartThread();
}

bool CRtcp::Stop()
{
	return StopThread();
}

void CRtcp::EventHandleLoop()
{
	int len = 0;
	RtcpPkt pkt;

	DEBUG("Begin__\n");

	memset(&pkt, 0, sizeof(RtcpPkt));

	len = BuildPacket(pkt);

	while(1) {
		if(write(m_rtcp_fd, &pkt, len) < 0) {
		 	ERROR("send rtcp packet error\n");
		}

		if(WaitForSleep(3000) < 0) {
			break;
		}
	}

	DEBUG("End__\n");

	return;
}

int CRtcp::BuildPacket(RtcpPkt &pkt)
{
	RTCP_header hdr;	
	RTCP_header_SDES hdr_sdes;
	unsigned int pkt_size = 0, local_ssrc;
	
	pkt.comm.version = 2;
	pkt.comm.padding = 0;
	pkt.comm.count = 1;
	pkt.comm.pt = SDES;;
	pkt_size = sizeof(RtcpPkt) + m_host.size();
	pkt.comm.length = htons((pkt_size >> 2) -1);
	local_ssrc = Random32(0);
	pkt.sdec.ssrc = htonl(local_ssrc);    
	pkt.sdec.attr_name = CNAME;	// 1=CNAME
	pkt.sdec.len = m_host.size();
	memcpy(pkt.sdec.name, m_host.c_str(), m_host.size());

	return pkt_size;
}


} // end namespace


