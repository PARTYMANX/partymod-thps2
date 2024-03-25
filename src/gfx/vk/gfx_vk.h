#ifndef _GFX_VK_H_
#define _GFX_VK_H_

// contains internal data for the vulkan driver
// contains common structs (ie renderer) for the vulkan driver

#include <stdint.h>

#include <vulkan/vulkan.h>

/*
	STRUCT
*/

typedef struct partyRenderer partyRenderer;

typedef struct {
	float x;
	float y;
	float z;
	float w;
	float u;
	float v;
	uint32_t color;
	int16_t texture;
	int16_t flags;
} renderVertex;

typedef struct {
	float x;
	float y;
	float u;
	float v;
} scalerVertex;

typedef struct {
	float width;
	float height;
} imageInfo;

/*
	FUNC
*/

uint8_t CreateVKRenderer(void *windowHandle, partyRenderer **dst);

void startRender(partyRenderer *renderer, uint32_t clearColor);
void finishRender(partyRenderer *renderer);
void drawVertices(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count);
//void drawTriangleFan(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count);
void drawLines(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count);
void setViewport(partyRenderer *renderer, float x, float y, float width, float height);
void setScissor(partyRenderer *renderer, float x, float y, float width, float height);
void setDepthState(partyRenderer *renderer, uint8_t test, uint8_t write);
void setBlendState(partyRenderer *renderer, uint32_t blendState);
void setRenderResolution(partyRenderer *renderer, uint32_t width, uint32_t height, float aspectRatio);
uint32_t createTextureEntry(partyRenderer *renderer, uint32_t width, uint32_t height);
void updateTextureEntry(partyRenderer *renderer, uint32_t idx, uint32_t width, uint32_t height, void *data);
void destroyTextureEntry(partyRenderer *renderer, uint32_t idx);

#endif _GFX_VK_H_