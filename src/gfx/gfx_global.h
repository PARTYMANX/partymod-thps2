#ifndef _GFX_GLOBAL_H_
#define _GFX_GLOBAL_H_

#include <gfx/vk/gfx_vk.h>

partyRenderer *renderer;

struct texture {
	uint32_t idx;
	uint8_t filler1[0x4];
	uint32_t img_data;
	uint32_t tex_checksum;
	// 0x10
	uint32_t flags;
	// 0x14
	uint16_t width;
	uint16_t height;
	//0x18
	uint16_t buf_width;
	uint16_t buf_height;
	//0x1c
	uint16_t unk_width;
	uint16_t unk_height;

	// 0x20
	int *palette;

	uint32_t unk[2];
};

void updateMovieTexture();

#endif