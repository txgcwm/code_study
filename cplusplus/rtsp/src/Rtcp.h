#ifndef __RTCP_H__
#define __RTCP_H__

#include "base/ThreadLoop.h"



namespace Rtsp {

typedef enum {
	SR=200,
	RR=201,
	SDES=202,
	BYE=203,
	APP=204
} rtcp_pkt_type;

typedef enum {
	CNAME=1,
	NAME=2,
	EMAIL=3,
	PHONE=4,
	LOC=5,
	TOOL=6,
	NOTE=7,
	PRIV=8		
} rtcp_info;
	
typedef struct _RTCP_header {
	unsigned int count:5;		//SC oppure RC
	unsigned int padding:1;
	unsigned int version:2;
	unsigned int pt:8;
	unsigned int length:16;
} RTCP_header;

typedef struct _RTCP_header_SR {
	unsigned int ssrc;		
	unsigned int ntp_timestampH;
	unsigned int ntp_timestampL;
	unsigned int rtp_timestamp;
	unsigned int pkt_count; 		
	unsigned int octet_count;
} RTCP_header_SR;

typedef struct _RTCP_header_RR {
	unsigned int ssrc;
} RTCP_header_RR;	

typedef struct _RTCP_header_SR_report_block {
	unsigned long  ssrc;
	unsigned char fract_lost;		
	unsigned char pck_lost[3];
	unsigned int h_seq_no;
	unsigned int jitter;
	unsigned int last_SR;
	unsigned int delay_last_SR; 		
} RTCP_header_SR_report_block;	
		
typedef struct _RTCP_header_SDES {
	unsigned int ssrc;
	unsigned char attr_name;
	unsigned char len;	
	char name[];
} RTCP_header_SDES;

typedef struct _RTCP_header_BYE {
	unsigned int ssrc;
	unsigned char length;
} RTCP_header_BYE;

typedef struct {
   RTCP_header comm;
   RTCP_header_SDES sdec;
} RtcpPkt;




class CRtcp : public Base::CThreadLoop
{
public:
	CRtcp();
	~CRtcp();
	
	bool Start(int port, int cliport);
	bool Stop();

private:
	void EventHandleLoop();
	int BuildPacket(RtcpPkt &pkt);

private:
	int 		m_rtcp_fd;
	std::string m_host;
};

} // end namespace

#endif


