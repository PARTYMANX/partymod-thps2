#ifndef _LOG_H_
#define _LOG_H_

typedef enum {
	LL_ERROR = 0,
	LL_WARN = 1,
	LL_INFO = 2,
	LL_DEBUG = 3,
	LL_TRACE = 4
} logLevel;

void configureLogging(int isDebug);
void log_printf(logLevel level, char *fmt, ...);

#endif