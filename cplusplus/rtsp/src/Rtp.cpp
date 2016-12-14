#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "Common.h"
#include "Socket.h"
#include "Rtp.h"



namespace Rtsp {

int abstr_nalu_indic(unsigned char *buf, int buf_size, int *be_found)
{
    unsigned char *p_tmp;
	int offset;
	int frame_size;
	
	*be_found = 0;
	offset = 0;
	frame_size = 4;	
	p_tmp = buf + 4;
	
	while(frame_size < buf_size - 4) {
	    if(p_tmp[2]) {
			offset = 3;
	    } else if(p_tmp[1]) {
			offset = 2;
		} else if(p_tmp[0]) {
			offset = 1;
		} else {
		    if(p_tmp[3] != 1) {
		        if(p_tmp[3]) {
					offset = 4;
		        } else {
					offset = 1;
				}
		    } else {
			    *be_found = 1;
				break;
			}
		}

		frame_size += offset;
		p_tmp += offset;
	}

	if(!*be_found) {
		frame_size = buf_size;
	}

	return frame_size;
}

int CRtp::BuildRtpHeader(RTP_header *r)
{	
	r->version = 2;
	r->padding = 0;
	r->extension = 0;
	r->csrc_len = 0;
	r->marker = 0;
	r->payload = 96;
	r->seq_no = htons(m_seq);

	int frame_rate_step = 3600;

	m_timestamp += frame_rate_step;
	r->timestamp = htonl(m_timestamp);
	r->ssrc = htonl(m_ssrc);

	return 0;
}

/*******************************************************************************
 * RTP Packet:
 * 1. NALU length small than 1460-sizeof(RTP header):
 *    (RTP Header) + (NALU without Start Code)
 * 2. NALU length larger than MTU:
 *    (RTP Header) + (FU Indicator) + (FU Header) + (NALU Slice)
 *                 + (FU Indicator) + (FU Header) + (NALU Slice)
 *                 + ...
 *
 * inbuffer--NALU: 00 00 00 01      1 Byte     XX XX XX
 *                | Start Code| |NALU Header| |NALU Data|
 *
 * NALU Slice: Cut NALU Data into Slice.
 *
 * NALU Header: F|NRI|TYPE
 *              F: 1 bit,
 *              NRI: 2 bits,
 *              Type: 5 bits
 *
 * FU Indicator: Like NALU Header, Type is FU-A(28)
 * 
 * FU Header: S|E|R|Type
 *            S: 1 bit, Start, First Slice should set
 *            E: 1 bit, End, Last Slice should set
 *            R: 1 bit, Reserved
 *            Type: 5 bits, Same with NALU Header's Type.
 ******************************************************************************/
int CRtp::BuildRtpNalu(unsigned char *inbuffer, int frame_size)
{
	unsigned char nalu_header;
	unsigned char fu_indic;
	unsigned char fu_header;	
	unsigned char *p_nalu_data;
	unsigned char *buffer = m_nalu_buffer;
	int time_delay;
	int data_left;
	int fu_start = 1;
	int fu_end   = 0;
	RTP_header rtp_header;
	
	BuildRtpHeader(&rtp_header);
	
	data_left   = frame_size - NALU_INDIC_SIZE;
	p_nalu_data = inbuffer + NALU_INDIC_SIZE;

	//Single RTP Packet.
    if(data_left <= SINGLE_NALU_DATA_MAX) {
	    rtp_header.seq_no = htons(m_seq++);
	    rtp_header.marker = 1;    
		memcpy(buffer, &rtp_header, sizeof(rtp_header));
        memcpy(buffer + RTP_HEADER_SIZE, p_nalu_data, data_left);
        write(m_serfd, m_nalu_buffer, data_left + RTP_HEADER_SIZE);
		usleep(DE_TIME);
        return 0;
    }

	//FU-A RTP Packet.
	nalu_header = inbuffer[4];
	fu_indic    = (nalu_header&0xE0)|28;	
	data_left   -= NALU_HEAD_SIZE;
	p_nalu_data += NALU_HEAD_SIZE;

	while(data_left > 0) {
	    int proc_size = MIN(data_left, SLICE_NALU_DATA_MAX);
		int rtp_size = proc_size + RTP_HEADER_SIZE + FU_A_HEAD_SIZE + FU_A_INDI_SIZE;
		fu_end = (proc_size == data_left);

		fu_header = nalu_header & 0x1F;
		if(fu_start) {
			fu_header |= 0x80;
		} else if(fu_end) {
			fu_header |= 0x40;
		}

        rtp_header.seq_no = htons(m_seq++);
		memcpy(buffer, &rtp_header, sizeof(rtp_header));
		memcpy(buffer + 14, p_nalu_data, proc_size);
		buffer[12] = fu_indic;
		buffer[13] = fu_header;
		write(m_serfd, m_nalu_buffer, rtp_size);
		if(fu_end) {
			usleep(36000);
		}
		
		data_left -= proc_size;	
		p_nalu_data += proc_size;
		fu_start = 0;
	}

	return 0;	
}

CRtp::CRtp()
: Base::CThreadLoop("Rtp")
{
}
	
CRtp::~CRtp()
{
	Stop();
}
	
bool CRtp::Start(int port, int cliport, int ssrc, int timestamp, short seq)
{
	m_serfd = CreateUdpConnect("127.0.0.1", port, cliport);

	m_ssrc = ssrc;
	m_timestamp = timestamp;
	m_seq = seq;

	return StartThread();
}

bool CRtp::Stop()
{
	close(m_serfd);

	return StopThread();
}

void CRtp::EventHandleLoop()
{
	FILE *infile = NULL;
	int total_size = 0, bytes_consumed = 0, frame_size = 0, bytes_left;
	unsigned char inbufs[READ_LEN] = "", outbufs[READ_LEN] = "";;
    unsigned char *p_tmp = NULL;
	int found_nalu = 0;
	int reach_last_nalu = 0;

	infile = fopen("1.h264", "rb");
	if(infile == NULL) {
		printf("please check media file\n");
		return;
	}
	
	total_size = GetFileSize(infile);
	if(total_size <= 4) {
		fclose(infile);
	    return;	
	}

	while(1) {
		bytes_left = fread(inbufs, 1, READ_LEN, infile);
		p_tmp = inbufs;

		while(bytes_left > 0) {
		    frame_size = abstr_nalu_indic(p_tmp, bytes_left, &found_nalu);
			reach_last_nalu = (bytes_consumed + frame_size >= total_size); 

			if(found_nalu || reach_last_nalu) {	      
			    memcpy(outbufs, p_tmp, frame_size);

				BuildRtpNalu(outbufs, frame_size);		 
				p_tmp += frame_size;
				bytes_consumed += frame_size;

				// if(reach_last_nalu)
				//  	rtsp[cur_conn_num]->is_runing = 0;
			}

			bytes_left -= frame_size;
		}
	 
	    fseek(infile, bytes_consumed, SEEK_SET);

		if(WaitForSleep(10) < 0) {
			break;
		}
	}

	fclose(infile);
	close(m_serfd);

	return;
}

} // end namespace


