#include "wave_header.h"


void *createWaveHeader(int fileLength, short formatTag, short channel, int sampleRate, short bitPerSample)
{
	struct wave_header *header = malloc(sizeof(struct wave_header));

	if (header == NULL) {
		return  NULL;
	}

	// RIFF
   header->riff[0] = 'R';
   header->riff[1] = 'I';
   header->riff[2] = 'F';
   header->riff[3] = 'F';

	// file length
   header->fileLength = fileLength + (44 - 8);

	// WAVE
   header->wavTag[0] = 'W';
   header->wavTag[1] = 'A';
   header->wavTag[2] = 'V';
   header->wavTag[3] = 'E';

	// fmt
   header->fmt[0] = 'f';
   header->fmt[1] = 'm';
   header->fmt[2] = 't';
   header->fmt[3] = ' ';

   header->size = 16;
   header->formatTag = formatTag;//0x45;
   header->channel = channel;
   header->sampleRate = sampleRate;
   header->bitPerSample = bitPerSample;
   header->blockAlign = 1;//(short)(header->channel * header->bitPerSample / 8);
   header->bytePerSec = bitPerSample*sampleRate/8;//96000;//header->blockAlign * header->sampleRate;

	// data
   header->data[0] = 'd';
   header->data[1] = 'a';
   header->data[2] = 't';
   header->data[3] = 'a';

	// data size
   header->dataSize = fileLength;

	return header;
}



