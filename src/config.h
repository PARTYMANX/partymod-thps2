#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdlib.h>

#define CONFIG_FILE_NAME "partymod.ini"

void initConfig();
int getConfigBool(char *section, char *key, int def);
int getConfigInt(char *section, char *key, int def);
int getConfigString(char *section, char *key, char *dst, size_t sz);

#endif