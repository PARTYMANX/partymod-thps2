#ifndef _VAB_H_
#define _VAB_H_

#include <stdint.h>

struct vab_entry {
    int32_t index;
    float vol;
    uint32_t tuning;
    char name[32];
    uint32_t loop;
    int32_t unk4;  // always -1?
};

struct vab_bank {
    struct vab_entry sounds[66];
};

void writeTHPS1VabData();
void writeTHPS2VabData();

#endif