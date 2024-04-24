#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONFIG_FILE_NAME "partymod.ini"

void initConfig();
int getConfigBool(char *section, char *key, int def);
int getConfigInt(char *section, char *key, int def);

#endif