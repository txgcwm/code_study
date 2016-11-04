#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "converter.h"

using namespace std;

fstream g_fileIn;
char g_flvFile[260];
int g_mode = 0;
CConverter g_cnvt;
unsigned char *g_pBufferIn, *g_pBufferOut;
int g_nFileSize = 0;

int GetOneNalu(unsigned char *pBufIn, int nInSize, unsigned char *pNalu, int &nNaluSize)
{
	unsigned char *p = pBufIn;
	int nStartPos = 0, nEndPos = 0;

	while (1) {
		if (p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 1) {
			nStartPos = p - pBufIn;
			break;
		}

		p++;

		if (p - pBufIn >= nInSize - 4) {
			return 0;
		}
	}

	p++;

	while (1) {
		if (p[0] == 0 && p[1] == 0 && p[2] == 0 && p[3] == 1) {
			nEndPos = p - pBufIn;
			break;
		}

		p++;

		if (p - pBufIn >= nInSize - 4) {
			nEndPos = nInSize;
			break;
		}
	}

	nNaluSize = nEndPos - nStartPos;
	memcpy(pNalu, pBufIn + nStartPos, nNaluSize);

	return 1;
}

int IsVideojjSEI(unsigned char *pNalu, int nNaluSize)
{
	unsigned char *p = pNalu;

	if (p[3] != 1 || p[4] != 6 || p[5] != 5)
		return 0;

	p += 6;

	while (*p++ == 0xff);

	const char *szVideojjUUID = "VideojjLeonUUID";
	char *pp = (char *)p;

	for (int i = 0; i < strlen(szVideojjUUID); i++) {
		if (pp[i] != szVideojjUUID[i]) {
			return 0;
		}
	}

	return 1;
}

int GetOneAACFrame(unsigned char *pBufIn, int nInSize, unsigned char *pAACFrame, int &nAACFrameSize)
{
	unsigned char *p = pBufIn;

	if (nInSize <= 7)
		return 0;

	int nFrameSize = ((p[3] & 0x3) << 11) + (p[4] << 3) + (p[5] >> 5);
	if (nInSize < nFrameSize)
		return 0;

	nAACFrameSize = nFrameSize;
	memcpy(pAACFrame, pBufIn, nFrameSize);

	return 1;
}

int Initialize(int argc, char *argv[])
{
	g_mode = atoi(argv[1]);
	if (g_mode != 1 && g_mode != 2) {
		cout << "mode must be 1 or 2" << endl;
		return 0;
	}

	g_fileIn.open(argv[2], ios::binary | ios::in);
	if (!g_fileIn) {
		cout << argv[1] << " can not be open!\n";
		return 0;
	}

	strcpy(g_flvFile, argv[3]);

	g_fileIn.seekg(0, ios::end);
	std::streampos ps = g_fileIn.tellg();
	g_nFileSize = ps;
	g_fileIn.seekg(0, ios_base::beg);

	g_pBufferIn = new unsigned char[g_nFileSize];
	g_pBufferOut = new unsigned char[g_nFileSize];
	if (g_pBufferIn == NULL && g_pBufferOut == NULL)
		return 0;

	g_fileIn.read((char *)g_pBufferIn, g_nFileSize);
	if (g_nFileSize != g_fileIn.gcount())
		return 0;

	return 1;
}

int Release()
{
	delete g_pBufferIn;
	delete g_pBufferOut;

	g_fileIn.close();

	return 1;
}

int ConvertH264()
{
	int nOffset = 0;
	int count = 0;

	if (g_cnvt.Open(g_flvFile) == 0)
		return 0;

	unsigned int nTimeStamp = 0;
	while (1) {
		int nNaluSize = 0;
		if (GetOneNalu(g_pBufferIn + nOffset, g_nFileSize - nOffset,
					   g_pBufferOut, nNaluSize) == 0)
			break;

		g_cnvt.ConvertH264((char *)g_pBufferOut, nNaluSize, nTimeStamp);

		if (g_pBufferOut[4] != 0x67 && g_pBufferOut[4] != 0x68)
			nTimeStamp += 33;
		nOffset += nNaluSize;
		if (nOffset >= g_nFileSize - 4)
			break;
		count++;
	}
	g_cnvt.Close();

	return 1;
}

int ConvertAAC()
{
	int nOffset = 0;
	int count = 0;

	if (g_cnvt.Open(g_flvFile, 1, 0) == 0)
		return 0;

	unsigned int nTimeStamp = 0;
	while (1) {
		int nAACFrameSize = 0;
		if (GetOneAACFrame(g_pBufferIn + nOffset, g_nFileSize - nOffset,
						   g_pBufferOut, nAACFrameSize) == 0)
			break;

		printf("nAACFrameSize = %d\n", nAACFrameSize);
		g_cnvt.ConvertAAC((char *)g_pBufferOut, nAACFrameSize, nTimeStamp);

		nTimeStamp += double (1024 * 1000) / double (44100);
		nOffset += nAACFrameSize;
		if (nOffset >= g_nFileSize - 4)
			break;
		count++;
	}
	g_cnvt.Close();

	return 1;
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
		cout << "Usage:\n\t" << "converter" << " [mode] [h.264 or aac file]" <<
			" [output file]" << endl;
		cout << "\tmode = 1 is h.264 to flv" << endl;
		cout << "\tmode = 2 is aac to flv\n" << endl;
		return 0;
	}

	if (Initialize(argc, argv) == 0)
		return 0;

	if (g_mode == 1)
		ConvertH264();

	if (g_mode == 2)
		ConvertAAC();

	Release();

	return 1;
}


