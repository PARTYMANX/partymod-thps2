#include <stdint.h>
#include <string.h>

#include <thps1/level.h>	// to get the level struct.  this should probably be elsewhere...
#include <util/hash.h>

#include <util.h>

uint32_t get_current_level() {
	uint32_t *level_id = 0x005674f8;
	
	return *level_id;
}

uint32_t get_level_crc(uint32_t id) {
	SLevel *levels = 0x00538ff8;

	if (id > 0xd) {
		return 0xffffffff;
	}

	return crc32(levels[id].levelName, strlen(levels[id].levelName));
}