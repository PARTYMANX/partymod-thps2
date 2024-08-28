#include <stdint.h>
#include <stdio.h>

#include <patch.h>

#include <gfx/vk/gfx_vk.h>
#include <gfx/gfx_global.h>

void (__stdcall **BinkClose)(void *) = 0x005152f4;
void (__stdcall **BinkDoFrame)(void *) = 0x005152fc;
void (__stdcall **BinkNextFrame)(void *) = 0x005152f0;
void (__stdcall **BinkCopyToBuffer)(void *, void *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) = 0x005152f8;
void (__stdcall **BinkGoto)(void *, int, int) = 0x005152e8;

void **pBink = 0x006a3934;
uint8_t *BinkHasVideo = 0x006a3931;
uint8_t *BinkIsPlaying = 0x006a394d;
uint32_t *BinkWidth = 0x006a36e8;
uint32_t *BinkHeight = 0x006a36ec;

struct texture *binkTex = 0x006a37e8;
uint32_t binkTexIdx = -1;
uint32_t *binkBuffer = NULL;

uint32_t __cdecl createBinkSurface() {
	uint32_t *gShellMode = 0x006a35b4;

	binkBuffer = malloc(sizeof(uint32_t) * *BinkWidth * *BinkHeight);

	binkTexIdx = createTextureEntry(renderer, *BinkWidth, *BinkHeight);

	binkTex->idx = binkTexIdx;
	binkTex->flags = 0x01000000;
	binkTex->width = *BinkWidth;
	binkTex->height = *BinkHeight;
	binkTex->buf_width = *BinkWidth;
	binkTex->buf_height = *BinkHeight;

	struct texture **pBinkTex = 0x006a365c;
	float *maxWidth = 0x006a3828;
	float *maxHeight = 0x006a382c;

	*pBinkTex = binkTex;
	*maxWidth = 1.0f;
	*maxHeight = 1.0f;

	return 1;
}

void __cdecl stopBinkMovie() {
	if (*pBink) {
		(*BinkClose)(*pBink);
		*pBink = NULL;

		destroyTextureEntry(renderer, binkTexIdx);

		free(binkBuffer);
		binkBuffer = NULL;
	}

	*BinkIsPlaying = 0;
}

uint32_t __cdecl advanceBinkMovie() {
	uint32_t *gShellMode = 0x006a35b4;

	if (!*pBink) {
		*BinkIsPlaying = 0;
		return 0;
	}

	(*BinkDoFrame)(*pBink);

	(*BinkCopyToBuffer)(*pBink, binkBuffer, sizeof(uint32_t) * *BinkWidth, *BinkHeight, 0, 0, 6);	// flags copy all and 32bit color

	for (int i = 0; i < *BinkWidth * *BinkHeight; i++) {
		// fixes opacity.  dumb.
		binkBuffer[i] |= 0xff000000;
	}

	if (!*BinkHasVideo != 0 && !isGameMinimized()) {
		updateTextureEntry(renderer, binkTexIdx, *BinkWidth, *BinkHeight, binkBuffer);
		renderImageFrame(renderer, binkTexIdx);
	}

	uint8_t *pBink8 = *pBink;
	if (*((uint32_t *)(pBink8 + 0xc)) != *((uint32_t *)(pBink8 + 0x8))) {
		(*BinkNextFrame)(*pBink);
		return 1;
	}

	if (!*BinkIsPlaying) {
		return 0;
	}

	(*BinkGoto)(*pBink, 1, 0);
	return 1;
}

void __cdecl drawBinkSurface() {
	updateTextureEntry(renderer, binkTexIdx, *BinkWidth, *BinkHeight, binkBuffer);
	renderImageFrame(renderer, binkTexIdx);
}

void __cdecl setupBinkRender() {
}

void updateMovieTexture() {
	if (*pBink) {
		updateTextureEntry(renderer, binkTexIdx, *BinkWidth, *BinkHeight, binkBuffer);
	}
}

void installMoviePatches() {
	//patchJmp(0x004e3760, startBinkMovie);
	patchJmp(0x004e3f70, stopBinkMovie);
	patchJmp(0x004e3a60, advanceBinkMovie);

	patchJmp(0x004e31f0, createBinkSurface);
	patchJmp(0x004e2c50, drawBinkSurface);
	patchJmp(0x004e28f0, setupBinkRender);

	patchByte(0x004e319c, 0xeb);	// remove d3d use from movie player

	//patchJmp(0x004e1390, maybe_isBinkMovieOver); // actually i think this is controller stuff
}