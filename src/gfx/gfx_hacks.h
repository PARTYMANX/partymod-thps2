#ifndef _GFX_HACKS_H_
#define _GFX_HACKS_H_

void set_hacks_current_level(uint32_t level);
void build_pushbacks();
uint8_t should_texture_write_depth(struct texture *tex);
uint8_t should_background_write_depth();
void installGraphicsHackPatches();
void installFreecam();

#endif