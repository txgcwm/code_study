#ifndef __RTP_H__
#define __RTP_H__

#include "ThreadLoop.h"



namespace Rtsp {

/****************************************************************
 0					 1					 2					 3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X|  CC	|M| 	PT		|		sequence number 		|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|							timestamp							|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|			synchronization source (SSRC) identifier			|
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
|			 contributing source (CSRC) identifiers 			|
|							  ....								|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

****************************************************************/

#define RTP_SIZE_MAX    1460
#define RTP_HEADER_SIZE 12
#define NALU_INDIC_SIZE 4
#define NALU_HEAD_SIZE  1
#define FU_A_INDI_SIZE  1
#define FU_A_HEAD_SIZE  1

/* SINGLE_NALU_DATA_MAX = RTP_SIZE_MAX - RTP_HEADER_SIZE*/
#define SINGLE_NALU_DATA_MAX  1448

/* SLICE_NALU_DATA_MAX = RTP_SIZE_MAX - RTP_HEADER_SIZE - FU_A_INDI_SIZE
       - FU_A_HEAD_SIZE */
#define SLICE_NALU_DATA_MAX   1446

#define MIN(a,b) ( ((a)<(b)) ? (a) : (b) )

#define READ_LEN 	150000
#define SLICE_SIZE 	1448
#define SLICE_FSIZE 1435
#define DE_TIME 	3600

typedef struct {
	/* byte 0 */
	unsigned char csrc_len:4;
	unsigned char extension:1;
	unsigned char padding:1;
	unsigned char version:2;
	
	/* byte 1 */
	unsigned char payload:7;
	unsigned char marker:1;

	/* bytes 2, 3 */
	unsigned short seq_no;

	/* bytes 4-7 */
	unsigned int timestamp;

	/* bytes 8-11 */
	unsigned int ssrc;					/* stream number is used here. */
} RTP_header;

class CRtp : public Base::CThreadLoop
{
public:
	CRtp();
	~CRtp();
	
	bool Start(int port, int cliport, int ssrc, int timestamp, short seq);
	bool Stop();

private:
	void EventHandleLoop();

	int BuildRtpHeader(RTP_header *r);
	int BuildRtpNalu(unsigned char *inbuffer, int frame_size);

private:
	int m_serfd;
	int m_ssrc;
	int m_timestamp;
	short m_seq;
	unsigned char m_nalu_buffer[1448];
};

} // end namespace

#endif


