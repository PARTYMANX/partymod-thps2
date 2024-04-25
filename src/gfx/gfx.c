#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SDL2/SDL.h>

#include <window.h>
#include <event.h>
#include <patch.h>
#include <global.h>
#include <gfx/gfx_movie.h>
#include <gfx/vk/gfx_vk.h>
#include <gfx/gfx_global.h>

int resolution_x = 512;
int resolution_y = 240;
float aspectRatio = 4.0f / 3.0f;

int isMinimized = 0;

void handleGfxEvent(SDL_Event *e) {
	switch (e->type) {
		case SDL_WINDOWEVENT:
			if (e->window.event == SDL_WINDOWEVENT_MINIMIZED) {
				isMinimized = 1;
			} else if (e->window.event == SDL_WINDOWEVENT_RESTORED) {
				isMinimized = 0;
			} else if (e->window.event == SDL_WINDOWEVENT_RESIZED) {
				updateRenderer(renderer);
			}
			return;
	}
}

void initDDraw() {
	//printf("STUB: initDDraw\n");

	int *width = 0x029d6fe4;
	int *height = 0x029d6fe8;

	*width = resolution_x;
	*height = resolution_y;
}

void initD3D() {
	//printf("STUB: initD3D\n");
	void *hwnd = 0x029d4fc4;

	uint8_t result = CreateVKRenderer(hwnd, &renderer);

	if (!result) {
		printf("failed to init vulkan!!!\n");
	} else {
		printf("vulkan initialized!!\n");
	}

	setRenderResolution(renderer, resolution_x, resolution_y, aspectRatio);

	uint32_t *is_software_renderer = 0x029d6ff0;
	uint32_t *unk_related_to_sw_renderer = 0x00546cc4;

	*is_software_renderer = 0;
	*unk_related_to_sw_renderer = 1;

	// TODO: fog settings

	registerEventHandler(handleGfxEvent);
}

void D3D_ClearBuffers() {
	//printf("STUB: D3D_ClearBuffers\n");
}

void D3DPOLY_Init() {
	//printf("STUB: D3DPOLY_Init\n");
	void (__cdecl *D3DMODEL_Startup)() = 0x004cffb0;
	void (__cdecl *SOFTREND_Startup)() = 0x004ef810;
	void (__cdecl *Init_PolyBuf)() = 0x004d4640;

	D3DMODEL_Startup();
	SOFTREND_Startup();
	Init_PolyBuf();
}

uint32_t fixDXColor(uint32_t color) {
	return ((color & 0x00FF0000) >> 16) + ((color & 0x0000FF00) >> 0) + ((color & 0x000000FF) << 16) + ((color & 0xFF000000));
}

void D3DPOLY_StartScene(int a, int b) {
	uint16_t *SP_OTPushback = 0x005606c4;
	uint16_t *SP_OTPushback2 = 0x005606dc;
	uint16_t *SP_OTPushback3 = 0x005606e8;
	
	uint16_t *M3d_OTPushback = 0x00560fc8;
	uint16_t *M3d_OTPushback2 = 0x00560fca;
	uint16_t *M3d_OTPushback3 = 0x00560fcc;

	uint32_t *modelPushback = 0x0056b498;
	uint32_t *SP_ZPushback = 0x0055ed08;
	uint32_t *LTIM3D_Pushback = 0x00560798;

	printf("SP_PUSHBACK: %d %d %d\n", *SP_OTPushback, *SP_OTPushback2, *SP_OTPushback3);
	printf("M3D_PUSHBACK: %d %d %d\n", *M3d_OTPushback, *M3d_OTPushback2, *M3d_OTPushback3);
	printf("MODEL PUSHBACK: %d %d %d\n", *modelPushback, *SP_ZPushback, *LTIM3D_Pushback);

	void (__cdecl *setupFog)(int) = 0x004d1160;
	uint32_t *viewportClass = 0x00560698;
	uint16_t *DpqMin = 0x005606a4;
	uint16_t *DpqMinMaybe = *viewportClass + 8;
	uint16_t *DpqMaxMaybe = *viewportClass + 10;
	float *fogThreshold = 0x00546b3c;
	float *FogYonScale = 0x00546b38;
	float *VideoFogYonScale = 0x00545334;
	uint32_t *gShellMode = 0x006a35b4;
	uint8_t *startscene = 0x0069d114;

	//printf("STUB: D3DPOLY_StartScene: 0x%08x 0x%08x\n", a, b);

	//float *fog = 0x00546b38;
	//*fog = 10000.0f;

	*startscene = 1;

	updateMovieTexture();	// a bit of a hack: update the movie texture here in the main thread, as the music thread also updates it.  not exactly safe, but it avoids invalid vulkan use

	*fogThreshold = (float)*DpqMin / (float)*DpqMaxMaybe;
	if (gShellMode == 1) {
		*FogYonScale = 1000.0f;
	} else {
		*FogYonScale = *VideoFogYonScale;
	}

	float hither = *DpqMinMaybe;
	float yon = *DpqMaxMaybe * *FogYonScale;
	float zBufMin = hither / yon;

	//printf("HITHER %f YON %f ZBUFMIN %f\n", hither, yon, zBufMin);


	setupFog(a);

	uint32_t clearColor = fixDXColor(b);

	if (!isMinimized) {
		startRender(renderer, clearColor);
	}
}

void (*D3DPOLY_EndScene)() = 0x004d12e0;

void D3D_EndSceneAndFlip() {
	D3DPOLY_EndScene();
	//printf("STUB: D3D_EndSceneAndFlip\n");

	if (!isMinimized) {
		finishRender(renderer);
	}
}

struct dxpolytextured {
	float x, y, z, w;
	uint32_t color;
	float u, v;
};

struct dxpoly {
	float x, y, z, w;
	uint32_t color;
};

inline uint32_t applyFog(uint32_t color_in, float depth) {
	uint32_t *viewportClass = 0x00560698;
	float *fogThreshold = 0x00546b3c;
	float *FogYonScale = 0x00546b38;
	uint16_t *someotherfogvalue = *viewportClass + 14;
	uint16_t *DpqMaxMaybe = *viewportClass + 10;
	uint32_t *aFog = 0x00599f90;
	uint32_t *rFog = 0x0061a47c;
	uint32_t *gFog = 0x0065c8b0;
	uint32_t *bFog = 0x005da390;

	float fogDist = (((float)*someotherfogvalue / depth) / ((float)*DpqMaxMaybe * *FogYonScale));
	//printf("Fog dist = %f\n", fogDist);

	if ((*fogThreshold < 1.0f) && (*fogThreshold <= fogDist)) {
		float fogFactor = (fogDist - *fogThreshold) / (1.0f - *fogThreshold);
		fogFactor *= 255.0f;
		if (fogFactor > 255.0f) {
			fogFactor = 255.0f;
		}

		uint32_t fogIdx = fogFactor;

		uint32_t r = color_in >> 16 & 0x000000ff;
		uint32_t g = color_in >> 8 & 0x000000ff;
		uint32_t b = color_in & 0x000000ff;
		uint32_t a = color_in >> 24 & 0x000000ff;

		return rFog[r * 256 + fogIdx] | gFog[g * 256 + fogIdx] | bFog[b * 256 + fogIdx] | aFog[a * 256 + fogIdx];
	}

	return color_in;
}

void transformDXCoords(renderVertex *vertices, int count) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	float xmult = (1.0f / (float)*screen_width) * 2.0f;
	float ymult = (1.0f / (float)*screen_height) * 2.0f;
	float xdisp = 1.0f - (xmult * 0.5);
	float ydisp = 1.0f - (ymult * 0.5);

	for (int i = 0; i < count; i++) {
		vertices[i].x = (vertices[i].x * xmult) - xdisp;
		vertices[i].y = (vertices[i].y * ymult) - ydisp;

		vertices[i].w = 1.0f / vertices[i].w;
		vertices[i].x *= vertices[i].w;
		vertices[i].y *= vertices[i].w;
		vertices[i].z *= vertices[i].w;
	}

	
}

// this is implemented in a kind of stupid way but i didn't want to compile another pipeline.  
// i also want it to correctly handle > 1 tri polygons but for some reason this game rarely seems to use them.  weird
void renderDXPolyWireframe(int *tag) {
	if (!*(uint32_t *)((uint8_t *)tag + 0x10)) {
		struct dxpoly *vertices = ((uint8_t *)tag + 0x18);
		uint32_t numVerts = *(uint32_t *)((uint8_t *)tag + 0x14);

		setBlendState(renderer, 1);
		setDepthState(renderer, 1, 1);

		renderVertex buf[18];

		int outputVert = 0;
		for (int i = 0; i < numVerts - 2 + 1; i++) {
			buf[outputVert].x = vertices[i].x;
			buf[outputVert].y = vertices[i].y;
			buf[outputVert].z = 1.0 - vertices[i].z;
			buf[outputVert].u = 0.0f;
			buf[outputVert].v = 0.0f;
			buf[outputVert].color = fixDXColor(vertices[i].color);

			buf[outputVert + 1].x = vertices[i + 1].x;
			buf[outputVert + 1].y = vertices[i + 1].y;
			buf[outputVert + 1].z = 1.0 - vertices[i + 1].z;
			buf[outputVert + 1].u = 0.0f;
			buf[outputVert + 1].v = 0.0f;
			buf[outputVert + 1].color = fixDXColor(vertices[i + 1].color);

			outputVert += 2;
		} 

		// close the shape
		if (outputVert > 0) {
			buf[outputVert] = buf[outputVert - 1];
			buf[outputVert + 1] = buf[0];

			outputVert += 2;
		}

		transformDXCoords(buf, outputVert);
		drawLines(renderer, buf, outputVert);
	} else {
		struct dxpolytextured *vertices = ((uint8_t *)tag + 0x18);
		uint32_t numVerts = *(uint32_t *)((uint8_t *)tag + 0x14);

		setBlendState(renderer, 1);
		setDepthState(renderer, 1, 1);

		renderVertex buf[18];

		int outputVert = 0;
		for (int i = 0; i < numVerts - 2 + 1; i++) {
			buf[outputVert].x = vertices[i].x;
			buf[outputVert].y = vertices[i].y;
			buf[outputVert].z = 1.0 - vertices[i].z;
			buf[outputVert].u = 0.0f;
			buf[outputVert].v = 0.0f;
			buf[outputVert].color = fixDXColor(vertices[i].color);

			buf[outputVert + 1].x = vertices[i + 1].x;
			buf[outputVert + 1].y = vertices[i + 1].y;
			buf[outputVert + 1].z = 1.0 - vertices[i + 1].z;
			buf[outputVert + 1].u = 0.0f;
			buf[outputVert + 1].v = 0.0f;
			buf[outputVert + 1].color = fixDXColor(vertices[i + 1].color);

			outputVert += 2;
		} 

		// close the shape
		if (outputVert > 0) {
			buf[outputVert] = buf[outputVert - 1];
			buf[outputVert + 1] = buf[0];

			outputVert += 2;
		}

		transformDXCoords(buf, outputVert);
		drawLines(renderer, buf, outputVert);
	}
}

void renderDXPoly(int *tag) {
	uint32_t polyflags = *(uint32_t *)((uint8_t *)tag + 8);

	if ((polyflags & 0x30000000) == 0x10000000) {
		// wireframe unfilled
		renderDXPolyWireframe(tag);
		return;
	} else if ((polyflags & 0x30000000) & 0x20000000) {
		// wireframe filled
		renderDXPolyWireframe(tag);
	}

	uint32_t alpha;
	uint16_t renderFlags = 0;

	if (polyflags & 0x40) {
		setDepthState(renderer, 1, 0);

		// alpha blending
		switch(polyflags & 0x180) {
		case 0x0:
			alpha = 0x80000000;
			// blend mode 1
			setBlendState(renderer, 1);
			break;
		case 0x80:
			alpha = 0xff000000;
			// blend mode 2
			setBlendState(renderer, 2);
			break;
		case 0x100:
			alpha = 0x00000000;
			// blend mode 4
			setBlendState(renderer, 4);
			renderFlags |= 1;
			break;
		case 0x180:
			alpha = 0x40000000;
			// blend mode 2
			setBlendState(renderer, 2);
			break;
		default:
			printf("unknown blend mode 0x%08x\n", polyflags & 0x40);
			alpha = 0xff000000;
		}

	} else {
		alpha = 0xff000000;
		setDepthState(renderer, 1, 1);
		setBlendState(renderer, 1);
	}

	if (!*(uint32_t *)((uint8_t *)tag + 0x10)) {
		struct dxpoly *vertices = ((uint8_t *)tag + 0x18);
		uint32_t numVerts = *(uint32_t *)((uint8_t *)tag + 0x14);

		// calc final colors
		for (int i = 0; i < numVerts; i++) {
			// apply alpha from blend mode
			uint8_t r = ((float)((vertices[i].color >> 16) & 0xff) / 31.0f) * 255.0f;
			uint8_t g = ((float)((vertices[i].color >> 8) & 0xff) / 31.0f) * 255.0f;
			uint8_t b = ((float)((vertices[i].color >> 0) & 0xff) / 31.0f) * 255.0f;

			//((float)((color >> 11) & 0x1f) / 31.0f) * 255.0f

			uint32_t newcolor = (vertices[i].color & 0x00ffffff) | alpha;


			if (!(polyflags & 0x80000000)) {
				newcolor = applyFog(newcolor, vertices[i].w);
			}

			vertices[i].color = newcolor;
		}

		//printf("RenderDXPoly with %d vertices!!! 0x%08x\n", numVerts, vertices);

		//printf("1: %f %f %f 0x%08x\n", vertices[0].x, vertices[0].y, vertices[0].z, vertices[0].color);
		//printf("2: %f %f %f 0x%08x\n", vertices[1].x, vertices[1].y, vertices[1].z, vertices[1].color);
		//printf("1: %f\n", ((vertices[0].x / 640.0f) * 2.0f) - 1.0f);

		renderVertex buf[18];

		//numVerts = 3;

		int outputVert = 0;
		for (int i = 1; i < numVerts - 2 + 1; i++) {
			buf[outputVert].x = vertices[0].x;
			buf[outputVert].y = vertices[0].y;
			buf[outputVert].z = vertices[0].z;
			buf[outputVert].w = vertices[0].w;
			buf[outputVert].u = 0.0f;
			buf[outputVert].v = 0.0f;
			buf[outputVert].color = fixDXColor(vertices[0].color);
			buf[outputVert].texture = -1;
			buf[outputVert].flags = 0;

			buf[outputVert + 1].x = vertices[i].x;
			buf[outputVert + 1].y = vertices[i].y;
			buf[outputVert + 1].z = vertices[i].z;
			buf[outputVert + 1].w = vertices[i].w;
			buf[outputVert + 1].u = 0.0f;
			buf[outputVert + 1].v = 0.0f;
			buf[outputVert + 1].color = fixDXColor(vertices[i].color);
			buf[outputVert + 1].texture = -1;
			buf[outputVert + 1].flags = 0;

			buf[outputVert + 2].x = vertices[i + 1].x;
			buf[outputVert + 2].y = vertices[i + 1].y;
			buf[outputVert + 2].z = vertices[i + 1].z;
			buf[outputVert + 2].w = vertices[i + 1].w;
			buf[outputVert + 2].u = 0.0f;
			buf[outputVert + 2].v = 0.0f;
			buf[outputVert + 2].color = fixDXColor(vertices[i + 1].color);
			buf[outputVert + 2].texture = -1;
			buf[outputVert + 2].flags = 0;

			outputVert += 3;
		} 

		transformDXCoords(buf, outputVert);
		drawVertices(renderer, buf, outputVert);
	} else {
		struct dxpolytextured *vertices = ((uint8_t *)tag + 0x18);
		uint32_t numVerts = *(uint32_t *)((uint8_t *)tag + 0x14);
		struct texture *tex = *(uint32_t **)(*(int *)((uint8_t *)tag + 0x10) + 0x14);

		if (!tex) {
			printf("TEXTURE WAS NULL!!!\n");
			return;
		}

		if (tex->flags & 0x01000000) {
			renderFlags |= 1;
		}

		//printf("TEST!! 0x%08x %d %d %d\n", tex, tex->idx, tex->width, tex->height);

		if (!(tex->flags & 0x10)) {
			//printf("TEST???\n");
			setDepthState(renderer, 1, 0);
		}

		// calc final colors
		for (int i = 0; i < numVerts; i++) {
			// apply alpha from blend mode
			uint8_t r = ((vertices[i].color >> 16) & 0xff);
			uint8_t g = ((vertices[i].color >> 8) & 0xff);
			uint8_t b = ((vertices[i].color >> 0) & 0xff);

			//((float)((color >> 11) & 0x1f) / 31.0f) * 255.0f

			uint32_t newcolor = r << 16 | g << 8 | b << 0 | alpha;

			if (!(*(uint32_t *)((uint8_t *)tag + 8) & 0x80000000)) {
				newcolor = applyFog(newcolor, vertices[i].w);
			}

			vertices[i].color = newcolor;
		}

		//printf("RenderDXPoly with %d vertices!!! 0x%08x\n", numVerts, vertices);

		//printf("1: %f %f %f 0x%08x\n", vertices[0].x, vertices[0].y, vertices[0].z, vertices[0].color);
		//printf("2: %f %f %f 0x%08x\n", vertices[1].x, vertices[1].y, vertices[1].z, vertices[1].color);
		//printf("1: %f\n", ((vertices[0].x / 640.0f) * 2.0f) - 1.0f);

		//struct texture *tex = (*(uint32_t **)*((uint8_t *)tag + 0x10) + 0x14);
		
		//printf("using texture %d\n", tex->idx);

		renderVertex buf[18];

		//numVerts = 3;

		int outputVert = 0;
		for (int i = 1; i < numVerts - 2 + 1; i++) {
			
			buf[outputVert].x = vertices[0].x;
			buf[outputVert].y = vertices[0].y;
			buf[outputVert].z = vertices[0].z;
			buf[outputVert].w = vertices[0].w;
			buf[outputVert].u = vertices[0].u * tex->width / tex->buf_width;
			buf[outputVert].v = vertices[0].v * tex->height / tex->buf_height;
			buf[outputVert].color = fixDXColor(vertices[0].color);
			buf[outputVert].texture = tex->idx;
			buf[outputVert].flags = renderFlags;

			buf[outputVert + 1].x = vertices[i].x;
			buf[outputVert + 1].y = vertices[i].y;
			buf[outputVert + 1].z = vertices[i].z; 
			buf[outputVert + 1].w = vertices[i].w;
			buf[outputVert + 1].u = vertices[i].u * tex->width / tex->buf_width;
			buf[outputVert + 1].v = vertices[i].v * tex->height / tex->buf_height;
			buf[outputVert + 1].color = fixDXColor(vertices[i].color);
			buf[outputVert + 1].texture = tex->idx;
			buf[outputVert + 1].flags = renderFlags;

			buf[outputVert + 2].x = vertices[i + 1].x;
			buf[outputVert + 2].y = vertices[i + 1].y;
			buf[outputVert + 2].z = vertices[i + 1].z;
			buf[outputVert + 2].w = vertices[i + 1].w;
			buf[outputVert + 2].u = vertices[i + 1].u * tex->width / tex->buf_width;
			buf[outputVert + 2].v = vertices[i + 1].v * tex->height / tex->buf_height;
			buf[outputVert + 2].color = fixDXColor(vertices[i + 1].color);
			buf[outputVert + 2].texture = tex->idx;
			buf[outputVert + 2].flags = renderFlags;

			outputVert += 3;
		} 

		transformDXCoords(buf, outputVert);
		drawVertices(renderer, buf, outputVert);
	}
}

uint32_t unormColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return r + (g << 8) + (b << 16) + (a << 24);
}

uint8_t validateShard(renderVertex *vertices, int count) {
	// breakable stuff sometimes give bad coords for some reason, so just ignore those when they appear (bad bad hack)
	uint8_t isValid = 1;
	for (int i = 0; i < count; i++) {
		if (vertices[i].x == 1023.0f || vertices[i].x == -1023.0f ||
			vertices[i].y == 1023.0f) {
			isValid = 0;
			break;
		}
	}

	return isValid;
}

void transformCoords(renderVertex *vertices, int count) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	float xmult = (1.0f / ((float)*screen_width)) * 2.0f;
	float ymult = (1.0f / ((float)*screen_height)) * 2.0f;
	// 0.125 of a pixel added to help align scaled elements to a single pixel (important for UI)
	float xdisp = 1.0f - (xmult * 0.125f);
	float ydisp = 1.0f - (ymult * 0.125f);

	// first, check that the y position is on-screen
	uint8_t on_screen = 0;

	for (int i = 0; i < count; i++) {
		if (vertices[i].y <= (float)*screen_height) {
			on_screen = 1;
		}
	}

	for (int i = 0; i < count; i++) {
		if (vertices[i].y > 4368.0f - (float)*screen_height && on_screen) {
			vertices[i].y -= 4368.0f;
		}
	}

	for (int i = 0; i < count; i++) {
		vertices[i].x = (vertices[i].x * xmult) - xdisp;
		vertices[i].y = (vertices[i].y * ymult) - ydisp;
	}
}

void transformCoordsLine(renderVertex *vertices, int count) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	float xmult = (1.0f / ((float)*screen_width)) * 2.0f;
	float ymult = (1.0f / ((float)*screen_height)) * 2.0f;
	float xdisp = 1.0f + (xmult * 0.5);
	float ydisp = 1.0f + (ymult * 0.5);

	// first, check that the y position is on-screen
	uint8_t on_screen = 0;

	for (int i = 0; i < count; i++) {
		if (vertices[i].y <= (float)*screen_height) {
			on_screen = 1;
		}
	}

	for (int i = 0; i < count; i++) {
		if (vertices[i].y > 4368.0f - (float)*screen_height && on_screen) {
			vertices[i].y -= 4368.0f;
		}
	}

	for (int i = 0; i < count; i++) {
		vertices[i].x = (vertices[i].x * xmult) - xdisp;
		vertices[i].y = (vertices[i].y * ymult) - ydisp;
	}
}

float fixZ(float z) {
	if (z < 0.0f) {
		setDepthState(renderer, 0, 0);

		z = 0.0f;
	} else {
		z = z;
	}

	return z;
}

void fixUVs(renderVertex *vertices, int count, int width, int height) {
	float wrecip = 1.0f / width;
	float hrecip = 1.0f / height;

	// find lowest coord on each axis
	float lowestU = vertices[0].u;
	float lowestV = vertices[0].v;
	for (int i = 1; i < count; i++) {
		if (vertices[i].u < lowestU) {
			lowestU = vertices[i].u;
		}

		if (vertices[i].v < lowestV) {
			lowestV = vertices[i].v;
		}
	}

	for (int i = 0; i < count; i++) {
		if (vertices[i].u > lowestU) {
			vertices[i].u += 1.0f;
		}

		if (vertices[i].v > lowestV) {
			vertices[i].v += 1.0f;
		}

		vertices[i].u *= wrecip;
		vertices[i].v *= hrecip;
	}
}

void renderLineF2(int *tag) {
	uint8_t r = *((uint8_t *)tag + 4);
	uint8_t g = *((uint8_t *)tag + 5);
	uint8_t b = *((uint8_t *)tag + 6);
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 12);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 14);

	float z = *(float *)((uint8_t *)tag + 16);
	z = fixZ(z);

	renderVertex vertices[2];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };

	transformCoordsLine(vertices, 2);

	drawLines(renderer, vertices, 2);
}

void renderLineF3(int *tag) {
	uint8_t r = *((uint8_t *)tag + 4);
	uint8_t g = *((uint8_t *)tag + 5);
	uint8_t b = *((uint8_t *)tag + 6);
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 12);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 14);
	int16_t x3 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 18);

	float z = *(float *)((uint8_t *)tag + 20);
	z = fixZ(z);

	renderVertex vertices[4];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[2] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[3] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };

	transformCoordsLine(vertices, 4);

	drawLines(renderer, vertices, 4);
}

void renderLineF4(int *tag) {
	uint8_t r = *((uint8_t *)tag + 4);
	uint8_t g = *((uint8_t *)tag + 5);
	uint8_t b = *((uint8_t *)tag + 6);
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 12);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 14);
	int16_t x3 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 18);
	int16_t x4 = *(int16_t *)((uint8_t *)tag + 20);
	int16_t y4 = *(int16_t *)((uint8_t *)tag + 22);

	float z = *(float *)((uint8_t *)tag + 24);
	z = fixZ(z);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[2] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[3] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };

	transformCoordsLine(vertices, 6);

	drawLines(renderer, vertices, 6);
}

void renderLineG2(int *tag) {
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;
	
	uint32_t color1 = unormColor(*((uint8_t *)tag + 4), *((uint8_t *)tag + 5), *((uint8_t *)tag + 6), alpha);
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);

	uint32_t color2 = unormColor(*((uint8_t *)tag + 12), *((uint8_t *)tag + 13), *((uint8_t *)tag + 14), alpha);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 18);

	float z = *(float *)((uint8_t *)tag + 20);
	z = fixZ(z);

	renderVertex vertices[2];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color1, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color2, -1, 0 };
	
	transformCoordsLine(vertices, 2);

	drawLines(renderer, vertices, 2);
}

void renderPolyF3(int *tag) {
	uint8_t r = *((uint8_t *)tag + 4);
	uint8_t g = *((uint8_t *)tag + 5);
	uint8_t b = *((uint8_t *)tag + 6);
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 12);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 14);
	int16_t x3 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 18);

	float z = *(float *)((uint8_t *)tag + 20);
	z = fixZ(z);

	renderVertex vertices[3];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };

	if (!validateShard(vertices, 3)) {
		return;
	}

	transformCoords(vertices, 3);

	drawVertices(renderer, vertices, 3);
}

void renderPolyF4(int *tag) {
	uint8_t r = *((uint8_t *)tag + 4);
	uint8_t g = *((uint8_t *)tag + 5);
	uint8_t b = *((uint8_t *)tag + 6);
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 12);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 14);
	int16_t x3 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 18);
	int16_t x4 = *(int16_t *)((uint8_t *)tag + 20);
	int16_t y4 = *(int16_t *)((uint8_t *)tag + 22);

	float z = *(float *)((uint8_t *)tag + 24);
	z = fixZ(z);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };

	if (!validateShard(vertices, 6)) {
		return;
	}

	transformCoords(vertices, 6);

	drawVertices(renderer, vertices, 6);
}

void renderPolyFT3(int *tag) {
	if (*(int *)((uint8_t *)tag + 36)) {
		struct texture *tex = *(uint32_t **)(*(int *)((uint8_t *)tag + 36) + 0x14);

		uint8_t r = *((uint8_t *)tag + 4);
		uint8_t g = *((uint8_t *)tag + 5);
		uint8_t b = *((uint8_t *)tag + 6);
		uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

		uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
		int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
		int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
		uint8_t u1 = *(uint8_t *)((uint8_t *)tag + 12);
		uint8_t v1 = *(uint8_t *)((uint8_t *)tag + 13);

		int16_t x2 = *(int16_t *)((uint8_t *)tag + 16);
		int16_t y2 = *(int16_t *)((uint8_t *)tag + 18);
		uint8_t u2 = *(uint8_t *)((uint8_t *)tag + 20);
		uint8_t v2 = *(uint8_t *)((uint8_t *)tag + 21);
	
		int16_t x3 = *(int16_t *)((uint8_t *)tag + 24);
		int16_t y3 = *(int16_t *)((uint8_t *)tag + 26);
		uint8_t u3 = *(uint8_t *)((uint8_t *)tag + 28);
		uint8_t v3 = *(uint8_t *)((uint8_t *)tag + 29);
		
		float z = *(float *)((uint8_t *)tag + 32);
		z = fixZ(z);

		renderVertex vertices[3];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, (float)u1, (float)v1, color, tex->idx, 0 };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, (float)u2, (float)v2, color, tex->idx, 0 };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, (float)u3, (float)v3, color, tex->idx, 0 };

		if (!validateShard(vertices, 3)) {
			return;
		}

		fixUVs(vertices, 3, tex->buf_width, tex->buf_height);
		transformCoords(vertices, 3);

		drawVertices(renderer, vertices, 3);
	}
}

void renderPolyFT4(int *tag) {
	if (*(int *)((uint8_t *)tag + 44)) {
		struct texture *tex = *(uint32_t **)(*(int *)((uint8_t *)tag + 44) + 0x14);

		uint8_t r = *((uint8_t *)tag + 4);
		uint8_t g = *((uint8_t *)tag + 5);
		uint8_t b = *((uint8_t *)tag + 6);
		uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

		uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
		int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
		int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
		uint8_t u1 = *(uint8_t *)((uint8_t *)tag + 12);
		uint8_t v1 = *(uint8_t *)((uint8_t *)tag + 13);

		int16_t x2 = *(int16_t *)((uint8_t *)tag + 16);
		int16_t y2 = *(int16_t *)((uint8_t *)tag + 18);
		uint8_t u2 = *(uint8_t *)((uint8_t *)tag + 20);
		uint8_t v2 = *(uint8_t *)((uint8_t *)tag + 21);
	
		int16_t x3 = *(int16_t *)((uint8_t *)tag + 24);
		int16_t y3 = *(int16_t *)((uint8_t *)tag + 26);
		uint8_t u3 = *(uint8_t *)((uint8_t *)tag + 28);
		uint8_t v3 = *(uint8_t *)((uint8_t *)tag + 29);
	
		int16_t x4 = *(int16_t *)((uint8_t *)tag + 32);
		int16_t y4 = *(int16_t *)((uint8_t *)tag + 34);
		uint8_t u4 = *(uint8_t *)((uint8_t *)tag + 36);
		uint8_t v4 = *(uint8_t *)((uint8_t *)tag + 37);
		
		float z = *(float *)((uint8_t *)tag + 40);
		z = fixZ(z);

		renderVertex vertices[6];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, (float)u1, (float)v1, color, tex->idx, 0 };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, (float)u2, (float)v2, color, tex->idx, 0 };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, (float)u3, (float)v3, color, tex->idx, 0 };
		vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, (float)u2, (float)v2, color, tex->idx, 0 };
		vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, (float)u3, (float)v3, color, tex->idx, 0 };
		vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 1.0f, (float)u4, (float)v4, color, tex->idx, 0 };

		if (!validateShard(vertices, 6)) {
			return;
		}

		fixUVs(vertices, 6, tex->buf_width, tex->buf_height);
		transformCoords(vertices, 6);

		drawVertices(renderer, vertices, 6);
	}
}

void renderPolyG3(int *tag) {
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color1 = unormColor(*((uint8_t *)tag + 4), *((uint8_t *)tag + 5), *((uint8_t *)tag + 6), alpha);
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);

	uint32_t color2 = unormColor(*((uint8_t *)tag + 12), *((uint8_t *)tag + 13), *((uint8_t *)tag + 14), alpha);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 18);
	
	uint32_t color3 = unormColor(*((uint8_t *)tag + 20), *((uint8_t *)tag + 21), *((uint8_t *)tag + 22), alpha);
	int16_t x3 = *(int16_t *)((uint8_t *)tag + 24);
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 26);

	float z = *(float *)((uint8_t *)tag + 28);
	z = fixZ(z);

	renderVertex vertices[3];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color1, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color2, -1, 0 };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color3, -1, 0 };

	if (!validateShard(vertices, 3)) {
		return;
	}

	transformCoords(vertices, 3);

	drawVertices(renderer, vertices, 3);
}

void renderPolyG4(int *tag) {
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color1 = unormColor(*((uint8_t *)tag + 4), *((uint8_t *)tag + 5), *((uint8_t *)tag + 6), alpha);
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);

	uint32_t color2 = unormColor(*((uint8_t *)tag + 12), *((uint8_t *)tag + 13), *((uint8_t *)tag + 14), alpha);
	int16_t x2 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 18);
	
	uint32_t color3 = unormColor(*((uint8_t *)tag + 20), *((uint8_t *)tag + 21), *((uint8_t *)tag + 22), alpha);
	int16_t x3 = *(int16_t *)((uint8_t *)tag + 24);
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 26);
	
	uint32_t color4 = unormColor(*((uint8_t *)tag + 28), *((uint8_t *)tag + 29), *((uint8_t *)tag + 30), alpha);
	int16_t x4 = *(int16_t *)((uint8_t *)tag + 32);
	int16_t y4 = *(int16_t *)((uint8_t *)tag + 34);

	float z = *(float *)((uint8_t *)tag + 36);
	z = fixZ(z);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color1, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color2, -1, 0 };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color3, -1, 0 };
	vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color2, -1, 0 };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color3, -1, 0 };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 1.0f, 0.0f, 0.0f, color4, -1, 0 };

	if (!validateShard(vertices, 6)) {
		return;
	}

	transformCoords(vertices, 6);

	drawVertices(renderer, vertices, 6);
}

void renderPolyGT3(int *tag) {
	if (*(int *)((uint8_t *)tag + 44)) {
		struct texture *tex = *(uint32_t **)(*(int *)((uint8_t *)tag + 44) + 0x14);

		uint8_t r = *((uint8_t *)tag + 4);
		uint8_t g = *((uint8_t *)tag + 5);
		uint8_t b = *((uint8_t *)tag + 6);
		uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;
	
		uint32_t color1 = unormColor(*((uint8_t *)tag + 4), *((uint8_t *)tag + 5), *((uint8_t *)tag + 6), alpha);
		int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
		int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
		uint8_t u1 = *(uint8_t *)((uint8_t *)tag + 12);
		uint8_t v1 = *(uint8_t *)((uint8_t *)tag + 13);

		uint32_t color2 = unormColor(*((uint8_t *)tag + 16), *((uint8_t *)tag + 17), *((uint8_t *)tag + 18), alpha);
		int16_t x2 = *(int16_t *)((uint8_t *)tag + 20);
		int16_t y2 = *(int16_t *)((uint8_t *)tag + 22);
		uint8_t u2 = *(uint8_t *)((uint8_t *)tag + 24);
		uint8_t v2 = *(uint8_t *)((uint8_t *)tag + 25);
	
		uint32_t color3 = unormColor(*((uint8_t *)tag + 28), *((uint8_t *)tag + 29), *((uint8_t *)tag + 30), alpha);
		int16_t x3 = *(int16_t *)((uint8_t *)tag + 32);
		int16_t y3 = *(int16_t *)((uint8_t *)tag + 34);
		uint8_t u3 = *(uint8_t *)((uint8_t *)tag + 36);
		uint8_t v3 = *(uint8_t *)((uint8_t *)tag + 37);
		
		float z = *(float *)((uint8_t *)tag + 40);
		z = fixZ(z);

		renderVertex vertices[6];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, (float)u1, (float)v1, color1, tex->idx, 0 };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, (float)u2, (float)v2, color2, tex->idx, 0 };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, (float)u3, (float)v3, color3, tex->idx, 0 };

		if (!validateShard(vertices, 3)) {
			return;
		}

		fixUVs(vertices, 3, tex->buf_width, tex->buf_height);
		transformCoords(vertices, 3);

		drawVertices(renderer, vertices, 3);
	}
}

void renderPolyGT4(int *tag) {
	if (*(int *)((uint8_t *)tag + 56)) {
		struct texture *tex = *(uint32_t **)(*(int *)((uint8_t *)tag + 56) + 0x14);

		uint8_t r = *((uint8_t *)tag + 4);
		uint8_t g = *((uint8_t *)tag + 5);
		uint8_t b = *((uint8_t *)tag + 6);
		uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;
	
		uint32_t color1 = unormColor(*((uint8_t *)tag + 4), *((uint8_t *)tag + 5), *((uint8_t *)tag + 6), alpha);
		int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
		int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
		uint8_t u1 = *(uint8_t *)((uint8_t *)tag + 12);
		uint8_t v1 = *(uint8_t *)((uint8_t *)tag + 13);

		uint32_t color2 = unormColor(*((uint8_t *)tag + 16), *((uint8_t *)tag + 17), *((uint8_t *)tag + 18), alpha);
		int16_t x2 = *(int16_t *)((uint8_t *)tag + 20);
		int16_t y2 = *(int16_t *)((uint8_t *)tag + 22);
		uint8_t u2 = *(uint8_t *)((uint8_t *)tag + 24);
		uint8_t v2 = *(uint8_t *)((uint8_t *)tag + 25);
	
		uint32_t color3 = unormColor(*((uint8_t *)tag + 28), *((uint8_t *)tag + 29), *((uint8_t *)tag + 30), alpha);
		int16_t x3 = *(int16_t *)((uint8_t *)tag + 32);
		int16_t y3 = *(int16_t *)((uint8_t *)tag + 34);
		uint8_t u3 = *(uint8_t *)((uint8_t *)tag + 36);
		uint8_t v3 = *(uint8_t *)((uint8_t *)tag + 37);
	
		uint32_t color4 = unormColor(*((uint8_t *)tag + 40), *((uint8_t *)tag + 41), *((uint8_t *)tag + 42), alpha);
		int16_t x4 = *(int16_t *)((uint8_t *)tag + 44);
		int16_t y4 = *(int16_t *)((uint8_t *)tag + 46);
		uint8_t u4 = *(uint8_t *)((uint8_t *)tag + 48);
		uint8_t v4 = *(uint8_t *)((uint8_t *)tag + 49);
		
		float z = *(float *)((uint8_t *)tag + 52);
		z = fixZ(z);

		renderVertex vertices[6];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, (float)u1, (float)v1, color1, tex->idx, 0 };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, (float)u2, (float)v2, color2, tex->idx, 0 };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, (float)u3, (float)v3, color3, tex->idx, 0 };
		vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, (float)u2, (float)v2, color2, tex->idx, 0 };
		vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, (float)u3, (float)v3, color3, tex->idx, 0 };
		vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 1.0f, (float)u4, (float)v4, color4, tex->idx, 0 };

		if (!validateShard(vertices, 6)) {
			return;
		}

		fixUVs(vertices, 6, tex->buf_width, tex->buf_height);
		transformCoords(vertices, 6);

		drawVertices(renderer, vertices, 6);
	}
}

void renderTile(int *tag) {
	uint8_t r = *((uint8_t *)tag + 4);
	uint8_t g = *((uint8_t *)tag + 5);
	uint8_t b = *((uint8_t *)tag + 6);
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;

	uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);

	int16_t width = *(int16_t *)((uint8_t *)tag + 12);
	int16_t height = *(int16_t *)((uint8_t *)tag + 14);

	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);

	int16_t x2 = *(int16_t *)((uint8_t *)tag + 8) + width;
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 10);

	int16_t x3 = *(int16_t *)((uint8_t *)tag + 8) + width;
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 10) + height;

	int16_t x4 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y4 = *(int16_t *)((uint8_t *)tag + 10) + height;

	float z = *(float *)((uint8_t *)tag + 16);
	z = fixZ(z);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[3] = (renderVertex) { (float)x1, (float)y1, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 1.0f, 0.0f, 0.0f, color, -1, 0 };

	transformCoords(vertices, 6);

	drawVertices(renderer, vertices, 6);
}

void renderTile1(int *tag) {
	*(int16_t *)((uint8_t *)tag + 12) = 1;
	*(int16_t *)((uint8_t *)tag + 14) = 1;

	renderTile(tag);
}

void renderTile8(int *tag) {
	*(int16_t *)((uint8_t *)tag + 12) = 8;
	*(int16_t *)((uint8_t *)tag + 14) = 8;

	renderTile(tag);
}

void renderTile16(int *tag) {
	*(int16_t *)((uint8_t *)tag + 12) = 16;
	*(int16_t *)((uint8_t *)tag + 14) = 16;

	renderTile(tag);
}

void changeViewport(int *tag) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	// all values are based on the original resolution of 512x240
	float x = *(int16_t*)((uint8_t*)tag + 8);
	float y = *(int16_t *)((uint8_t *)tag + 10);
	float width = *(int16_t *)((uint8_t *)tag + 12);
	float height = *(int16_t *)((uint8_t *)tag + 14);

	if (y >= 256.0f) {
		y -= 256.0f;
	}

	if (x < 0) {
		x = 0;
	}

	if (y < 0) {
		y = 0;
	}

	if (width == 1) {
		width = 0;	// incredibly stupid hack to fix the stats menu.  my theory is that gray matter clamped to 1 to prevent directx from yelling at them for making a 0 width viewport.  needs revisit
	}

	x = (x / 512.0f) * (float)*screen_width;
	y = (y / 240.0f) * (float)*screen_height;
	width = (width / 512.0f) * (float)*screen_width;
	height = (height / 240.0f) * (float)*screen_height;

	// we want scissor here, as viewport will restrict draw coords
	setScissor(renderer, x, y, width, height);
}

void D3DPOLY_DrawOTag(int *tag) {
	uint32_t *polyFlags = 0x0065c8ac;
	uint32_t *alphaBlend = 0x0069c8b0;
	uint32_t *nextPSXBlendMode = 0x0069d10c;

	if (isMinimized) {
		return;
	}

	while(tag != NULL) {
		if (tag[1] != 0) {
			// hangar heli causes command 226, what's up there?
			uint8_t cmd = *(uint8_t *)((int)tag + 7);
			if (!(cmd & 0x80)) {
				//*nextPSXBlendMode = 0;

				uint8_t blendMode = cmd & 0xfffffffc;
				uint8_t otherBlendMode = cmd & 4;
				if (blendMode == 0x40 || blendMode == 0x48 || blendMode == 0x4c || blendMode == 0x50) {
					otherBlendMode = 0;
				} else if (!otherBlendMode) {
					if (!(cmd & 0x04)) {
					
					} else {
					
					}

				} else {
					if (!(cmd & 0x10)) {
						blendMode = *(uint16_t *)(((uint8_t *)tag + 0x16));
					} else {
						blendMode = *(uint16_t *)(((uint8_t *)tag + 0x1a));
					}
				}

				if (cmd & 0x02) {
					setDepthState(renderer, 1, 0);

					if (otherBlendMode) {
						*nextPSXBlendMode = (blendMode >> 5) & 3;
					}

					// alpha blending
					switch(*nextPSXBlendMode) {
					case 0:
						*alphaBlend = 0x80000000;
						// blend mode 1
						setBlendState(renderer, 1);
						break;
					case 1:
						*alphaBlend = 0xff000000;
						// blend mode 2
						setBlendState(renderer, 2);
						break;
					case 2:
						*alphaBlend = 0x00000000;
						// blend mode 4
						setBlendState(renderer, 4);
						break;
					case 3:
						*alphaBlend = 0x40000000;
						// blend mode 2
						setBlendState(renderer, 2);
						break;
					default:
						printf("unknown blend mode 0x%08x\n", *nextPSXBlendMode);
						*alphaBlend = 0xff000000;
					}

				} else {
					*alphaBlend = 0xff000000;
					setDepthState(renderer, 1, 1);
					setBlendState(renderer, 1);
				}

				switch(cmd >> 2) {
					case 8: 
						renderPolyF3(tag);
						break;
					case 9: 
						renderPolyFT3(tag);
						break;
					case 10: 
						renderPolyF4(tag);
						break;
					case 11: 
						renderPolyFT4(tag);
						break;
					case 12: 
						renderPolyG3(tag);
						break;
					case 13: 
						renderPolyGT3(tag);
						break;
					case 14: 
						renderPolyG4(tag);
						break;
					case 15: 
						renderPolyGT4(tag);
						break;
					case 16: 
						renderLineF2(tag);
						break;
					case 18: 
						renderLineF3(tag);
						break;
					case 19: 
						renderLineF4(tag);
						break;
					case 20: 
						renderLineG2(tag);
						break;
					case 24: 
						renderTile(tag);
						break;
					case 26: 
						renderTile1(tag);
						break;
					case 28: 
						renderTile8(tag);
						break;
					case 30: 
						renderTile16(tag);
						break;
					default:
						printf("UNKNOWN RENDER COMMAND: %d\n", cmd >> 2);
				}
			} else if (cmd == 0xB0) {
				renderDXPoly(tag);
			} else if (cmd == 0xE1) {
				// maybe blend mode
				uint32_t blendMode = tag[1];
				*nextPSXBlendMode = (blendMode >> 5) & 3;
			} else if (cmd == 0xE3) {
				// set viewport
				changeViewport(tag);
			} else {
				//printf("UNKNOWN RENDER COMMAND: %d\n", cmd);
			}
		}
		tag = *tag;
	}
}

void *(*createGameTexture)() = 0x4d6a70;


void D3DTEX_Init() {
	printf("STUB: D3DTEX_Init\n");

	//setup 16 bit format:
	uint32_t *r_bits = 0x0069d148;
	uint32_t *g_bits = 0x0069d14c;
	uint32_t *b_bits = 0x0069d150;
	uint32_t *a_bits = 0x0069d154;
	uint32_t *r_offset = 0x0069d158;
	uint32_t *g_offset = 0x0069d15c;
	uint32_t *b_offset = 0x0069d160;
	uint32_t *a_offset = 0x0069d164;

	*r_bits = 5;
	*g_bits = 5;
	*b_bits = 5;
	*a_bits = 1;
	*r_offset = 0;
	*g_offset = 5;
	*b_offset = 10;
	*a_offset = 15;
}

int D3DTEX_AddToTextureList(int a, int b, int c, char d) {
	printf("STUB: D3DTEX_AddToTextureList\n");

	return 0;
}

int D3DTEX_AddToTextureList2(int a, int b, int c, char d, int e, int f) {
	printf("STUB: D3DTEX_AddToTextureList2: %d, %d, %d, %d, %d, %d\n", a, b, c, d, e, f);

	uint32_t *result = createGameTexture();
	*(uint32_t *)((uint8_t *)result + 8) = a;
	*(uint16_t *)((uint8_t *)result + 20) = b;
	*(uint16_t *)((uint8_t *)result + 22) = c;
	*(uint32_t *)((uint8_t *)result + 16) = *(uint32_t *)((uint8_t *)result + 16) | 0x12;

	return result;
}

int D3DTEX_AddToTextureList3(int a, int b, int c, int d) {
	printf("STUB: D3DTEX_AddToTextureList3\n");

	return 0;
}

void D3DTEX_ConvertTexturePalette(uint16_t *palette, int size) {
	
}

uint32_t colorHSB(uint32_t hue, float saturation, float brightness, float opacity) {
	hue = hue % 360;
	float hue60 = (float)(hue % 60) / 60.0f;

	float red;
	float green;
	float blue;
	
	if (hue < 60.0f) {
		red = 1.0f;
		green = hue60;
		blue = 0.0f;
	} else if (hue < 120.0f) {
		red = 1.0f - hue60;
		green = 1.0f;
		blue = 0.0f;
	} else if (hue < 180.0f) {
		red = 0.0f;
		green = 1.0f;
		blue = hue60;
	} else if (hue < 240.0f) {
		red = 0.0f;
		green = 1.0f - hue60;
		blue = 1.0f;
	} else if (hue < 300.0f) {
		red = hue60;
		green = 0.0f;
		blue = 1.0f;
	} else  {
		red = 1.0f;
		green = 0.0f;
		blue = 1.0f - hue60;
	} 

	red = ((1.0f * (1.0f - saturation)) + (red * saturation)) * brightness;
	green = ((1.0f * (1 - saturation)) + (green * saturation)) * brightness;
	blue = ((1.0f * (1 - saturation)) + (blue * saturation)) * brightness;

	uint32_t result = ((uint8_t)(red * 255.0f) << 0) | ((uint8_t)(green * 255.0f) << 8) | ((uint8_t)(blue * 255.0f) << 16) | ((uint8_t)(opacity * 255.0f) << 24);

	return result;
}

void makeTextureListEntry(struct texture *a, int b, int c, int d) {
	//printf("STUB: makeTextureListEntry: 0x%08x, 0x%08x, %d, %d\n", a, b, c, d);
	int **palFront = 0x0069d174;

	if (a->palette == NULL) {
		int *pal = *palFront;
		while (pal) {
			//printf("palette: 0x%08x\n", *pal);
			if (*pal == b) {
				a->palette = pal;
				break;
			}

			pal = pal[4];
		}

		if (!a->palette) {
			printf("Palette Checksum not found: 0x%08x\n", b);
			a->palette = *palFront;
		} else {
			//printf("Found!\n");
		}
		
		if (a->palette && (uint16_t*)(a->palette[3]) && *(uint16_t*)(a->palette[3]) == 0) {
			a->flags = a->flags | 0x100;
		}
	}

	//printf("texture 0x%08x (0x%08x): width: %hu height: %hu\n", a->tex_checksum, a->img_data, a->width, a->height);

	if (!(a->flags & 8)) {
		//printf("Load 1 - local data only\n");
	} else {
		printf("Load 2 - load higher res\n");
		if (a->img_data && (a->flags & 0x40)) {
			printf("image name: %s\n", a->img_data);
		}
	}

	if (a->idx == 0) {
		// set buffer size (powers of two)
		a->buf_width = 1;
		a->buf_height = 1;

		while (a->buf_width < a->width) {
			a->buf_width <<= 1;
		}

		while (a->buf_height < a->height) {
			a->buf_height <<= 1;
		}

		// create texture
		a->idx = createTextureEntry(renderer, a->buf_width, a->buf_height);
	}

	// create random image data
	uint32_t hue = rand() % 360;
	uint32_t colorlight = colorHSB(hue, 0.75f, 1.0f, 1.0f);
	uint32_t colordark = colorHSB(hue, 0.75f, 0.5f, 1.0f);

	uint32_t *buf = malloc(sizeof(uint32_t) * a->buf_width * a->buf_height);

	for (int y = 0; y < a->buf_height; y++) {
		for (int x = 0; x < a->buf_width; x++) {
			//buf[(y * a->buf_width) + x] = ((x + y) % 2) ? colordark : colorlight;
			buf[(y * a->buf_width) + x] = 0;
		}
			
	}

	if (!(a->flags & 8)) {
		uint16_t *colors = *(uint16_t **)((uint8_t *)a->palette + 8);
		a->flags |= 0x10;
			
		if (*(uint32_t *)((uint8_t *)a->palette + 4) == 0x10) {
			// 4-bit
			int linewidth = (a->width + (a->width % 2)) / 2;
			linewidth += linewidth % 2;
			for (int y = 0; y < a->height; y++) {
				for (int x = 0; x < a->width; x++) {
					uint32_t pixel_idx = ((y * linewidth) + (x / 2));
					uint8_t color_idx = ((uint8_t *)a->img_data)[pixel_idx];

					if (x % 2) {
						color_idx = (color_idx & 0xf0) >> 4;
					} else {
						color_idx = color_idx & 0x0f;
					}

					uint16_t color = colors[color_idx];
					if (color == 0) {
						a->flags &= ~0x10;
					}

					uint8_t alpha = ((color >> 15) & 0x0001) * 255;
					uint8_t r = ((float)((color >> 0) & 0x1f) / 31.0f) * 255.0f;
					uint8_t g = ((float)((color >> 5) & 0x1f) / 31.0f) * 255.0f;
					uint8_t b = ((float)((color >> 10) & 0x1f) / 31.0f) * 255.0f;

					// if input was 16 for a channel, force it to 128 to smooth sky texture edge transitions out
					if (r == 131) {
						r = 128;
					}
					if (g == 131) {
						g = 128;
					}
					if (b == 131) {
						b = 128;
					}

					buf[(y * a->buf_width) + x] = r << 0 | g << 8 | b << 16 | alpha << 24;
				}
			}
		} else if (*(uint32_t *)((uint8_t *)a->palette + 4) == 0x100) {
			// 8-bit
			for (int y = 0; y < a->height; y++) {
				for (int x = 0; x < a->width; x++) {
					uint8_t color_idx = ((uint8_t *)a->img_data)[(y * a->width) + x];

					uint16_t color = colors[color_idx];
					if (color == 0) {
						a->flags &= ~0x10;
					}

					uint8_t alpha = ((color >> 15) & 0x0001) * 255;
					uint8_t r = ((float)((color >> 0) & 0x1f) / 31.0f) * 255.0f;
					uint8_t g = ((float)((color >> 5) & 0x1f) / 31.0f) * 255.0f;
					uint8_t b = ((float)((color >> 10) & 0x1f) / 31.0f) * 255.0f;

					// if input was 16 for a channel, force it to 128 to smooth sky texture edge transitions out
					if (r == 131) {
						r = 128;
					}
					if (g == 131) {
						g = 128;
					}
					if (b == 131) {
						b = 128;
					}

					buf[(y * a->buf_width) + x] = r << 0 | g << 8 | b << 16 | alpha << 24;
				}
			}
		} else {
			printf("TEST!\n");
		}
			
	} else {
		printf("TEXTURE NOT PALETTED\n");
	}

	updateTextureEntry(renderer, a->idx, a->buf_width, a->buf_height, buf);

	free(buf);
}

void __stdcall freeD3DTexture(int idx) {
	destroyTextureEntry(renderer, idx);
}

void *D3DTEX_GetTexturePalette(void *a) {
	printf("STUB: D3DTEX_GetTexturePalette\n");

	return 0;
}

int dummypalette[128];

void *D3DTEX_GetPalette(void *a) {
	printf("STUB: D3DTEX_GetPalette\n");

	return dummypalette;
}

void D3DTEX_SetPalette(void *a, void *b) {
	printf("STUB: D3DTEX_SetPalette\n");
}

int D3DTEX_TextureCountColors(struct texture *a) {
	uint8_t counted[256];
	memset(counted, 0, 256);
	int count = 0;

	if (!(a->flags & 8)) {
		uint16_t *colors = *(uint16_t **)((uint8_t *)a->palette + 8);
		a->flags |= 0x10;
			
		if (*(uint32_t *)((uint8_t *)a->palette + 4) == 0x10) {
			// 4-bit
			int linewidth = (a->width + (a->width % 2)) / 2;
			linewidth += linewidth % 2;
			for (int y = 0; y < a->height; y++) {
				for (int x = 0; x < a->width; x++) {
					uint32_t pixel_idx = ((y * linewidth) + (x / 2));
					uint8_t color_idx = ((uint8_t *)a->img_data)[pixel_idx];

					if (x % 2) {
						color_idx = (color_idx & 0xf0) >> 4;
					} else {
						color_idx = color_idx & 0x0f;
					}

					if (!counted[color_idx]) {
						counted[color_idx] = 1;
						count++;
					}
				}
			}
		} else {
			// 8-bit
			for (int y = 0; y < a->height; y++) {
				for (int x = 0; x < a->width; x++) {
					uint8_t color_idx = ((uint8_t *)a->img_data)[(y * a->width) + x];

					if (!counted[color_idx]) {
						counted[color_idx] = 1;
						count++;
					}
				}
			}
		}
			
	}

	return count;
}

void D3DTEX_GrayTexture(struct texture *a) {
	// NOTE: luma is intentionally crushed here to match original PS1/PC behavior
	uint32_t *buf = malloc(sizeof(uint32_t) * a->buf_width * a->buf_height);

	for (int y = 0; y < a->buf_height; y++) {
		for (int x = 0; x < a->buf_width; x++) {
			//buf[(y * a->buf_width) + x] = ((x + y) % 2) ? colordark : colorlight;
			buf[(y * a->buf_width) + x] = 0;
		}
	}

	if (!(a->flags & 8)) {
		uint16_t *colors = *(uint16_t **)((uint8_t *)a->palette + 8);
		a->flags |= 0x10;
			
		if (*(uint32_t *)((uint8_t *)a->palette + 4) == 0x10) {
			// 4-bit
			int linewidth = (a->width + (a->width % 2)) / 2;
			linewidth += linewidth % 2;
			for (int y = 0; y < a->height; y++) {
				for (int x = 0; x < a->width; x++) {
					uint32_t pixel_idx = ((y * linewidth) + (x / 2));
					uint8_t color_idx = ((uint8_t *)a->img_data)[pixel_idx];

					if (x % 2) {
						color_idx = (color_idx & 0xf0) >> 4;
					} else {
						color_idx = color_idx & 0x0f;
					}

					uint16_t color = colors[color_idx];
					if (color == 0) {
						a->flags &= ~0x10;
					}

					uint8_t alpha = ((color >> 15) & 0x0001) * 255;
					uint32_t r = (color >> 0) & 0x1f;
					uint32_t g = (color >> 5) & 0x1f;
					uint32_t b = (color >> 10) & 0x1f;

					uint8_t luma = ((r + g + b) * 0x226) / 3000;

					luma = ((float)luma / 31.0f) * 255.0f;

					buf[(y * a->buf_width) + x] = luma << 0 | luma << 8 | luma << 16 | alpha << 24;
				}
			}
		} else if (*(uint32_t *)((uint8_t *)a->palette + 4) == 0x100) {
			// 8-bit
			for (int y = 0; y < a->height; y++) {
				for (int x = 0; x < a->width; x++) {
					uint8_t color_idx = ((uint8_t *)a->img_data)[(y * a->width) + x];

					uint16_t color = colors[color_idx];
					if (color == 0) {
						a->flags &= ~0x10;
					}

					uint8_t alpha = ((color >> 15) & 0x0001) * 255;
					uint32_t r = (color >> 0) & 0x1f;
					uint32_t g = (color >> 5) & 0x1f;
					uint32_t b = (color >> 10) & 0x1f;

					uint8_t luma = ((r + g + b) * 0x226) / 3000;

					luma = ((float)luma / 31.0f) * 255.0f;

					buf[(y * a->buf_width) + x] = luma << 0 | luma << 8 | luma << 16 | alpha << 24;
				}
			}
		}	
	}

	updateTextureEntry(renderer, a->idx, a->buf_width, a->buf_height, buf);

	free(buf);
}

// why does this store so many textures
typedef struct {
	struct texture *texture;
	struct texture *unk_texture;
	int *dimensions;
	int texture_count;
	struct texture *textures;	// not actually texture but just an identically sized thing used when rendering.  weird.
	float unk_float;
	uint8_t *data;
} D3DSprite;

void *__fastcall D3DSprite_D3DSprite(D3DSprite *sprite, void *pad, int a, char *b, char c) {
	void *(__cdecl *operator_new)(int sz) = 0x004fd32e;
	void (__cdecl *operator_delete)(void *) = 0x004fd323;
	void (__cdecl *D3DTEX_FreePaletteEntry)(void *, int) = 0x004d7680;
	void (__cdecl *mem_delete)(void *) = 0x0046f460;

	sprite->data = NULL;

	// presumably depth
	if (c) {
		D3DSprite **gBackground = 0x0069d118;
		*gBackground = sprite;
		sprite->unk_float = 0.01f;
	} else {
		sprite->unk_float = 0.9f;
	}

	sprite->texture = operator_new(sizeof(struct texture));
	memset(sprite->texture, 0, sizeof(struct texture));

	struct texture *(__cdecl *D3DTEX_AddToTextureList3)(int a, int b, int c, int d) = 0x004d6b40;
	sprite->texture = D3DTEX_AddToTextureList3(a, sprite->texture, 1, b);

	//printf("texture info %d, %d, %d, 0x%08x 0x%08x 0x%08x\n", sprite->texture->width, sprite->texture->height, sprite->texture->idx, sprite->texture->flags, sprite->texture->unk[0], sprite->texture->unk[1]);
	sprite->texture->flags |= 0x01000000;	// hack for drawing: don't multiply texture color 2x

	if (!(sprite->texture->flags & 8) && sprite->texture->img_data) {
		operator_delete(sprite->texture->img_data);
		sprite->texture->img_data = NULL;
		sprite->texture->flags = sprite->texture->flags & ~0x01;
	}

	D3DTEX_FreePaletteEntry(sprite->texture->palette, 1);
	sprite->texture->palette = NULL;
	sprite->texture->flags = sprite->texture->flags & ~0x20;

	mem_delete(a);

	// create the texture
	sprite->texture_count = 1;

	sprite->textures = operator_new(0x2c * sprite->texture_count);
	memset(sprite->textures, 0, 0x2c * sprite->texture_count);

	sprite->unk_texture = NULL;

	sprite->dimensions = operator_new(sizeof(int) * 4);
	sprite->dimensions[0] = 0;
	sprite->dimensions[1] = 0;
	sprite->dimensions[2] = sprite->texture->width;
	sprite->dimensions[3] = sprite->texture->height;

	((uint32_t *)sprite->textures)[5] = sprite->texture;

	return sprite;
}

void __fastcall D3DSprite_Draw(void *sprite, void *pad, int a, float b, float c) {
	//printf("STUB: D3DSprite::Draw: 0x%08x, %f, %f\n", a, b, c);
}

void __fastcall D3DSprite_Destroy(D3DSprite *sprite) {
	void (__cdecl *operator_delete)(void *) = 0x004fd323;

	operator_delete(sprite->dimensions);
	operator_delete(sprite->textures);
	freeD3DTexture(sprite->texture->idx);
	operator_delete(sprite->texture);
}

void WINMAIN_SwitchResolution() {
	//printf("STUB: WINMAIN_SwitchResolution\n");

	int *width = 0x029d6fe4;
	int *height = 0x029d6fe8;

	*width = resolution_x;
	*height = resolution_y;

	setRenderResolution(renderer, *width, *height, aspectRatio);

	/*int *fog = 0x0054546c;
	float *fog2 = 0x00545334;

	*fog = 10000;
	*fog2 = 10000.0f;*/
}

uint16_t PixelAspectYFov = 0x1000;

void m3dinit_setresolution() {
	int *width = 0x029d6fe4;
	int *height = 0x029d6fe8;

	uint16_t *PixelAspectX = 0x005606cc;
	uint16_t *PixelAspectY = 0x005606d0;
	uint16_t *ResX = 0x0055ed00;
	uint16_t *ResY = 0x0055ed18;

	*ResX = *width;
	*ResY = *height;

	*PixelAspectX = 0x1000;
	*PixelAspectY = ((*ResX * 0x3000) / (*ResY * 4));
	//PixelAspectYFov = (float)*PixelAspectY / ((float)*ResY / (float)*ResX) / (3.0f / 4.0f);
	//PixelAspectYFov = ((640 * 0x3000) / (480 * 4));

	PixelAspectYFov = 4096.0f * (((float)*ResY / (float)*ResX) / (1.0f / aspectRatio));
}

void openExternalTexture(void *a, struct texture *b) {
	// dummy version of this function to cancel any high res loads

	return -1;
}

void WINMAIN_Configure() {
	printf("STUB: WINMAIN_Configure\n");

	float *VideoFogYonScale = 0x00545334;
	*VideoFogYonScale = 1.5f;
}

void SaveVidConfig() {
	printf("STUB: SaveVidConfig\n");
}

void toGameScreenCoord(int x, int y, int *xOut, int *yOut) {
	// converts from window coord to game coord
	int winwidth, winheight;
	getWindowSize(&winwidth, &winheight);

	float fx, fy;
	fx = (float)x / (float)winwidth;
	fy = (float)y / (float)winheight;

	float windowaspect = (float)winwidth / (float)winheight;

	if (windowaspect >= aspectRatio) {
		float aspectConv = windowaspect / aspectRatio;
		fx *= aspectConv;

		fx -= 0.5f * (aspectConv - 1.0f);
	} else {
		float aspectConv = aspectRatio / windowaspect;
		fy *= aspectConv;

		fy -= 0.5f * (aspectConv - 1.0f);
	}

	*xOut = (fx * (float)resolution_x) + 0.5f;
	*yOut = (fy * (float)resolution_y) + 0.5f;
}

void getGameResolution(int *w, int *h) {
	*w = resolution_x;
	*h = resolution_y;
}

void MENUPC_DrawMouseCursor() {

}

void installGfxPatches() {
	patchJmp(0x004f5190, initDDraw);
	patchJmp(0x004f41c0, initD3D);
	
	patchJmp(0x004ce0d0, D3D_ClearBuffers);
	
	patchJmp(0x004d13f0, D3DPOLY_Init);
	patchJmp(0x004d0d50, D3DPOLY_StartScene);
	patchJmp(0x004d0370, D3DPOLY_DrawOTag);
	patchJmp(0x004cde40, D3D_EndSceneAndFlip);
	
	patchJmp(0x004d7a00, D3DTEX_Init);
	//patchJmp(0x004d5ec0, D3DTEX_AddToTextureList);
	//patchJmp(0x004d6ac0, D3DTEX_AddToTextureList2);
	//patchJmp(0x004d6b40, D3DTEX_AddToTextureList3);
	//patchJmp(0x004d7110, D3DTEX_GetTexturePalette);
	//patchJmp(0x004d7750, D3DTEX_GetPalette);
	//patchJmp(0x004d77e0, D3DTEX_SetPalette);
	patchJmp(0x004d7430, D3DTEX_TextureCountColors);
	patchJmp(0x004d7120, D3DTEX_GrayTexture);
	patchJmp(0x004d7940, D3DTEX_ConvertTexturePalette);
	patchJmp(0x004d6100, makeTextureListEntry);
	//patchJmp(0x004d5d40, freeD3DTexture);
	patchJmp(0x004d5fe0, openExternalTexture);

	// patch freeD3DTexture to call our code
	patchNop(0x004d5d4c, 69);
	patchByte(0x004d5d4c, 0x50);	// PUSH EAX
	patchCall(0x004d5d4c + 1, freeD3DTexture);

	// remove DX usage in AddToTextureList3
	patchByte(0x004d6d1a, 0xEB);
	patchByte(0x004d6d32, 0xEB);

	patchJmp(0x004d46b0, D3DSprite_D3DSprite);
	//patchJmp(0x004d4ba0, D3DSprite_Draw);
	patchJmp(0x004d4ad0, D3DSprite_Destroy);
	
	patchJmp(0x004f3f10, WINMAIN_SwitchResolution);
	patchJmp(0x004cc240, WINMAIN_Configure);
	patchJmp(0x004cc510, SaveVidConfig);

	patchJmp(0x00464620, m3dinit_setresolution);
	patchDWord(0x0045e9e9 + 2, &PixelAspectYFov);

	//patchByte(0x004ced21, 0xEB);
	patchNop(0x004ced21, 2);	// don't brighten sky dome in nyc
	patchByte(0x004ced26, 0xEB);	// don't brighten sky in philadelphia
	patchNop(0x004cde20, 2);	// fix philadelphia clear color

	//patchNop(0x004cde18, 3);
	//patchNop(0x004cf4c2, 5);	// enabling this makes THPS sign in philadelpha opaque

	// disable palette conversion
	patchNop(0x004d7887, 1);
	patchByte(0x004d7887 + 1, 0xe9);

	// don't show in-game cursor
	patchJmp(0x004d9060, MENUPC_DrawMouseCursor);

	// pal_loadpalette - don't mess with alpha

	//patchByte(0x00488216, 0xeb);
	//patchNop(0x00488218, 6);
	//patchByte(0x00488234, 0xeb);
	//patchByte(0x004881f8, 0xeb);

	//patchDWord(0x00449c48 + 3, 0x808080ff);

	installMoviePatches();
}