#include <stdio.h>
#include <stdarg.h>

#include <config.h>
#include <global.h>
#include <log.h>

logLevel currentLevel = LL_TRACE;
FILE *logFile = NULL;
char logFilePath[1024];
int logToConsole = 0;

#define LOG_SECTION "Logging"

void configureLogging(int isDebug) {
	logToConsole = isDebug;
	currentLevel = getConfigInt(LOG_SECTION, "Level", 2);
	
	int logToFile = getConfigBool(LOG_SECTION, "LogToFile", 0);

	if (logToFile) {
		char logFilename[256];
		logFilename[0] = '\0';
		int result = getConfigString(LOG_SECTION, "LogFile", logFilename, 256);

		if (result) {
			logFilePath[0] = '\0';

			sprintf(logFilePath, "%s%s", executableDirectory, logFilename);

			logFile = fopen(logFilePath, "w");
		}
	}
};

void log_printf(logLevel level, char *fmt, ...) {
	va_list args;
	
	if (level <= currentLevel) {
		va_start(args, fmt);
		if (logToConsole) {
			vprintf(fmt, args);
		}
		
		if (logFile) {
			vfprintf(logFile, fmt, args);
		}

		va_end(args);
	}
};