#ifndef FLVPARSER_H
#define FLVPARSER_H

#include <stdint.h>
#include <vector>

using namespace std;



class CFlvParser {
public:
	CFlvParser();
	virtual ~CFlvParser();

	int Parse(u_char *pBuf, int nBufSize, int &nUsedLen);
	int Stat();

	int DumpH264(const std::string &path);
	int DumpAAC(const std::string &path);
	int DumpFlv(const std::string &path);

private:
	typedef struct {
		int nVersion;
		int bHaveVideo;
		int bHaveAudio;
		int nHeadSize;

		u_char *pFlvHeader;
	} FlvHeader;

	typedef struct {
		int nType;
		int nDataSize;
		int nTimeStamp;
		int nTSEx;
		int nStreamID;

		uint32_t nTotalTS;
	} TagHeader;

	class Tag {
	public:
		Tag()
		: _pTagHeader(NULL)
		, _pTagData(NULL)
		, _pMedia(NULL)
		, _nMediaLen(0) 
		{
		} 

		void Init(TagHeader *pHeader, u_char *pBuf, int nLeftLen);

		TagHeader 	_header;
		u_char 		*_pTagHeader;
		u_char 		*_pTagData;
		u_char 		*_pMedia;
		int 		_nMediaLen;
	};

	class CVideoTag : public Tag 
	{
	public:
		CVideoTag(TagHeader *pHeader, u_char *pBuf, int nLeftLen, CFlvParser *pParser);

		int _nFrameType;
		int _nCodecID;

		int ParseH264Tag(CFlvParser *pParser);
		int ParseH264Configuration(CFlvParser *pParser, u_char *pTagData);
		int ParseNalu(CFlvParser *pParser, u_char *pTagData);
	};

	class CAudioTag : public Tag 
	{
	public:
		CAudioTag(TagHeader *pHeader, u_char *pBuf, int nLeftLen, CFlvParser *pParser);

		int _nSoundFormat;
		int _nSoundRate;
		int _nSoundSize;
		int _nSoundType;

		// aac
		static int _aacProfile;
		static int _sampleRateIndex;
		static int _channelConfig;

		int ParseAACTag(CFlvParser *pParser);
		int ParseAudioSpecificConfig(CFlvParser *pParser, u_char *pTagData);
		int ParseRawAAC(CFlvParser *pParser, u_char *pTagData);
	};

	typedef struct {
		int nMetaNum, nVideoNum, nAudioNum;
		int nMaxTimeStamp;
		int nLengthSize;
	} FlvStat;

	friend class Tag;

private:
	FlvHeader *CreateFlvHeader(u_char *pBuf);
	int DestroyFlvHeader(FlvHeader *pHeader);
	Tag *CreateTag(u_char *pBuf, int nLeftLen);
	int DestroyTag(Tag *pTag);
	int StatVideo(Tag *pTag);

	void SetAudioSpecificConfig();
	void GetAudioSpecificConfig();

private:
	FlvHeader 		*m_pFlvHeader;
	vector <Tag *>	m_vpTag;
	FlvStat 		m_sStat;

	int 			m_nNalUnitLength;
};

#endif // FLVPARSER_H
