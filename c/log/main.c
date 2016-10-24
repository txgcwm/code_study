/* 
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

int main(int argc, char *argv[])
{
	int n = 8;

	Log_Init("/tmp/httpserver.log", LOGFLAG_FILE);

	LOG_ERRORS("error !\n");
	LOG_WARNS("warn !\n");
	LOG_INFOS("info !\n");
	LOG_DEBUGS("debug !\n");
	LOG_TRACES("trace !\n");

	Log_Close();

	return 0;
}
