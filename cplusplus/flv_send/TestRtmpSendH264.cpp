#include <stdio.h>

#include "RtmpSendH264.h"



FILE *fp_send = NULL;

int read_buffer(unsigned char *buf, int buf_size)
{
	if(feof(fp_send)) {
        fseek(fp_send, 0L, SEEK_SET);
    }

	int true_size = fread(buf, 1, buf_size, fp_send);

	return true_size;
}

int main(int argc, char* argv[])
{
	fp_send = fopen("test.h264", "rb");

	RTMP264_Connect("rtmp://localhost:1935/live/livestream");

	RTMP264_Send(read_buffer);

	RTMP264_Close();

	return 0;
}


