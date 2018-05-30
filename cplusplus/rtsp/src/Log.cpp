#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "Log.h"


namespace Base {

const char *log_info[] = {
	[LOG_DEBUG] = "Debug",
	[LOG_INFO] = "Info",
	[LOG_NOTICE] = "Notice",
	[LOG_WARN] = "Warn",
	[LOG_ERROR] = "Error",
	[LOG_CRIT] = "Crit",
	[LOG_ALERT] = "Alert",
	[LOG_EMERG] = "Emerg"
};

typedef struct rb_log_st {
	FILE *file;
	LOGLEVEL level;
}rb_log;

rb_log * g_rb_log = NULL;

void LogInit(const char *log_file, LOGLEVEL level)
{
	if(g_rb_log != NULL) {
		return;
	}

	g_rb_log = (rb_log *)malloc(sizeof(rb_log));
	if(g_rb_log == NULL) {
		return;
	}

	memset(g_rb_log, 0, sizeof(rb_log));

	if(log_file == NULL) {
		g_rb_log->file = stderr;
	} else {
		g_rb_log->file = fopen(log_file, "a+");
		if(g_rb_log->file == NULL) {
			g_rb_log->file = stderr;	
		}
	}

	if(level < LOG_DEBUG || level > LOG_EMERG ) {
		g_rb_log->level = LOG_ERROR;
	} else {
		g_rb_log->level = level;
	}

	return;
}

void LogUninit(void)
{
	if(g_rb_log == NULL) {
		return;
	}

	if(g_rb_log->file != stderr) {
		fclose(g_rb_log->file);
	}
	
	g_rb_log->file = NULL;

	free(g_rb_log);
	g_rb_log = NULL;

	return;
}

int LogCheckLevel(LOGLEVEL level)
{
	if(g_rb_log == NULL)
		return 1;

	return (g_rb_log->level > level ? 0:1);
}

void LogPrint(LOGLEVEL level, const char *prefix, const char *fmt, ...)
{
	FILE *fp = NULL;
	va_list args;
	time_t t_secs;
	struct tm *t_info = NULL;

	if(g_rb_log == NULL) {
		fp = stderr;
	} else {
		fp = g_rb_log->file;
	}

	time(&t_secs);
	t_info = gmtime(&t_secs);

	fprintf(fp, "[%d-%d-%d %d:%d:%d %s %s] ", t_info->tm_year + 1900, t_info->tm_mon + 1, 
				t_info->tm_mday, t_info->tm_hour, t_info->tm_min, t_info->tm_sec, prefix, log_info[level]);

	va_start(args, fmt);
	vfprintf(fp, fmt, args);
	va_end(args);

	return;
}

} // end namespace Base


