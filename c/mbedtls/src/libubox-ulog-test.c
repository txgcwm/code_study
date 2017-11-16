#include <string.h>
#include <stdio.h>

#include "libubox/ulog.h"


/*
Priority Level               Description
LOG_EMERG                    An emergency situation
LOG_ALERT                    High-priority problem, such as database corruption
LOG_CRIT                     Critical error, such as hardware failure
LOG_ERR                      Errors
LOG_WARNING                  Warning
LOG_NOTICE                   Special conditions requiring attention
LOG_INFO                     Informational messages
LOG_DEBUG                    Debug messages 
*/

void log()
{
    ulog_open(ULOG_STDIO, LOG_USER, NULL);
    ulog_threshold(LOG_INFO);

    ULOG_INFO("info\n");
    ULOG_NOTE("notice\n");
    ULOG_WARN("warn\n");
    ULOG_ERR("err\n");

    ulog_close();

    return;
}

int main(int argc, char** argv)
{
    log();

    return 0;
}