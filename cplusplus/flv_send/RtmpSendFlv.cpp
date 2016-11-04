#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "rtmp.h"
#include "log.h"



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

int publish_using_packet_new()
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

	fp = fopen("test.flv", "rb");
	if (!fp) {
		RTMP_LogPrintf("Open File Error.\n");
		return -1;
	}

	rtmp = RTMP_Alloc();
	RTMP_Init(rtmp);

	rtmp->Link.timeout = 5; //set connection timeout,default 30s

	if (!RTMP_SetupURL(rtmp, "rtmp://localhost:1935/live/livestream")) {
		RTMP_Log(RTMP_LOGERROR, "SetupURL Err\n");
		RTMP_Free(rtmp);
		return -1;
	}

	//if unable,the AMF command would be 'play' instead of 'publish'
	RTMP_EnableWrite(rtmp);
	if (!RTMP_Connect(rtmp, NULL)) {
		RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
		RTMP_Free(rtmp);
		return -1;
	}

	if (!RTMP_ConnectStream(rtmp, 0)) {
		RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		return -1;
	}

	packet = (RTMPPacket *)malloc(sizeof(RTMPPacket));

	RTMPPacket_Alloc(packet, 1024 * 64);
	RTMPPacket_Reset(packet);

	packet->m_hasAbsTimestamp = 0;
	packet->m_nChannel = 0x04;
	packet->m_nInfoField2 = rtmp->m_stream_id;

	RTMP_LogPrintf("Start to send data ...\n");

	fseek(fp, 9, SEEK_SET); //jump over FLV Header

	start_time = RTMP_GetTime();

	while (1) {
		now_time = RTMP_GetTime();

		if(((now_time - start_time) < pre_frame_time) && bNextIsKey) {
			//wait for 1 sec if the send process is too fast
			//this mechanism is not very good,need some improvement
			if (pre_frame_time > lasttime) {
				RTMP_LogPrintf("TimeStamp: %8lu ms\n", pre_frame_time);
				lasttime = pre_frame_time;
			}
			usleep(1000);
			continue;
		}

		ReadFlvTagHeader(fp, tag);

		if (tag.type != 0x08 && tag.type != 0x09) {
			//jump over non audio/video frame，
			fseek(fp, tag.data_size, SEEK_CUR);
			continue;
		}

		if(fread(packet->m_body, 1, tag.data_size, fp) != tag.data_size) {
			break;
		}

		uint32_t timestamp = (tag.timestamp_ext << 24) | tag.timestamp;

		packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet->m_nTimeStamp = timestamp;
		packet->m_packetType = tag.type;
		packet->m_nBodySize = tag.data_size;
		pre_frame_time = timestamp;

		if (!RTMP_IsConnected(rtmp)) {
			RTMP_Log(RTMP_LOGERROR, "rtmp is not connect\n");
			break;
		}

		if (!RTMP_SendPacket(rtmp, packet, 0)) {
			RTMP_Log(RTMP_LOGERROR, "Send Error\n");
			break;
		}

		CheckNextTagIfKeyFrame(fp, bNextIsKey);
	}

	RTMP_LogPrintf("\nSend Data Over\n");

	if (fp) {
		fclose(fp);
	}

	if (rtmp != NULL) {
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		rtmp = NULL;
	}

	if (packet != NULL) {
		RTMPPacket_Free(packet);
		free(packet);
		packet = NULL;
	}

	return 0;
}

int publish_using_write_new()
{
	uint32_t start_time = 0;
	uint32_t now_time = 0;
	uint32_t pre_frame_time = 0;
	uint32_t lasttime = 0;
	int bNextIsKey = 0;
	char *pFileBuf = NULL;
	FlvTagHeader tag;
	RTMP *rtmp = NULL;
	FILE *fp = NULL;
	fp = fopen("test.flv", "rb");
	if (!fp) {
		RTMP_LogPrintf("Open File Error.\n");
		return -1;
	}

	rtmp = RTMP_Alloc();
	RTMP_Init(rtmp);

	rtmp->Link.timeout = 5; //set connection timeout,default 30s

	if (!RTMP_SetupURL(rtmp, "rtmp://localhost:1935/live/livestream")) {
		RTMP_Log(RTMP_LOGERROR, "SetupURL Err\n");
		RTMP_Free(rtmp);
		return -1;
	}

	RTMP_EnableWrite(rtmp);

	RTMP_SetBufferMS(rtmp, 3600 * 1000); //1hour

	if (!RTMP_Connect(rtmp, NULL)) {
		RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
		RTMP_Free(rtmp);
		return -1;
	}

	if (!RTMP_ConnectStream(rtmp, 0)) {
		RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		return -1;
	}

	printf("Start to send data ...\n");

	fseek(fp, 9, SEEK_SET); //jump over FLV Header

	start_time = RTMP_GetTime();

	while (1) {
		now_time = RTMP_GetTime();

		if (((now_time - start_time) < pre_frame_time) && bNextIsKey) {
			//wait for 1 sec if the send process is too fast
			//this mechanism is not very good,need some improvement
			if (pre_frame_time > lasttime) {
				RTMP_LogPrintf("TimeStamp:%8lu ms\n", pre_frame_time);
				lasttime = pre_frame_time;
			}
			usleep(1000);
			continue;
		}

		ReadFlvTagHeader(fp, tag);

		fseek(fp, -11, SEEK_CUR); //jump back

		int len = 11 + tag.data_size;

		pFileBuf = (char *)malloc(len);
		memset(pFileBuf, 0, len);

		if(fread(pFileBuf, 1, len, fp) != len) {
			break;
		}

		pre_frame_time = (tag.timestamp_ext << 24) | tag.timestamp;

		if (!RTMP_IsConnected(rtmp)) {
			RTMP_Log(RTMP_LOGERROR, "rtmp is not connect\n");
			break;
		}

		if (!RTMP_Write(rtmp, pFileBuf, len)) {
			RTMP_Log(RTMP_LOGERROR, "Rtmp Write Error\n");
			break;
		}

		free(pFileBuf);
		pFileBuf = NULL;

		CheckNextTagIfKeyFrame(fp, bNextIsKey);
	}

	RTMP_LogPrintf("\nSend Data Over\n");

	if (fp) {
		fclose(fp);
	}

	if (rtmp != NULL) {
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		rtmp = NULL;
	}

	if (pFileBuf) {
		free(pFileBuf);
		pFileBuf = NULL;
	}

	return 0;
}

