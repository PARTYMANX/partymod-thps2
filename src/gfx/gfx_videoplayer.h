#ifndef _GFX_VIDEOPLAYER_H_
#define _GFX_VIDEOPLAYER_H_

#include <gfx/vk/gfx_vk.h>

struct videoPlayerState;

void video_streamer(void *data);
struct videoPlayerState *start_video(char *path);
void advanceVideo(struct videoPlayerState *state);
struct texture *createVideoTexture(struct videoPlayerState *state, partyRenderer *renderer);
void updateVideoTexture(struct videoPlayerState *state, partyRenderer *renderer, struct texture *tex);

#endif