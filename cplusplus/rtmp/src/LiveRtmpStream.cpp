#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <string>

#include "rtmp.h"

#include "base/Log.h"

#include "LiveRtmpStream.h"

using namespace std;



enum TAGTYPE {
    TAGTYPE_AUDIODATA        = 0x08,
    TAGTYPE_VIDEODATA        = 0x09,
    TAGTYPE_SCRIPTDATAOBJECT = 0x12,
};

typedef struct {
    uint8_t		type;
    uint32_t 	data_size;
    uint32_t 	timestamp;
    uint8_t 	timestamp_ext;
    uint32_t 	stream_id;
} FlvTagHeader;

typedef struct {
    uint8_t frame_type;
    uint8_t codec_id;
} VideoTag;

bool ReadFlvTagHeader(FILE *fp, FlvTagHeader &tag)
{
	u_char buf[11] = {0};

	fseek(fp, 4, SEEK_CUR); //jump over previousTagSize

	size_t size = fread(buf, 1, 11, fp);

	memset(&tag, 0, sizeof(FlvTagHeader));

	tag.type = buf[0];
	tag.data_size = (buf[1] << 16) | (buf[2] << 8) | buf[3];
	tag.timestamp = (buf[4] << 16) | (buf[5] << 8) | buf[6];
	tag.timestamp_ext = buf[7];
	tag.stream_id = (buf[8] << 16) | (buf[9] << 8) | buf[10];

	return true;
}

void CheckNextTagIfKeyFrame(FILE *fp, int &key)
{
	FlvTagHeader tag;

	ReadFlvTagHeader(fp, tag);

	if(tag.type == 0x09) {
		uint8_t frame_type = 0;

		fread((void *)&frame_type, 1, 1, fp);

		if(frame_type == 0x17) {
			key = 1;
		} else {
			key = 0;
		}

		fseek(fp, -16, SEEK_CUR);
	} else {
		fseek(fp, -15, SEEK_CUR);
	}

	return;
}

bool CreateRtmp(RTMPSENDMODE mode, string url, RTMP **pprtmp, RTMPPacket **pppacket)
{
	RTMP *rtmp = NULL;
	RTMPPacket *packet = NULL;

	rtmp = RTMP_Alloc();
	RTMP_Init(rtmp);

	rtmp->Link.timeout = 5;

	if(!RTMP_SetupURL(rtmp, (char *)url.c_str())) {
		RTMP_Free(rtmp);
		return false;
	}

	RTMP_EnableWrite(rtmp);

	if(!RTMP_Connect(rtmp, NULL)) {
		RTMP_Free(rtmp);
		return false;
	}

	if(!RTMP_ConnectStream(rtmp, 0)) {
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		return false;
	}

	if(mode == RTMP_SEND_MODE_PACKET) {
		packet = (RTMPPacket *)malloc(sizeof(RTMPPacket));

		RTMPPacket_Alloc(packet, 1024 * 64);
		RTMPPacket_Reset(packet);

		packet->m_hasAbsTimestamp = 0;
		packet->m_nChannel = 0x04;
		packet->m_nInfoField2 = rtmp->m_stream_id;
	} else if (mode == RTMP_SEND_MODE_WRITE) {
		RTMP_SetBufferMS(rtmp, 3600 * 1000); // 1hour
	}
	
	*pprtmp = rtmp;
	*pppacket = packet;

	return true;
}

bool DestroyRtmp(RTMP **pprtmp, RTMPPacket **pppacket)
{
	RTMP *rtmp = *pprtmp;
	RTMPPacket *packet = *pppacket;

	if (rtmp != NULL) {
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		*pprtmp = NULL;
	}

	if (packet != NULL) {
		RTMPPacket_Free(packet);
		free(packet);
		*pppacket = NULL;
	}

	return true;
}

bool RtmpSendPacket(FILE *fp, FlvTagHeader tag, RTMP *rtmp, RTMPPacket *packet)
{
	bool res = false;

	do {
		if(fread(packet->m_body, 1, tag.data_size, fp) != tag.data_size) {
			break;
		}

		packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet->m_nTimeStamp = (tag.timestamp_ext << 24) | tag.timestamp;
		packet->m_packetType = tag.type;
		packet->m_nBodySize = tag.data_size;

		if (!RTMP_IsConnected(rtmp)) {
			break;
		}

		if (!RTMP_SendPacket(rtmp, packet, 0)) {
			break;
		}

		res = true;
	} while(0);

	return res;
}

bool RtmpWrite(FILE *fp, FlvTagHeader tag, RTMP *rtmp)
{
	bool res = false;
	int len = 11 + tag.data_size;

	fseek(fp, -11, SEEK_CUR); //jump back

	char *pFileBuf = (char *)malloc(len);
	if(pFileBuf == NULL) {
		return res;
	}

	memset(pFileBuf, 0, len);

	do {
		if(fread(pFileBuf, 1, len, fp) != len) {
			break;
		}

		if (!RTMP_IsConnected(rtmp)) {
			break;
		}

		if (!RTMP_Write(rtmp, pFileBuf, len)) {
			break;
		}

		res = true;
	} while(0);

		
	if(pFileBuf != NULL) {
		free(pFileBuf);
		pFileBuf = NULL;
	}
	
	return res;
}

/***********************************************************************************/

CLiveRtmpStream::CLiveRtmpStream()
: Base::CThreadLoop("LiveRtmpStream")
, m_mode(RTMP_SEND_MODE_PACKET)
, m_filename("")
, m_url("")
{
}

CLiveRtmpStream::~CLiveRtmpStream()
{
}

void CLiveRtmpStream::SetParams(RTMPSENDMODE mode, string filename, string url)
{
	m_mode = mode;
	m_filename = filename;
	m_url = url;

	INFO("send mode(%d), filename(%s), url(%s)\n", 
			mode, filename.c_str(), url.c_str());

	return;
}

bool CLiveRtmpStream::Start()
{
	DEBUG("\n");

	return StartThread();
}

bool CLiveRtmpStream::Stop()
{
	DEBUG("\n");
	
	return StopThread();
}

void CLiveRtmpStream::EventHandleLoop()
{
	RTMP *rtmp = NULL;
	RTMPPacket *packet = NULL;
	uint32_t start_time = 0;
	uint32_t now_time = 0;
	long pre_frame_time = 0; //the timestamp of the previous frame
	long lasttime = 0;
	int bNextIsKey = 1;
	FlvTagHeader tag;
	FILE *fp = NULL;

	DEBUG("Begin___\n");

	fp = fopen(m_filename.c_str(), "rb");
	if (!fp) {
		DEBUG("Open File Error.\n");
		return;
	}

	CreateRtmp(m_mode, m_url, &rtmp, &packet);

	fseek(fp, 9, SEEK_SET); //jump over FLV Header

	start_time = RTMP_GetTime();

	while(1) {
		now_time = RTMP_GetTime();

		lasttime = now_time - start_time;

		if((lasttime < pre_frame_time) && bNextIsKey) {
			long sleep_time = pre_frame_time - lasttime;
			DEBUG("sleep %d ms\n", sleep_time);
			usleep(sleep_time * 1000);
			continue;
		}

		ReadFlvTagHeader(fp, tag);

		if (tag.type != 0x08 && tag.type != 0x09) {
			//jump over non audio/video frame，
			fseek(fp, tag.data_size, SEEK_CUR);
			continue;
		}

		if(m_mode == RTMP_SEND_MODE_PACKET) {
			if(!RtmpSendPacket(fp, tag, rtmp, packet)) {
				break;
			}
		} else if (m_mode == RTMP_SEND_MODE_WRITE) {
			if(!RtmpWrite(fp, tag, rtmp)) {
				break;
			}
		}

		pre_frame_time = (tag.timestamp_ext << 24) | tag.timestamp;

		CheckNextTagIfKeyFrame(fp, bNextIsKey);

		if(WaitForSleep(20) < 0) {
			break;
		}
	}

	if (fp) {
		fclose(fp);
	}

	DestroyRtmp(&rtmp, &packet);

	DEBUG("End___\n");

	return;
}


