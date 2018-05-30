#ifndef __LOG_H__
#define __LOG_H__

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

namespace Base {

enum LOGLEVEL {
	LOG_DEBUG,
	LOG_INFO,
	LOG_NOTICE,
	LOG_WARN,
	LOG_ERROR,
	LOG_CRIT,
	LOG_ALERT,
	LOG_EMERG
};

void LogInit(const char *log_file, LOGLEVEL level);
void LogUninit();
int LogCheckLevel(LOGLEVEL level);
void LogPrint(LOGLEVEL level, const char *prefix, const char *fmt, ...);

} // end namespace Base


#define LOG_BUFFER_LEN	256
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define LOG(level, fmt, args...)	\
	do {	\
		if(Base::LogCheckLevel(level) == 0)	\
			break;	\
		char buf[LOG_BUFFER_LEN] = {0};	\
		snprintf(buf, LOG_BUFFER_LEN, "pid(%d) %s %s(%d)", \
					getpid(), __FILENAME__, __FUNCTION__, __LINE__);	\
		Base::LogPrint(level, buf, fmt, ##args);	\
	} while(0)

#define DEBUG(fmt, args...)		LOG(Base::LOG_DEBUG, fmt, ##args)
#define INFO(fmt, args...)		LOG(Base::LOG_INFO, fmt, ##args)
#define NOTICE(fmt, args...)	LOG(Base::LOG_NOTICE, fmt, ##args)
#define WARN(fmt, args...)		LOG(Base::LOG_WARN, fmt, ##args)
#define ERROR(fmt, args...)		LOG(Base::LOG_ERROR, fmt, ##args)
#define CRIT(fmt, args...)		LOG(Base::LOG_CRIT, fmt, ##args)
#define ALERT(fmt, args...)		LOG(Base::LOG_ALERT, fmt, ##args)
#define EMERG(fmt, args...)		LOG(Base::LOG_EMERG, fmt, ##args)

#endif


