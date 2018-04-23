#ifndef _WAVE_HEADER_H_
#define _WAVE_HEADER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// wav头部结构体
struct wave_header {
    char riff[4];
    unsigned long fileLength;
    char wavTag[4];
    char fmt[4];
    unsigned long size;
    unsigned short formatTag;
    unsigned short channel;
    unsigned long sampleRate;
    unsigned long bytePerSec;
    unsigned short blockAlign;
    unsigned short bitPerSample;
    char data[4];
    unsigned long dataSize;
};

void *createWaveHeader(int fileLength, short formatTag, short channel, int sampleRate, short bitPerSample);

#endif
