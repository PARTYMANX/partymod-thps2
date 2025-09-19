#ifndef _GFX_GLOBAL_H_
#define _GFX_GLOBAL_H_

#include <stdint.h>

#include <gfx/vk/gfx_vk.h>

partyRenderer *renderer;

struct palette {
	uint16_t idx;
	uint8_t slot;
	uint8_t flags;
	uint16_t usage;
	uint8_t in_vram;
	uint8_t remap_idx;
	uint32_t checksum;
	struct palette *next;
	struct palette *prev;
};

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
	struct palette *palette;

	uint32_t unk[2];
};

void updateMovieTexture();
uint8_t isGameMinimized();

#endif