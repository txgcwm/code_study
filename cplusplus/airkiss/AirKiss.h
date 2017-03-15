#ifndef __AIRKISS_H__
#define __AIRKISS_H__

#include <string>

#include "ThreadLoop.h"



namespace Slink {

class CAirKiss : public CThreadLoop
{
public:
	CAirKiss();
	~CAirKiss();

	bool SetRouteInfo(std::string ssid, std::string password);
	bool Reset();

	virtual void EventHandleLoop();

private:
	void AppendEncodedData(int length);
	void LeadingPart();
	void MagicCode(std::string ssid, std::string password);
	void PrefixCode(std::string password);
	void Sequence(int index, char *data, int len);

private:
	int *m_encodedData;
	int m_length;
};

}  // end namespace

#endif