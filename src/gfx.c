/*struct D3D7Device {
	void *QueryInterface;
	void *AddRef;
	void *Release;
	void *GetCaps;
	void *EnumTextureFormats;
	void *BeginScene;
	void *EndScene;
	void *GetDirect3D;
	void *SetRenderTarget;
	void *GetRenderTarget;
	void *Clear;
	void *SetTransform;
	void *GetTransform;
	void *SetViewport;
	void *MultiplyTransform;
	void *GetViewport;
	void *SetMaterial;
	void *GetMaterial;
	void *SetLight;
	void *GetLight;
	void *SetRenderState;
	void *GetRenderState;
	void *BeginStateBlock;
	void *EndStateBlock;
	void *PreLoad;
	void *DrawPrimitive;
	void *DrawIndexedPrimitive;
	void *SetClipStatus;
	void *GetClipStatus;
	void *DrawPrimitiveStrided;
	void *DrawIndexedPrimitiveStrided;
	void *DrawPrimitiveVB;
	void *DrawIndexedPrimitiveVB;
	void *ComputeSphereVisibility;
	void *GetTexture;
	void *SetTexture;
	void *GetTextureStageState;
	void *SetTextureStageState;
	void *ValidateDevice;
	void *ApplyStateBlock;
	void *CaptureStateBlock;
	void *DeleteStateBlock;
	void *CreateStateBlock;
	void *Load;
	void *LightEnable;
	void *GetLightEnable;
	void *SetClipPlane;
	void *GetClipPlane;
	void *GetInfo;
};

int __fastcall D3D7Dev_QueryInterface(struct D3D7Device *dev, void *pad, int riid, void *ppvObj) {
	printf("QueryInterface() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_AddRef(struct D3D7Device *dev) {
	printf("AddRef() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_Release(struct D3D7Device *dev) {
	printf("Release() STUB\n");

	free(dev);

	return 0;
}

int __fastcall D3D7Dev_GetCaps(struct D3D7Device *dev, void *pad, void *desc) {
	printf("GetCaps() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_EnumTextureFormats(struct D3D7Device *dev, void *pad, void *desc, void *a) {
	printf("GetCaps() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_BeginScene(struct D3D7Device *dev) {
	printf("BeginScene() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_EndScene(struct D3D7Device *dev) {
	printf("EndScene() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetDirect3D(struct D3D7Device *dev, void *pad, void *desc) {
	printf("GetDirect3D() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetRenderTarget(struct D3D7Device *dev, void *pad, void *desc, int a) {
	printf("SetRenderTarget() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetRenderTarget(struct D3D7Device *dev, void *pad, void *desc) {
	printf("GetRenderTarget() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_Clear(struct D3D7Device *dev, void *pad, int a, int rect, int b, int color, int value, int c) {
	printf("Clear() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetTransform(struct D3D7Device *dev, void *pad, void *statetype, void *matrix) {
	printf("SetTransform() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetTransform(struct D3D7Device *dev, void *pad, void *statetype, void *matrix) {
	printf("GetTransform() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetViewport(struct D3D7Device *dev, void *pad, void *viewport) {
	printf("SetViewport() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_MultiplyTransform(struct D3D7Device *dev, void *pad, void *statetype, void *matrix) {
	printf("MultiplyTransform() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetViewport(struct D3D7Device *dev, void *pad, void *viewport) {
	printf("GetViewport() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetMaterial(struct D3D7Device *dev, void *pad, void *material) {
	printf("SetMaterial() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetMaterial(struct D3D7Device *dev, void *pad, void *material) {
	printf("GetMaterial() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetLight(struct D3D7Device *dev, void *pad, int n, void *light) {
	printf("SetLight() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetLight(struct D3D7Device *dev, void *pad, int n, void *light) {
	printf("GetLight() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetRenderState(struct D3D7Device *dev, void *pad, int type, int val) {
	printf("SetRenderState() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetRenderState(struct D3D7Device *dev, void *pad, int type, int *val) {
	printf("GetRenderState() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_BeginStateBlock(struct D3D7Device *dev, void *pad) {
	printf("BeginStateBlock() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_EndStateBlock(struct D3D7Device *dev, void *pad, void *desc) {
	printf("EndStateBlock() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_PreLoad(struct D3D7Device *dev, void *pad, void *surface) {
	printf("PreLoad() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_DrawPrimitive(struct D3D7Device *dev, void *pad, int type, int a, void *buffer, int b, int c) {
	printf("DrawPrimitive() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_DrawIndexedPrimitive(struct D3D7Device *dev, void *pad, int type, int a, void *buffer, int b, int *idxbuf, int d, int e) {
	printf("DrawIndexedPrimitive() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetClipStatus(struct D3D7Device *dev, void *pad, void *clipstatus) {
	printf("SetClipStatus() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetClipStatus(struct D3D7Device *dev, void *pad, void *clipstatus) {
	printf("GetClipStatus() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_DrawPrimitiveStrided(struct D3D7Device *dev, void *pad, int type, int n, void *stridedData, int a, int b) {
	printf("DrawPrimitiveStrided() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_DrawIndexedPrimitiveStrided(struct D3D7Device *dev, void *pad, int type, int a, void *stridedData, int b, int *idxbuf, int d, int e) {
	printf("DrawIndexedPrimitiveStrided() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_DrawPrimitiveVB(struct D3D7Device *dev, void *pad, int type, void *vertexBuffer, int n, int a, int b) {
	printf("DrawPrimitiveVB() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_DrawIndexedPrimitiveVB(struct D3D7Device *dev, void *pad, int type, void *vertexBuffer, int n, int a, void *idxbuf, int b, int c) {
	printf("DrawIndexedPrimitiveVB() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_ComputeSphereVisibility(struct D3D7Device *dev, void *pad, void *vector, void *value, int a, int b, int *out) {
	printf("ComputeSphereVisibility() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetTexture(struct D3D7Device *dev, void *pad, int n, void *texture) {
	printf("GetTexture() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetTexture(struct D3D7Device *dev, void *pad, int n, void *texture) {
	printf("SetTexture() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetTextureStageState(struct D3D7Device *dev, void *pad, int stage, int type, int *val) {
	printf("GetTextureStageState() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetTextureStageState(struct D3D7Device *dev, void *pad, int stage, int type, int val) {
	printf("SetTextureStageState() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_ValidateDevice(struct D3D7Device *dev, void *pad, int *out) {
	printf("ValidateDevice() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_ApplyStateBlock(struct D3D7Device *dev, void *pad, int block) {
	printf("ApplyStateBlock() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_CaptureStateBlock(struct D3D7Device *dev, void *pad, int block) {
	printf("CaptureStateBlock() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_DeleteStateBlock(struct D3D7Device *dev, void *pad, int block) {
	printf("DeleteStateBlock() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_CreateStateBlock(struct D3D7Device *dev, void *pad, void *type, int *block) {
	printf("CreateStateBlock() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_Load(struct D3D7Device *dev, void *pad, void *surface, void *point, void *surface2, void *rect, int a) {
	printf("Load() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_LightEnable(struct D3D7Device *dev, void *pad, int n, int state) {
	printf("LightEnable() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetLightEnable(struct D3D7Device *dev, void *pad, int n, int *state) {
	printf("GetLightEnable() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_SetClipPlane(struct D3D7Device *dev, void *pad, int a, void *value) {
	printf("SetClipPlane() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetClipPlane(struct D3D7Device *dev, void *pad, int a, void *value) {
	printf("GetClipPlane() STUB\n");

	return 0;
}

int __fastcall D3D7Dev_GetInfo(struct D3D7Device *dev, void *pad, int a, void *b, int c) {
	printf("GetInfo() STUB\n");

	return 0;
}

struct D3D7Device *createD3d7Device() {
	struct D3D7Device *result = malloc(sizeof(struct D3D7Device));

	*result = (struct D3D7Device) {
		D3D7Dev_QueryInterface,
		D3D7Dev_AddRef,
		D3D7Dev_Release,
		D3D7Dev_GetCaps,
		D3D7Dev_EnumTextureFormats,
		D3D7Dev_BeginScene,
		D3D7Dev_EndScene,
		D3D7Dev_GetDirect3D,
		D3D7Dev_SetRenderTarget,
		D3D7Dev_GetRenderTarget,
		D3D7Dev_Clear,
		D3D7Dev_SetTransform,
		D3D7Dev_GetTransform,
		D3D7Dev_SetViewport,
		D3D7Dev_MultiplyTransform,
		D3D7Dev_GetViewport,
		D3D7Dev_SetMaterial,
		D3D7Dev_GetMaterial,
		D3D7Dev_SetLight,
		D3D7Dev_GetLight,
		D3D7Dev_SetRenderState,
		D3D7Dev_GetRenderState,
		D3D7Dev_BeginStateBlock,
		D3D7Dev_EndStateBlock,
		D3D7Dev_PreLoad,
		D3D7Dev_DrawPrimitive,
		D3D7Dev_DrawIndexedPrimitive,
		D3D7Dev_SetClipStatus,
		D3D7Dev_GetClipStatus,
		D3D7Dev_DrawPrimitiveStrided,
		D3D7Dev_DrawIndexedPrimitiveStrided,
		D3D7Dev_DrawPrimitiveVB,
		D3D7Dev_DrawIndexedPrimitiveVB,
		D3D7Dev_ComputeSphereVisibility,
		D3D7Dev_GetTexture,
		D3D7Dev_SetTexture,
		D3D7Dev_GetTextureStageState,
		D3D7Dev_SetTextureStageState,
		D3D7Dev_ValidateDevice,
		D3D7Dev_ApplyStateBlock,
		D3D7Dev_CaptureStateBlock,
		D3D7Dev_DeleteStateBlock,
		D3D7Dev_CreateStateBlock,
		D3D7Dev_Load,
		D3D7Dev_LightEnable,
		D3D7Dev_GetLightEnable,
		D3D7Dev_SetClipPlane,
		D3D7Dev_GetClipPlane,
		D3D7Dev_GetInfo,
	};

	return result;
}

struct D3D7 {
	void *QueryInterface;
	void *AddRef;
	void *Release;
	void *EnumDevices;
	void *CreateDevice;
	void *CreateVertexBuffer;
	void *EnumZBufferFormats;
	void *EvictManagedTextures;
};

int __fastcall D3D7_QueryInterface(struct D3D7 *d3d7, void *pad, int riid, void *ppvObj) {
	printf("D3D7 QueryInterface() STUB\n");

	return 0;
}

int __fastcall D3D7_AddRef(struct D3D7 *d3d7) {
	printf("D3D7 AddRef() STUB\n");

	return 0;
}

int __fastcall D3D7_Release(struct D3D7 *d3d7) {
	printf("D3D7 Release() STUB\n");

	free(d3d7);

	return 0;
}

int __fastcall D3D7_EnumDevices(struct D3D7 *d3d7, void *pad, void *callback, void *data) {
	printf("D3D7 EnumDevices() STUB\n");

	return 0;
}

int __fastcall D3D7_CreateDevice(struct D3D7 *d3d7, void *pad, int refclsid, void *surface, struct D3D7Device **device) {
	printf("D3D7 CreateDevice() STUB\n");

	*device = createD3d7Device();

	return 0;
}

int __fastcall D3D7_CreateVertexBuffer(struct D3D7 *d3d7, void *pad, void *bufferdesc, void *buffer, int a) {
	printf("D3D7 CreateVertexBuffer() STUB\n");

	return 0;
}

int __fastcall D3D7_EnumZBufferFormats(struct D3D7 *d3d7, void *pad, int refclsid, void *callback, void *data) {
	printf("D3D7 EnumZBufferFormats() STUB\n");

	return 0;
}

int __fastcall D3D7_EvictManagedTextures(struct D3D7 *d3d7) {
	printf("D3D7 EvictManagedTextures() STUB\n");

	return 0;
}

struct D3D7 *createD3d7Device() {
	struct D3D7 *result = malloc(sizeof(struct D3D7));

	*result = (struct D3D7) {
		D3D7_QueryInterface,
		D3D7_AddRef,
		D3D7_Release,
		D3D7_EnumDevices,
		D3D7_CreateDevice,
		D3D7_CreateVertexBuffer,
		D3D7_EnumZBufferFormats,
		D3D7_EvictManagedTextures,
	};

	return result;
}

struct DDraw7 {
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;
    int (*Compact)(THIS) PURE;
    int (*CreateClipper)(THIS_ DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR * ) PURE;
    int (*CreatePalette)(THIS_ DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR*, IUnknown FAR * ) PURE;
    int (*CreateSurface)(THIS_  LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE7 FAR *, IUnknown FAR *) PURE;
    int (*DuplicateSurface)( THIS_ LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 FAR * ) PURE;
    int (*EnumDisplayModes)( THIS_ DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2 ) PURE;
    int (*EnumSurfaces)(THIS_ DWORD, LPDDSURFACEDESC2, LPVOID,LPDDENUMSURFACESCALLBACK7 ) PURE;
    int (*FlipToGDISurface)(THIS) PURE;
    int (*GetCaps)( THIS_ LPDDCAPS, LPDDCAPS) PURE;
    int (*GetDisplayMode)( THIS_ LPDDSURFACEDESC2) PURE;
    int (*GetFourCCCodes)(THIS_  LPDWORD, LPDWORD ) PURE;
    int (*GetGDISurface)(THIS_ LPDIRECTDRAWSURFACE7 FAR *) PURE;
    int (*GetMonitorFrequency)(THIS_ LPDWORD) PURE;
    int (*GetScanLine)(THIS_ LPDWORD) PURE;
    int (*GetVerticalBlankStatus)(THIS_ LPBOOL ) PURE;
    int (*Initialize)(THIS_ GUID FAR *) PURE;
    int (*RestoreDisplayMode)(THIS) PURE;
    int (*SetCooperativeLevel)(THIS_ HWND, DWORD) PURE;
    int (*SetDisplayMode)(THIS_ DWORD, DWORD,DWORD, DWORD, DWORD) PURE;
    int (*WaitForVerticalBlank)(THIS_ DWORD, HANDLE ) PURE;
    int (*GetAvailableVidMem)(THIS_ LPDDSCAPS2, LPDWORD, LPDWORD) PURE;
    int (*GetSurfaceFromDC) (THIS_ HDC, LPDIRECTDRAWSURFACE7 *) PURE;
    int (*RestoreAllSurfaces)(THIS) PURE;
    int (*TestCooperativeLevel)(THIS) PURE;
    int (*GetDeviceIdentifier)(THIS_ LPDDDEVICEIDENTIFIER2, DWORD ) PURE;
    int (*StartModeTest)(THIS_ LPSIZE, DWORD, DWORD ) PURE;
    int (*EvaluateMode)(THIS_ DWORD, DWORD * ) PURE;
};

int createDirectDrawWrapper(void *a, void *b, void *c, void *d) {
	return 0;
}

void patchD3D() {
	patchCall(0x004f5310, createDirectDrawWrapper);
}
*/

#include <stdio.h>
#include <stdint.h>

#include <patch.h>
#include <global.h>
#include <gfx/vk/gfx_vk.h>

partyRenderer *renderer = NULL;

void initDDraw() {
	printf("STUB: initDDraw\n");

	int *width = 0x029d6fe4;
	int *height = 0x029d6fe8;

	*width = 640;
	*height = 480;
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
}

void D3D_ClearBuffers() {
	printf("STUB: D3D_ClearBuffers\n");
}

void D3DPOLY_Init() {
	printf("STUB: D3DPOLY_Init\n");
}

uint32_t fixDXColor(uint32_t color) {
	return ((color & 0x00FF0000) >> 16) + ((color & 0x0000FF00) >> 0) + ((color & 0x000000FF) << 16) + ((color & 0xFF000000));
}

void D3DPOLY_StartScene(int a, int b) {
	void (__cdecl *setupFog)(int) = 0x004d1160;
	uint32_t *viewportClass = 0x00560698;
	uint16_t *DpqMin = 0x005606a4;
	uint16_t *DpqMinMaybe = *viewportClass + 8;
	uint16_t *DpqMaxMaybe = *viewportClass + 10;
	float *fogThreshold = 0x00546b3c;
	float *FogYonScale = 0x00546b38;
	float *VideoFogYonScale = 0x00545334;
	uint32_t gShellMode = 0x006a35b4;

	//printf("STUB: D3DPOLY_StartScene: 0x%08x 0x%08x\n", a, b);

	//float *fog = 0x00546b38;
	//*fog = 10000.0f;

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

	startRender(renderer, clearColor);
}

void (*D3DPOLY_EndScene)() = 0x004d12e0;

void D3D_EndSceneAndFlip() {
	D3DPOLY_EndScene();
	//printf("STUB: D3D_EndSceneAndFlip\n");

	finishRender(renderer);
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
			buf[outputVert].z = 1.0 - vertices[0].z;
			buf[outputVert].u = 0.0f;
			buf[outputVert].v = 0.0f;
			buf[outputVert].color = fixDXColor(vertices[0].color);

			buf[outputVert + 1].x = vertices[i].x;
			buf[outputVert + 1].y = vertices[i].y;
			buf[outputVert + 1].z = 1.0 - vertices[i].z;
			buf[outputVert + 1].u = 0.0f;
			buf[outputVert + 1].v = 0.0f;
			buf[outputVert + 1].color = fixDXColor(vertices[i].color);

			buf[outputVert + 2].x = vertices[i + 1].x;
			buf[outputVert + 2].y = vertices[i + 1].y;
			buf[outputVert + 2].z = 1.0 - vertices[i + 1].z;
			buf[outputVert + 2].u = 0.0f;
			buf[outputVert + 2].v = 0.0f;
			buf[outputVert + 2].color = fixDXColor(vertices[i + 1].color);

			outputVert += 3;
		} 

		transformDXCoords(buf, outputVert);
		drawVertices(renderer, buf, outputVert);
	} else {
		struct dxpolytextured *vertices = ((uint8_t *)tag + 0x18);
		uint32_t numVerts = *(uint32_t *)((uint8_t *)tag + 0x14);

		if (!(*(uint8_t *)(*(uint32_t *)(*(uint32_t *)((uint8_t *)tag + 0x10) + 0x14) + 0x10) & 0x10)) {
			printf("TEST???\n");
			setDepthState(renderer, 1, 0);
		}

		// calc final colors
		for (int i = 0; i < numVerts; i++) {
			// apply alpha from blend mode
			uint32_t newcolor = (vertices[i].color & 0x00ffffff) | alpha;

			if (!(*(uint32_t *)((uint8_t *)tag + 8) & 0x80000000)) {
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
			buf[outputVert].z = 1.0 - vertices[0].z;
			buf[outputVert].u = vertices[0].u;
			buf[outputVert].v = vertices[0].v;
			buf[outputVert].color = fixDXColor(vertices[0].color);

			buf[outputVert + 1].x = vertices[i].x;
			buf[outputVert + 1].y = vertices[i].y;
			buf[outputVert + 1].z = 1.0 - vertices[i].z;
			buf[outputVert + 1].u = vertices[i].u;
			buf[outputVert + 1].v = vertices[i].v;
			buf[outputVert + 1].color = fixDXColor(vertices[i].color);

			buf[outputVert + 2].x = vertices[i + 1].x;
			buf[outputVert + 2].y = vertices[i + 1].y;
			buf[outputVert + 2].z = 1.0 - vertices[i + 1].z;
			buf[outputVert + 2].u = vertices[i + 1].u;
			buf[outputVert + 2].v = vertices[i + 1].v;
			buf[outputVert + 2].color = fixDXColor(vertices[i + 1].color);

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

	float xmult = (1.0f / (float)*screen_width) * 2.0f;
	float ymult = (1.0f / (float)*screen_height) * 2.0f;
	float xdisp = 1.0f - (xmult * 0.5);
	float ydisp = 1.0f - (ymult * 0.5);

	// first, check that the y position is on-screen
	uint8_t on_screen = 0;

	for (int i = 0; i < count; i++) {
		if (vertices[i].y <= (float)*screen_height) {
			on_screen = 1;
		}
	}

	for (int i = 0; i < count; i++) {
		vertices[i].x = (vertices[i].x * xmult) - xdisp;

		if (vertices[i].y > 4368.0f - (float)*screen_height && on_screen) {
			vertices[i].y -= 4368.0f;
		}

		vertices[i].y = (vertices[i].y * ymult) - ydisp;
	}
}

float fixZ(float z) {
	if (z < 0.0f) {
		setDepthState(renderer, 0, 0);

		z = 0.0f;
	} else {
		z = 1.0f - z;
	}

	return z;
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

	//printf("drawing quad (%d, %d), (%d, %d), (%d, %d), (%d, %d), z %f (0x%08x)\n", x1, y1, x2, y2, x3, y3, x4, y4, z, zi);

	renderVertex vertices[2];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };

	transformCoords(vertices, 2);

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

	//printf("drawing quad (%d, %d), (%d, %d), (%d, %d), (%d, %d), z %f (0x%08x)\n", x1, y1, x2, y2, x3, y3, x4, y4, z, zi);

	renderVertex vertices[4];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[2] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[3] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };

	transformCoords(vertices, 4);

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

	//printf("renderLineF4 %f %d %d %d %d\n", z, x1, y1, x2, y2);

	//printf("drawing quad (%d, %d), (%d, %d), (%d, %d), (%d, %d), z %f (0x%08x)\n", x1, y1, x2, y2, x3, y3, x4, y4, z, zi);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[2] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[3] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 0.0f, 0.0f, color };

	transformCoords(vertices, 6);

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

	//printf("drawing quad (%d, %d), (%d, %d), (%d, %d), (%d, %d), z %f (0x%08x)\n", x1, y1, x2, y2, x3, y3, x4, y4, z, zi);

	renderVertex vertices[2];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color1 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color2 };
	
	transformCoords(vertices, 2);

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

	printf("(%d, %d) (%d, %d) (%d, %d)\n", x1, y1, x2, y2, x3, y3);

	float z = *(float *)((uint8_t *)tag + 20);
	z = fixZ(z);

	renderVertex vertices[3];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };

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

	//printf("drawing quad (%d, %d), (%d, %d), (%d, %d), (%d, %d), z %f (0x%08x)\n", x1, y1, x2, y2, x3, y3, x4, y4, z, zi);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
	vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 0.0f, 0.0f, color };

	if (!validateShard(vertices, 6)) {
		return;
	}

	transformCoords(vertices, 6);

	drawVertices(renderer, vertices, 6);
}

void renderPolyFT3(int *tag) {
	//if (*(int *)((uint8_t *)tag + 36)) {
		//printf("TEST!\n");

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

		//printf("UVS: (%d, %d) (%d, %d) (%d, %d) (%d, %d)\n", u1, v1, u2, v2, u3, v3);
		
		float z = *(float *)((uint8_t *)tag + 32);
		z = fixZ(z);

		renderVertex vertices[3];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };

		if (!validateShard(vertices, 3)) {
			return;
		}

		transformCoords(vertices, 3);

		drawVertices(renderer, vertices, 3);
	//}
}

void renderPolyFT4(int *tag) {
	//if (*(int *)((uint8_t *)tag + 44)) {
		//printf("TEST!\n");

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

		//printf("UVS: (%d, %d) (%d, %d) (%d, %d) (%d, %d)\n", u1, v1, u2, v2, u3, v3, u4, v4);
		
		float z = *(float *)((uint8_t *)tag + 40);
		z = fixZ(z);

		renderVertex vertices[6];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
		vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
		vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
		vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 0.0f, 0.0f, color };

		if (!validateShard(vertices, 6)) {
			return;
		}

		transformCoords(vertices, 6);

		drawVertices(renderer, vertices, 6);
	//}
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
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color1 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color2 };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color3 };

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
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color1 };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color2 };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color3 };
	vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color2 };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color3 };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 0.0f, 0.0f, color4 };

	if (!validateShard(vertices, 6)) {
		return;
	}

	transformCoords(vertices, 6);

	drawVertices(renderer, vertices, 6);
}

void renderPolyGT3(int *tag) {
	//if (*(int *)((uint8_t *)tag + 44)) {
		//printf("TEST!\n");

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

		//printf("UVS: (%d, %d) (%d, %d) (%d, %d) (%d, %d)\n", u1, v1, u2, v2, u3, v3, u4, v4);
		
		float z = *(float *)((uint8_t *)tag + 40);
		z = fixZ(z);

		renderVertex vertices[6];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color1 };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color2 };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color3 };

		if (!validateShard(vertices, 3)) {
			return;
		}

		transformCoords(vertices, 3);

		drawVertices(renderer, vertices, 3);
	//}
}

void renderPolyGT4(int *tag) {
	//if (*(int *)((uint8_t *)tag + 56)) {
		//printf("TEST!\n");

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

		//printf("UVS: (%d, %d) (%d, %d) (%d, %d) (%d, %d)\n", u1, v1, u2, v2, u3, v3, u4, v4);
		
		float z = *(float *)((uint8_t *)tag + 52);
		z = fixZ(z);

		renderVertex vertices[6];
		vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color1 };
		vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color2 };
		vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color3 };
		vertices[3] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color2 };
		vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color3 };
		vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 0.0f, 0.0f, color4 };

		if (!validateShard(vertices, 6)) {
			return;
		}

		transformCoords(vertices, 6);

		drawVertices(renderer, vertices, 6);
	//}
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
	vertices[0] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
	vertices[1] = (renderVertex) { (float)x2, (float)y2, z, 0.0f, 0.0f, color };
	vertices[2] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
	vertices[3] = (renderVertex) { (float)x1, (float)y1, z, 0.0f, 0.0f, color };
	vertices[4] = (renderVertex) { (float)x3, (float)y3, z, 0.0f, 0.0f, color };
	vertices[5] = (renderVertex) { (float)x4, (float)y4, z, 0.0f, 0.0f, color };

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

	//printf("STUB: D3DPOLY_DrawOTag: 0x%08x\n", tag);

	/*uint8_t isWireframe = (*(uint32_t *)(tag + 8)) & 0x30000000 >> 28;
	if (isWireframe) {
		printf("Draw wireframe!");
		if (!(isWireframe & 0x2)) {
			return;
		}
	}*/

	// call to 4fe396?

	while(tag != NULL) {
		if (tag[1] != 0) {
			uint8_t cmd = *(uint8_t *)((int)tag + 7);
			if (!(cmd & 0x80)) {
				//*nextPSXBlendMode = 0;

				uint8_t blendMode = cmd & 0xfffffffc;
				uint8_t otherBlendMode = cmd & 4;
				if (blendMode == 0x40 || blendMode == 0x48 || blendMode == 0x4c || blendMode == 0x50) {
					otherBlendMode = 0;


				} else if (!otherBlendMode) {
					

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
				printf("UNKNOWN RENDER COMMAND: %d\n", cmd);
			}
		}
		tag = *tag;
	}
}

void *(*createTexture)() = 0x4d6a70;


void D3DTEX_Init() {
	printf("STUB: D3DTEX_Init\n");
}

int D3DTEX_AddToTextureList(int a, int b, int c, char d) {
	printf("STUB: D3DTEX_AddToTextureList\n");

	return 0;
}

int D3DTEX_AddToTextureList2(int a, int b, int c, char d, int e, int f) {
	printf("STUB: D3DTEX_AddToTextureList2: %d, %d, %d, %d, %d, %d\n", a, b, c, d, e, f);

	uint32_t *result = createTexture();
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

void makeTextureListEntry(int **a, int b, int c, int d) {
	printf("STUB: makeTextureListEntry: 0x%08x, 0x%08x, %d, %d\n", a, b, c, d);
	int **palFront = 0x0069d174;

	if (a[8] == NULL) {
		int **idx = palFront;
		while (idx != NULL) {
			if (*idx == b) {
				a[8] = idx;
				break;
			}

			idx = idx[4];
		}

		if (a[8] == NULL) {
			printf("Palette Checksum not found: 0x%08x\n", b);
			a[8] = palFront;
		}
	}
}

void freeD3DTexture(void *a) {

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

int D3DTEX_TextureCountColors(void *a) {
	printf("STUB: D3DTEX_TextureCountColors\n");

	return 1;
}

void D3DTEX_GrayTexture(void *a) {
	printf("STUB: D3DTEX_GrayTexture\n");
}

void *__fastcall D3DSprite_D3DSprite(void *sprite, void *pad, int a, char *b, char c) {
	printf("STUB: D3DSprite: 0x%08x, %s, %d\n", a, b, c);

	void (__cdecl *mem_delete)(void *) = 0x0046f460;
	mem_delete(a);

	return sprite;
}

void __fastcall D3DSprite_Draw(void *sprite, void *pad, int a, float b, float c) {
	printf("STUB: D3DSprite::Draw: 0x%08x, %f, %f\n", a, b, c);
}

void __fastcall D3DSprite_Destroy(void *sprite) {
	printf("STUB: ~D3DSprite\n");
}

void WINMAIN_SwitchResolution() {
	printf("STUB: WINMAIN_SwitchResolution\n");

	int *width = 0x029d6fe4;
	int *height = 0x029d6fe8;

	*width = 640;
	*height = 480;

	setRenderResolution(renderer, *width, *height, 4.0f / 3.0f);

	/*int *fog = 0x0054546c;
	float *fog2 = 0x00545334;

	*fog = 10000;
	*fog2 = 10000.0f;*/
}

void WINMAIN_Configure() {
	printf("STUB: WINMAIN_Configure\n");
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
	patchJmp(0x004d7110, D3DTEX_GetTexturePalette);
	patchJmp(0x004d7750, D3DTEX_GetPalette);
	patchJmp(0x004d77e0, D3DTEX_SetPalette);
	patchJmp(0x004d7430, D3DTEX_TextureCountColors);
	patchJmp(0x004d7120, D3DTEX_GrayTexture);
	patchJmp(0x004d6100, makeTextureListEntry);
	//patchJmp(0x004d5fe0, openExternalTexture);

	// remove DX usage in AddToTextureList3
	patchByte(0x004d6d1a, 0xEB);
	patchByte(0x004d6d32, 0xEB);

	patchJmp(0x004d46b0, D3DSprite_D3DSprite);
	patchJmp(0x004d4ba0, D3DSprite_Draw);
	patchJmp(0x004d4ad0, D3DSprite_Destroy);
	
	patchJmp(0x004f3f10, WINMAIN_SwitchResolution);
	patchJmp(0x004cc240, WINMAIN_Configure);
}