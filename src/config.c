#include <config.h>

#include <windows.h>

#include <stdio.h>
#include <stdint.h>

#include <global.h>

char configFile[1024];

void initConfig() {
	char configFile[1024];
	sprintf(configFile, "%s%s", executableDirectory, CONFIG_FILE_NAME);
}

int getConfigBool(char *section, char *key, int def) {
	int result = GetPrivateProfileInt(section, key, def, configFile);
	if (result) {
		return 1;
	} else {
		return 0;
	}
}

int getConfigInt(char *section, char *key, int def) {
	 return GetPrivateProfileInt(section, key, def, configFile);
}
