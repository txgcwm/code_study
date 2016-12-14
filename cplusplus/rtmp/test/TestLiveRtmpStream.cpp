#include <unistd.h>

#include "LiveRtmpStream.h"



int main(int argc, char **argv) 
{
	CLiveRtmpStream stream;

	// stream.SetParams(RTMP_SEND_MODE_PACKET, "test.flv", "rtmp://localhost:1935/live/livestream");
	stream.SetParams(RTMP_SEND_MODE_WRITE, "test.flv", "rtmp://localhost:1935/live/livestream");

	stream.Start();

	while(1) {
		sleep(5);
	}

	stream.Stop();

	return 0;
}