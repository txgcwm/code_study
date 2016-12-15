#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "log.h"
#include "rtmp.h"



int main(int argc, char* argv[])
{
	int nRead;
	bool bLiveStream = true;
	int bufsize = 1024*1024*10;
	char *buf = (char*)malloc(bufsize);
	long countbufsize = 0;

	memset(buf, 0, bufsize);

	FILE *fp = fopen("receive.flv", "wb");
	if (!fp) {
		RTMP_LogPrintf("Open File Error.\n");
		return -1;
	}

	RTMP *rtmp = RTMP_Alloc();
	RTMP_Init(rtmp);
	
	rtmp->Link.timeout = 10; //set connection timeout,default 30s

	if(!RTMP_SetupURL(rtmp, "rtmp://live.hkstv.hk.lxdns.com/live/hks")) {
		RTMP_Log(RTMP_LOGERROR,"SetupURL Err\n");
		RTMP_Free(rtmp);
		return -1;
	}

	if (bLiveStream) {
		rtmp->Link.lFlags |= RTMP_LF_LIVE;
	}

	RTMP_SetBufferMS(rtmp, 3600*1000); // 1hour

	if(!RTMP_Connect(rtmp, NULL)) {
		RTMP_Log(RTMP_LOGERROR,"Connect Err\n");
		RTMP_Free(rtmp);
		return -1;
	}

	if(!RTMP_ConnectStream(rtmp, 0)) {
		RTMP_Log(RTMP_LOGERROR,"ConnectStream Err\n");
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		return -1;
	}

	while((nRead = RTMP_Read(rtmp, buf, bufsize)) > 0) {
		fwrite(buf, 1, nRead, fp);
		countbufsize += nRead;
		RTMP_LogPrintf("Receive: %5d Byte, Total: %5.2f kB\n", nRead, countbufsize*1.0/1024);
	}

	if(fp) {
		fclose(fp);
	}

	if(buf) {
		free(buf);
	}

	if(rtmp) {
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		rtmp = NULL;
	}

	return 0;
}


