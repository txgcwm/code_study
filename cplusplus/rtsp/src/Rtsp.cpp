#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>

#include "Log.h"

#include "Common.h"
#include "Rtsp.h"


using namespace std;


#define RTP_DEFAULT_PORT 5004

namespace Rtsp {

typedef struct {
	const char *token;
	int code;
} Status;

Status status[] = {
		{"Continue", 100}, 
		{"OK", 200}, 
		{"Created", 201}, 
		{"Accepted", 202}, 
		{"Non-Authoritative Information", 203}, 
		{"No Content", 204}, 
		{"Reset Content", 205}, 
		{"Partial Content", 206}, 
		{"Multiple Choices", 300}, 
		{"Moved Permanently", 301}, 
		{"Moved Temporarily", 302}, 
		{"Bad Request", 400}, 
		{"Unauthorized", 401}, 
		{"Payment Required", 402}, 
		{"Forbidden", 403}, 
		{"Not Found", 404}, 
		{"Method Not Allowed", 405}, 
		{"Not Acceptable", 406}, 
		{"Proxy Authentication Required", 407}, 
		{"Request Time-out", 408}, 
		{"Conflict", 409}, 
		{"Gone", 410}, 
		{"Length Required", 411}, 
		{"Precondition Failed", 412}, 
		{"Request Entity Too Large", 413}, 
		{"Request-URI Too Large", 414}, 
		{"Unsupported Media Type", 415}, 
		{"Bad Extension", 420}, 
		{"Invalid Parameter", 450}, 
		{"Parameter Not Understood", 451}, 
		{"Conference Not Found", 452}, 
		{"Not Enough Bandwidth", 453}, 
		{"Session Not Found", 454}, 
		{"Method Not Valid In This State", 455}, 
		{"Header Field Not Valid for Resource", 456}, 
		{"Invalid Range", 457}, 
		{"Parameter Is Read-Only", 458}, 
		{"Unsupported transport", 461}, 
		{"Internal Server Error", 500}, 
		{"Not Implemented", 501}, 
		{"Bad Gateway", 502}, 
		{"Service Unavailable", 503}, 
		{"Gateway Time-out", 504}, 
		{"RTSP Version Not Supported", 505}, 
		{"Option not supported", 551}, 
		{"Extended Error:", 911}, 
		{NULL, -1} };

static const char *GetStat(int err)
{
	int i;

	for (i = 0; status[i].code != err && status[i].code != -1; ++i);

	return status[i].token;
}

int GetRtspCseg(const char *in, int &cseq)
{
	char *p = NULL;
	char trash[255];

	/****check  CSeq****/ 
	if ((p = (char *)strstr(in, "CSeq")) == NULL) {
		/**** not find CSeq send 400 error ****/
		return -1;
	}

	if(sscanf(p, "%254s %d", trash, &cseq) != 2) {
		/**** not find CSeq value send 400 error ****/
		return -1;
	}

	INFO("CSeq: %d\n", cseq);

	return 0;
}

void SetOptionsReply(int status, int cseq, string &response)
{
	char temp[256] = {0};

	sprintf(temp, "%s %d %s\r\nCSeq: %d\r\n", RTSP_VER, status, GetStat(status), cseq);
	response.append(temp);
	response.append("Public: OPTIONS,DESCRIBE,SETUP,PLAY,PAUSE,TEARDOWN\r\n\r\n");

	return;
}

/* expects format '[rtsp://server[:port/]]filename' */
static bool ParseUrl(const char *url, char *server, int &port, char *file)
{
	bool valid = false;
	char *ptr = NULL;
	char temp[128] = "";
	
	/* copy url */
	strcpy(temp, url);
	if (strncmp(temp, "rtsp://", 7) == 0) {
	   	ptr = strtok(&temp[7], " :/\t\n");
	   	strcpy(server, ptr);

	   	ptr = strtok(&temp[strlen(server) + 7 + 1], " /\t\n");
	   	if (ptr) {
			port = atoi(ptr);
	   	} else {
			port = 554;
	   	}

		ptr = strtok(NULL, " ");
		if (ptr) {
			strcpy(file, ptr);
		} else {
			file[0] = '\0';
		}

		valid = true;
	} else {
		/* try just to extract a file name */
	}
	
	return valid;
}

/* Add supported format extensions here*/
static bool IsSupportMediaType(char *p)
{
	if (strcasecmp(p,".264") == 0) {
		return true;
	}
	
	if (strcasecmp(p,".H264") == 0) {
		return true;
	}	
	
	if (strstr(p,"h264stream")) {
		return true;
	}
	
	if (strcasecmp(p,".ps") == 0) {
		return true;
	}

	return false;
 }

static bool CheckRtspFilename(const char *filename, int &err)
{
	char *p = NULL;
	bool valid = false;
	
	do {
		if (strstr(filename, "../")) {
			/* disallow relative paths outside of current directory. */
			err = 403; /* Forbidden */	 
			break;
		}

		if (strstr(filename, "./")) {
			/* Disallow ./ */
			err = 403;	 /* Forbidden */
			break;
		}
		
		p = (char *)strrchr(filename, '.');
		if (p == NULL) {
			err = 415; /* Unsupported media type */	 
			break;
		} else {
			valid = IsSupportMediaType(p);
		}

		if (!valid) {
			err = 415;	 /* Unsupported media type */
		}
	} while(0);
	
	return valid;
}

int CheckRtspUrl(const char *in, int &err)
{
#if 0
	strcpy(rtsp[0]->host_name, server);

	/****  get media file name   ****/
	if(strstr(object,"trackID")) {
		strcpy(object, rtsp[0]->file_name);
	} else {
		if(strcmp(object, "") == 0){
			strcpy(object, rtsp[0]->file_name);
		}

		strcpy(rtsp[0]->file_name, object);
	}
#endif

	return 0;
}

char *get_SDP_user_name(char *buffer)
{
	strcpy(buffer, PACKAGE);

	return buffer;
}

float NTP_time(time_t t)
{
	return (float)t+2208988800U;
}

char *get_SDP_session_id(char *buffer)
{	 
	buffer[0] = '\0';

	sprintf(buffer, "%.0f", NTP_time(time(NULL)));

	return buffer;  
}

char *get_SDP_version(char *buffer)
{
	buffer[0] = '\0';

	sprintf(buffer, "%.0f", NTP_time(time(NULL)));

	return buffer;
}

void GetSdpInfo(const char *host_name, const char *filename, char *sdp_buff)
{
	char s[30];

	strcpy(sdp_buff, "v=0"SDP_EL);	
	strcat(sdp_buff, "o=");
	strcat(sdp_buff, get_SDP_user_name(s));
	strcat(sdp_buff," ");
	strcat(sdp_buff, get_SDP_session_id(s));
	strcat(sdp_buff," ");
	strcat(sdp_buff, get_SDP_version(s));
	strcat(sdp_buff, SDP_EL);
   	strcat(sdp_buff, "c=");
   	strcat(sdp_buff, "IN ");		/* Network type: Internet. */
   	strcat(sdp_buff, "IP4 ");		/* Address type: IP4. */
	strcat(sdp_buff, host_name);
	strcat(sdp_buff, SDP_EL);
	strcat(sdp_buff, "s=RTSP Session"SDP_EL);
	sprintf(sdp_buff + strlen(sdp_buff), "i=%s %s Streaming Server"SDP_EL, PACKAGE, VERSION);
   	sprintf(sdp_buff + strlen(sdp_buff), "u=%s"SDP_EL, filename);
   	strcat(sdp_buff, "t=0 0"SDP_EL);	
	/**** media specific ****/
	strcat(sdp_buff,"m=");
	strcat(sdp_buff,"video 0");
	strcat(sdp_buff," RTP/AVP "); /* Use UDP */

	int payload_type = 96;
	sprintf(sdp_buff + strlen(sdp_buff), "%d"SDP_EL, payload_type);
	if (payload_type >= 96) {
		/**** Dynamically defined payload ****/
		strcat(sdp_buff,"a=rtpmap:");
		sprintf(sdp_buff + strlen(sdp_buff), "%d", payload_type);
		strcat(sdp_buff," ");	
		strcat(sdp_buff,"H264/90000");
		strcat(sdp_buff, SDP_EL);
		strcat(sdp_buff,"a=fmtp:96 packetization-mode=1;profile-level-id=1EE042;sprop-parameter-sets=QuAe2gLASRA=,zjCkgA==");
		strcat(sdp_buff, SDP_EL);
		strcat(sdp_buff,"a=control:");
		sprintf(sdp_buff + strlen(sdp_buff),"rtsp://%s/%s/trackID=0", host_name, filename);
		strcat(sdp_buff, SDP_EL);
	}

	strcat(sdp_buff, SDP_EL);

	return;
}

int SetDescribeReply(const char *in, int cseq, int &err, string &response)
{
	char sdp_buff[1024];
	int port;
	char url[128];
	char object[128], server[128];

	if (!sscanf(in, " %*s %254s ", url)) {
		err = 400;	/* bad request */
		return -1;
	}

	/* Validate the URL */
	if (!ParseUrl(url, server, port, object)) {
		err = 400;	/* bad request */
		return -1;
	}

	if(!CheckRtspFilename(object, err)) {
		return -1;
	}

	INFO("url: %s, server: %s, port: %d, object: %s\n", url, server, port, object);

	// Disallow Header REQUIRE
	if (strstr(in, HDR_REQUIRE)) {
		err = 551;	/* Option not supported */
		return -1;
	}

	/* Get the description format. SDP is recomended */
	if (strstr(in, HDR_ACCEPT) != NULL) {
		if (strstr(in, "application/sdp") != NULL) {
			//descr_format = df_SDP_format;		
		} else {
			// Add here new description formats
			err = 551;	/* Option not supported */
			return -1;
		}
	}

	GetSdpInfo(server, object, sdp_buff);

	/*describe*/
	char out[2048] = {0};
	sprintf(out, "%s 200 %s\r\nCSeq: %d\r\nServer: %s/%s\r\n", RTSP_VER, (char *)GetStat(200), cseq, PACKAGE, VERSION);	
	AddTimestamp(out + strlen(out), 0);
	strcat(out, "Content-Type: application/sdp"RTSP_EL);
	sprintf(out + strlen(out), "Content-Base: rtsp://%s/%s/"RTSP_EL, server, object);
	sprintf(out + strlen(out), "Content-Length: %d"RTSP_EL, strlen(sdp_buff));
	strcat(out, RTSP_EL);
	/**** concatenate description ****/
	strcat(out, sdp_buff);

	response.append(out);

	return 0;
}

void GetResponse(int error, int cseq, string &response)
{
	char buf[128] = {0};

	sprintf(buf, "%s %d %s"RTSP_EL"CSeq: %d"RTSP_EL, RTSP_VER, error, 
		(char *)GetStat(error), cseq);

	strcat(buf, RTSP_EL);

	response.append(buf);

	return;
}

void GetSetupInfo(const char *host_name, RtspInfo cmd_port, int cseq, string &response)
{
	char out[1028];
	char temp[30];
	char ttl[4];

	/* build a reply message */
	sprintf(out, "%s 200 %s"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, 
			RTSP_VER, (char *)GetStat(200), cseq, PACKAGE, VERSION);
	AddTimestamp(out + strlen(out), 0);
	strcat(out, "Session: ");
	sprintf(temp, "%d", 0); //session_id);
	strcat(out, temp);
	strcat(out, RTSP_EL);
	/**** unicast  ****/
	strcat(out, "Transport: RTP/AVP;unicast;client_port=");
	sprintf(temp, "%d", cmd_port.rtp_cli_port);
	strcat(out, temp);
	strcat(out, "-");
	sprintf(temp, "%d", cmd_port.rtcp_cli_port);
	strcat(out, temp);
	//sprintf(temp, ";source=%s", get_address());
	// strcat(out, host_name);
	// strcat(out, temp);
	strcat(out, ";server_port=");
	sprintf(temp, "%d", cmd_port.rtp_ser_port);
	strcat(out, temp);
	strcat(out, "-");
	sprintf(temp, "%d", cmd_port.rtcp_ser_port);
	strcat(out, temp);
	sprintf(temp, ";ssrc=%u", cmd_port.ssrc);/*xxx*/
	strcat(out, temp);
	strcat(out,";ttl=");
	sprintf(ttl,"%d",(int)DEFAULT_TTL);
	strcat(out,ttl);	
	strcat(out, RTSP_EL);
	strcat(out, RTSP_EL);

	response.append(out);

	return;
}

int SetSetupReply(const char *in, int cseq, int &err, RtspInfo &cmd_port, string &response)
{
	const char *p = NULL;
	char trash[255], line[255];

	int port;
	char url[128];
	char object[128], server[128];

	if (!sscanf(in, " %*s %254s ", url)) {
		err = 400;	/* bad request */
		return -1;
	}

	/* Validate the URL */
	if (!ParseUrl(url, server, port, object)) {
		err = 400;	/* bad request */
		return -1;
	}

	// if(!CheckRtspFilename(object, err)) {
	// 	return -1;
	// }

	INFO("url: %s, server: %s, port: %d, object: %s\n", url, server, port, object);
	
	if ((p = strstr(in, "client_port")) == NULL && strstr(in, "multicast") == NULL) {
		err = 406; /* Not Acceptable */	
		return -1;
	}

	/**** Start parsing the Transport header ****/
	if ((p = strstr(in, HDR_TRANSPORT)) == NULL) {
		err = 406;	/* Not Acceptable */
		return -1;
	}

	if (sscanf(p, "%10s%255s", trash, line) != 2) {
		err = 400;	/* Bad Request */
		return -1;
	}

	/****  get client rtp and rtcp port  ****/
	if(strstr(line, "client_port") != NULL) {
		p = strstr(line, "client_port");
		p = strstr(p, "=");
		sscanf(p + 1, "%d", &(cmd_port.rtp_cli_port));
		p = strstr(p, "-");
		sscanf(p + 1, "%d", &(cmd_port.rtcp_cli_port));
	}

	cmd_port.rtp_ser_port = (cmd_port.rtp_cli_port - RTP_DEFAULT_PORT)/2 + RTP_DEFAULT_PORT;
	cmd_port.rtcp_ser_port = cmd_port.rtp_ser_port + 1;

	cmd_port.seq = GetRanddomSeq();
	cmd_port.ssrc = Random32(0);
	cmd_port.timestamp = Random32(0);

	INFO("%d, %u, %u\n", cmd_port.seq, cmd_port.ssrc, cmd_port.timestamp);

	GetSetupInfo(server, cmd_port, cseq, response);

	return 0;
}

int SetPlayReply(const char *in, int cseq, int &err, string &response)
{
	const char *p = NULL;
	char temp[255];
	int port;
	char url[128];
	char object[128], server[128];

	if (!sscanf(in, " %*s %254s ", url)) {
		err = 400;	/* bad request */
		return -1;
	}

	/* Validate the URL */
	if (!ParseUrl(url, server, port, object)) {
		err = 400;	/* bad request */
		return -1;
	}
	
	int session_id;

	// If we get a Session hdr, then we have an aggregate control
	if ((p = strstr(in, HDR_SESSION)) != NULL) {
		if (sscanf(p, "%254s %d", temp, &session_id) != 2) {
			err = 454;	/* Session Not Found */
			return -1;
		}
	} else {
		err = 400;	/* bad request */
		return -1;
	}

	char out[256] = {0};

	/* build a reply message */
	sprintf(out, "%s 200 %s"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, 
		RTSP_VER, (char *)GetStat(200), cseq, PACKAGE, VERSION);
	AddTimestamp(out + strlen(out), 0);
	strcat(out, "Session: ");
	sprintf(temp, "%d", session_id);
	strcat(out, temp);
	strcat(out, RTSP_EL);
	strcat(out, RTSP_EL);

	response.append(out);

	return 0;
}

int SetTeardownReply(const char *in, int cseq, int &err, string &response)
{
	const char *p = NULL;
	char  trash[255];
	char temp[20];
	int session_id;

	// If we get a Session hdr, then we have an aggregate control
	if ((p = strstr(in, HDR_SESSION)) != NULL) {
		if (sscanf(p, "%254s %d", trash, &session_id) != 2) {
			err = 454;	/* Session Not Found */
			return -1;
		}
	} else {
		err = 400;	/* bad request */
		return -1;
	}
	
	sprintf(trash, "%s 200 %s"RTSP_EL"CSeq: %d"RTSP_EL"Server: %s/%s"RTSP_EL, 
		RTSP_VER, (char *)GetStat(200), cseq, PACKAGE, VERSION);
	AddTimestamp(trash + strlen(trash), 0);
	strcat(trash, "Session: ");
	sprintf(temp, "%d", session_id);
	strcat(trash, temp);
	strcat(trash, RTSP_EL);
	strcat(trash, RTSP_EL);

	response.append(trash);

	return 0;
}

} // end namespace


