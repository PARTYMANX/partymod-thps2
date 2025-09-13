#ifndef _THPS1_GAP_H_
#define _THPS1_GAP_H_

#include <stdint.h>

struct gap {
	uint32_t maybe_type;
	uint16_t id;
	uint16_t score;
	char name[36];
};

void writeTHPS1GapData();

#endif