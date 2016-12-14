#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "rtsp/RtspServerSession.h"




int main(int argc, char **argv)
{
	Rtsp::CRtspServerSession rtsp;

	rtsp.Start("127.0.0.1", 5004);

	while(1) {
		sleep(10);
	}

	rtsp.Stop();

	return 0;
}


