#ifndef __RTSP_H__
#define __RTSP_H__

#include <string>


#define RTSP_EL 		"\r\n"
#define RTSP_VER 		"RTSP/1.0"
#define HDR_REQUIRE 	"Require"
#define HDR_ACCEPT 		"Accept"
#define PACKAGE 		"rtspd"
#define VERSION 		"1.0"
#define SDP_EL 			"\r\n"
#define HDR_TRANSPORT 	"Transport"
#define HDR_SESSION 	"Session"
#define DEFAULT_TTL 	32
#define HDR_CSEQ 		"CSeq"

namespace Rtsp {

typedef struct {
	int rtp_cli_port;
	int rtcp_cli_port;
	int rtp_ser_port;
	int rtcp_ser_port;
	unsigned int ssrc;
	unsigned int timestamp;
	// unsigned int frame_rate_step;
	short seq;
} RtspInfo;

int GetRtspCseg(const char *in, int &cseq);

void GetResponse(int error, int cseq, std::string &response);

void SetOptionsReply(int status, int cseq, std::string &response);

int SetDescribeReply(const char *in, int cseq, int &err, std::string &response);

int SetSetupReply(const char *in, int cseq, int &err, RtspInfo &cmd_port, std::string &response);

int SetPlayReply(const char *in, int cseq, int &err, std::string &response);

int SetTeardownReply(const char *in, int cseq, int &err, std::string &response);

} // end namespace

#endif


