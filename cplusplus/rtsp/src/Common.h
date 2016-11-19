#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>



namespace Rtsp {

unsigned int Random32(int type);

unsigned long GetRanddomSeq();

void AddTimestamp(char *b, int crlf);

long GetFileSize(FILE *infile);

} // end namespace


#endif