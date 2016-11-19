#include <stdio.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/time.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#ifdef __cplusplus
};
#endif
#endif

int main(int argc, char *argv[])
{
	AVOutputFormat *ofmt = NULL;
	AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL; // Input AVFormatContext and Output AVFormatContext
	AVPacket pkt;
	const char *in_filename, *out_filename;
	int ret, i;
	int videoindex = -1;
	int frame_index = 0;
	int64_t start_time = 0;
	//in_filename  = "test.mov";
	//in_filename  = "test.mkv";
	//in_filename  = "test.ts";
	//in_filename  = "test.mp4";
	//in_filename  = "test.h264";
	in_filename = "test.flv";

	out_filename = "rtmp://localhost:1935/live/livestream";	//输出 URL（Output URL）[RTMP]  
	//out_filename = "rtp://localhost:6666";//输出 URL（Output URL）[UDP]

	av_register_all();
	avformat_network_init();

	if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
		printf("Could not open input file.");
		goto end;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
		printf("Failed to retrieve input stream information");
		goto end;
	}

	for (i = 0; i < ifmt_ctx->nb_streams; i++)
		if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoindex = i;
			break;
		}

	av_dump_format(ifmt_ctx, 0, in_filename, 0);

	avformat_alloc_output_context2(&ofmt_ctx, NULL, "flv", out_filename);	//RTMP
	//avformat_alloc_output_context2(&ofmt_ctx, NULL, "mpegts", out_filename);//UDP

	if (!ofmt_ctx) {
		printf("Could not create output context\n");
		ret = AVERROR_UNKNOWN;
		goto end;
	}

	ofmt = ofmt_ctx->oformat;
	for (i = 0; i < ifmt_ctx->nb_streams; i++) {
		//根据输入流创建输出流（Create output AVStream according to input AVStream）
		AVStream *in_stream = ifmt_ctx->streams[i];
		AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
		if (!out_stream) {
			printf("Failed allocating output stream\n");
			ret = AVERROR_UNKNOWN;
			goto end;
		}

		//复制AVCodecContext的设置（Copy the settings of AVCodecContext）
		ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
		if (ret < 0) {
			printf("Failed to copy context from input to output stream codec context\n");
			goto end;
		}

		out_stream->codec->codec_tag = 0;
		if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
			out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}

	//Dump Format------------------
	av_dump_format(ofmt_ctx, 0, out_filename, 1);
	//打开输出URL（Open output URL）
	if (!(ofmt->flags & AVFMT_NOFILE)) {
		ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
		if (ret < 0) {
			printf("Could not open output URL '%s'", out_filename);
			goto end;
		}
	}

	//写文件头（Write file header）
	ret = avformat_write_header(ofmt_ctx, NULL);
	if (ret < 0) {
		printf("Error occurred when opening output URL\n");
		goto end;
	}

	start_time = av_gettime();

	while (1) {
		AVStream *in_stream, *out_stream;
		//获取一个AVPacket（Get an AVPacket）
		ret = av_read_frame(ifmt_ctx, &pkt);
		if (ret < 0) {
			break;
		}

		//FIX：No PTS (Example: Raw H.264)
		//Simple Write PTS
		if (pkt.pts == AV_NOPTS_VALUE) {
			//Write PTS
			AVRational time_base1 = ifmt_ctx->streams[videoindex]->time_base;
			//Duration between 2 frames (us)
			int64_t calc_duration = (double)AV_TIME_BASE /
				av_q2d(ifmt_ctx->streams[videoindex]->r_frame_rate);
			//Parameters
			pkt.pts = (double)(frame_index * calc_duration) /
				(double)(av_q2d(time_base1) * AV_TIME_BASE);
			pkt.dts = pkt.pts;
			pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1) *
												 AV_TIME_BASE);
		}

		//Important:Delay
		if (pkt.stream_index == videoindex) {
			AVRational time_base = ifmt_ctx->streams[videoindex]->time_base;
			AVRational time_base_q = { 1, AV_TIME_BASE };
			int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
			int64_t now_time = av_gettime() - start_time;
			if (pts_time > now_time) {
				av_usleep(pts_time - now_time);
			}
		}

		in_stream = ifmt_ctx->streams[pkt.stream_index];
		out_stream = ofmt_ctx->streams[pkt.stream_index];
		/* copy packet */
		//转换PTS/DTS（Convert PTS/DTS）
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base,
							 out_stream->time_base,
							 (AVRounding) (AV_ROUND_NEAR_INF |
										   AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base,
							 out_stream->time_base,
							 (AVRounding) (AV_ROUND_NEAR_INF |
										   AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base,
						 out_stream->time_base);
		pkt.pos = -1;
		//Print to Screen
		if (pkt.stream_index == videoindex) {
			printf("Send %8d video frames to output URL\n", frame_index);
			frame_index++;
		}

		//ret = av_write_frame(ofmt_ctx, &pkt);
		ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
		if (ret < 0) {
			printf("Error muxing packet\n");
			break;
		}

		av_free_packet(&pkt);
	}

	//写文件尾（Write file trailer）
	av_write_trailer(ofmt_ctx);

end:
	avformat_close_input(&ifmt_ctx);
	/* close output */
	if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
		avio_close(ofmt_ctx->pb);

	avformat_free_context(ofmt_ctx);

	if (ret < 0 && ret != AVERROR_EOF) {
		printf("Error occurred.\n");
		return -1;
	}

	return 0;
}

// g++ ffmpeg_stream.c -o ffmpeg_stream -I /usr/local/include -L /usr/local/lib -lavformat -lavcodec -lavutil -lswresample -llzma -lX11 -lvdpau -lpthread -ldl -lm -lz -lrt


