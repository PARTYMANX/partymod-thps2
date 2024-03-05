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

#include <patch.h>
#include <global.h>
#include <gfx/vk/gfx_vk.h>

#include <hash.h>

partyRenderer *renderer = NULL;
map_t *memmap = NULL;

void initDDraw() {
	printf("STUB: initDDraw\n");

	int *width = 0x029d6fe4;
	int *height = 0x029d6fe8;

	*width = 640;
	*height = 480;

	memmap = map_alloc(2048, NULL, NULL);
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

void D3DPOLY_StartScene(int a, int b) {
	printf("STUB: D3DPOLY_StartScene: 0x%08x 0x%08x\n", a, b);

	startRender(renderer);
}

void (*D3DPOLY_EndScene)() = 0x004d12e0;

void D3D_EndSceneAndFlip() {
	D3DPOLY_EndScene();
	printf("STUB: D3D_EndSceneAndFlip\n");

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

void renderDXPoly(int *tag) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	float xmult = (1.0f / (float)*screen_width) * 2.0f;
	float ymult = (1.0f / (float)*screen_height) * 2.0f;

	if (!*(uint32_t *)((uint8_t *)tag + 0x10)) {
		struct dxpoly *vertices = ((uint8_t *)tag + 0x18);
		uint32_t numVerts = *(uint32_t *)((uint8_t *)tag + 0x14);

		//printf("RenderDXPoly with %d vertices!!! 0x%08x\n", numVerts, vertices);

		//printf("1: %f %f %f 0x%08x\n", vertices[0].x, vertices[0].y, vertices[0].z, vertices[0].color);
		//printf("2: %f %f %f 0x%08x\n", vertices[1].x, vertices[1].y, vertices[1].z, vertices[1].color);
		//printf("1: %f\n", ((vertices[0].x / 640.0f) * 2.0f) - 1.0f);

		renderVertex buf[18];

		//numVerts = 3;

		int outputVert = 0;
		for (int i = 1; i < numVerts - 2 + 1; i++) {
			buf[outputVert].x = (vertices[0].x * xmult) - 1.0f;
			buf[outputVert].y = (vertices[0].y * ymult) - 1.0f;
			buf[outputVert].z = vertices[0].z;
			buf[outputVert].u = 0.0f;
			buf[outputVert].v = 0.0f;
			buf[outputVert].color = ((vertices[0].color & 0x00FF0000) >> 16) + (vertices[0].color & 0x0000FF00) + ((vertices[0].color & 0x000000FF) << 16) + (vertices[0].color & 0xFF000000);

			buf[outputVert + 1].x = (vertices[i].x * xmult) - 1.0f;
			buf[outputVert + 1].y = (vertices[i].y * ymult) - 1.0f;
			buf[outputVert + 1].z = vertices[i].z;
			buf[outputVert + 1].u = 0.0f;
			buf[outputVert + 1].v = 0.0f;
			buf[outputVert + 1].color = ((vertices[i].color & 0x00FF0000) >> 16) + (vertices[i].color & 0x0000FF00) + ((vertices[i].color & 0x000000FF) << 16) + (vertices[i].color & 0xFF000000);

			buf[outputVert + 2].x = (vertices[i + 1].x * xmult) - 1.0f;
			buf[outputVert + 2].y = (vertices[i + 1].y * ymult) - 1.0f;
			buf[outputVert + 2].z = vertices[i + 1].z;
			buf[outputVert + 2].u = 0.0f;
			buf[outputVert + 2].v = 0.0f;
			buf[outputVert + 2].color = ((vertices[i + 1].color & 0x00FF0000) >> 16) + (vertices[i + 1].color & 0x0000FF00) + ((vertices[i + 1].color & 0x000000FF) << 16) + (vertices[i + 1].color & 0xFF000000);

			outputVert += 3;
		} 

		drawVertices(renderer, buf, (numVerts - 2) * 3);
	} else {
		struct dxpolytextured *vertices = ((uint8_t *)tag + 0x18);
		uint32_t numVerts = *(uint32_t *)((uint8_t *)tag + 0x14);

		//printf("RenderDXPoly with %d vertices!!! 0x%08x\n", numVerts, vertices);

		//printf("1: %f %f %f 0x%08x\n", vertices[0].x, vertices[0].y, vertices[0].z, vertices[0].color);
		//printf("2: %f %f %f 0x%08x\n", vertices[1].x, vertices[1].y, vertices[1].z, vertices[1].color);
		//printf("1: %f\n", ((vertices[0].x / 640.0f) * 2.0f) - 1.0f);

		renderVertex buf[18];

		//numVerts = 3;

		int outputVert = 0;
		for (int i = 1; i < numVerts - 2 + 1; i++) {
			buf[outputVert].x = (vertices[0].x * xmult) - 1.0f;
			buf[outputVert].y = (vertices[0].y * ymult) - 1.0f;
			buf[outputVert].z = vertices[0].z;
			buf[outputVert].u = vertices[0].u;
			buf[outputVert].v = vertices[0].v;
			buf[outputVert].color = ((vertices[0].color & 0x00FF0000) >> 16) + (vertices[0].color & 0x0000FF00) + ((vertices[0].color & 0x000000FF) << 16) + (vertices[0].color & 0xFF000000);

			buf[outputVert + 1].x = (vertices[i].x * xmult) - 1.0f;
			buf[outputVert + 1].y = (vertices[i].y * ymult) - 1.0f;
			buf[outputVert + 1].z = vertices[i].z;
			buf[outputVert + 1].u = vertices[i].u;
			buf[outputVert + 1].v = vertices[i].v;
			buf[outputVert + 1].color = ((vertices[i].color & 0x00FF0000) >> 16) + (vertices[i].color & 0x0000FF00) + ((vertices[i].color & 0x000000FF) << 16) + (vertices[i].color & 0xFF000000);

			buf[outputVert + 2].x = (vertices[i + 1].x * xmult) - 1.0f;
			buf[outputVert + 2].y = (vertices[i + 1].y * ymult) - 1.0f;
			buf[outputVert + 2].z = vertices[i + 1].z;
			buf[outputVert + 2].u = vertices[i + 1].u;
			buf[outputVert + 2].v = vertices[i + 1].v;
			buf[outputVert + 2].color = ((vertices[i + 1].color & 0x00FF0000) >> 16) + (vertices[i + 1].color & 0x0000FF00) + ((vertices[i + 1].color & 0x000000FF) << 16) + (vertices[i + 1].color & 0xFF000000);

			outputVert += 3;
		} 

		drawVertices(renderer, buf, (numVerts - 2) * 3);
	}
}

uint32_t unormColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return r + (g << 8) + (b << 16) + (a << 24);
}

void renderPolyFT4(int *tag) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	float xmult = (1.0f / (float)*screen_width) * 2.0f;
	float ymult = (1.0f / (float)*screen_height) * 2.0f;

	uint8_t r = *((uint8_t *)tag + 4);
	uint8_t g = *((uint8_t *)tag + 5);
	uint8_t b = *((uint8_t *)tag + 6);
	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;
	alpha = 0xff;
	//printf("ALPHA: 0x%08x\n", *((uint32_t *)0x0069c8b0));

	uint32_t color = r + (g << 8) + (b << 16) + (alpha << 24);
	
	int16_t x1 = *(int16_t *)((uint8_t *)tag + 8);
	int16_t y1 = *(int16_t *)((uint8_t *)tag + 10);
	uint8_t u1 = *(uint8_t *)((uint8_t *)tag + 12);
	uint8_t v1 = *(uint8_t *)((uint8_t *)tag + 13);

	int16_t x2 = *(int16_t *)((uint8_t *)tag + 16);
	int16_t y2 = *(int16_t *)((uint8_t *)tag + 18);
	
	int16_t x3 = *(int16_t *)((uint8_t *)tag + 24);
	int16_t y3 = *(int16_t *)((uint8_t *)tag + 26);
	
	int16_t x4 = *(int16_t *)((uint8_t *)tag + 32);
	int16_t y4 = *(int16_t *)((uint8_t *)tag + 34);

	float z = *(float *)((uint8_t *)tag + 40);
	int zi = *(int *)((uint8_t *)tag + 40);

	z = (z + 1.0f) / 2.0f;

	//printf("drawing quad (%d, %d), (%d, %d), (%d, %d), (%d, %d), z %f (0x%08x)\n", x1, y1, x2, y2, x3, y3, x4, y4, z, zi);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { ((float)x1 * xmult) - 1.0f, ((float)y1 * ymult) - 1.0f, z, 0.0f, 0.0f, color };
	vertices[1] = (renderVertex) { ((float)x2 * xmult) - 1.0f, ((float)y2 * ymult) - 1.0f, z, 0.0f, 0.0f, color };
	vertices[2] = (renderVertex) { ((float)x3 * xmult) - 1.0f, ((float)y3 * ymult) - 1.0f, z, 0.0f, 0.0f, color };
	vertices[3] = (renderVertex) { ((float)x2 * xmult) - 1.0f, ((float)y2 * ymult) - 1.0f, z, 0.0f, 0.0f, color };
	vertices[4] = (renderVertex) { ((float)x3 * xmult) - 1.0f, ((float)y3 * ymult) - 1.0f, z, 0.0f, 0.0f, color };
	vertices[5] = (renderVertex) { ((float)x4 * xmult) - 1.0f, ((float)y4 * ymult) - 1.0f, z, 0.0f, 0.0f, color };

	drawVertices(renderer, vertices, 6);
}

renderPolyG4(int *tag) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	float xmult = (1.0f / (float)*screen_width) * 2.0f;
	float ymult = (1.0f / (float)*screen_height) * 2.0f;

	uint8_t alpha = *((uint32_t *)0x0069c8b0) >> 24;
	alpha = 0xff;
	//printf("ALPHA: 0x%08x\n", *((uint32_t *)0x0069c8b0));

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

	float z = *(float *)((uint8_t *)tag + 40);
	int zi = *(int *)((uint8_t *)tag + 40);

	z = (z + 1.0f) / 2.0f;

	//printf("drawing quad (%d, %d), (%d, %d), (%d, %d), (%d, %d), z %f (0x%08x)\n", x1, y1, x2, y2, x3, y3, x4, y4, z, zi);

	renderVertex vertices[6];
	vertices[0] = (renderVertex) { ((float)x1 * xmult) - 1.0f, ((float)y1 * ymult) - 1.0f, z, 0.0f, 0.0f, color1 };
	vertices[1] = (renderVertex) { ((float)x2 * xmult) - 1.0f, ((float)y2 * ymult) - 1.0f, z, 0.0f, 0.0f, color2 };
	vertices[2] = (renderVertex) { ((float)x3 * xmult) - 1.0f, ((float)y3 * ymult) - 1.0f, z, 0.0f, 0.0f, color3 };
	vertices[3] = (renderVertex) { ((float)x2 * xmult) - 1.0f, ((float)y2 * ymult) - 1.0f, z, 0.0f, 0.0f, color2 };
	vertices[4] = (renderVertex) { ((float)x3 * xmult) - 1.0f, ((float)y3 * ymult) - 1.0f, z, 0.0f, 0.0f, color3 };
	vertices[5] = (renderVertex) { ((float)x4 * xmult) - 1.0f, ((float)y4 * ymult) - 1.0f, z, 0.0f, 0.0f, color4 };

	drawVertices(renderer, vertices, 6);
}

void changeViewport(int *tag) {
	int *screen_width = 0x029d6fe4;
	int *screen_height = 0x029d6fe8;

	// all values are based on the original resolution of 512x240
	float x = (*(int16_t*)((uint8_t*)tag + 8) / 512.0f) * (float)*screen_width;
	float y = (*(int16_t *)((uint8_t *)tag + 10) / 240.0f) * (float)*screen_height;
	float width = (*(int16_t *)((uint8_t *)tag + 12) / 512.0f) * (float)*screen_width;
	float height = (*(int16_t *)((uint8_t *)tag + 14) / 240.0f) * (float)*screen_height;

	if (y >= 512.0f) {
		y -= 512.0f;
	}

	if (x < 0) {
		x = 0;
	}

	if (y < 0) {
		y = 0;
	}

	//printf("SETTING VIEWPORT: %f %f %f %f\n", x, y, width, height);

	// we want scissor here, as viewport will restrict draw coords
	setScissor(renderer, x, y, width, height);
}

void D3DPOLY_DrawOTag(int *tag) {
	printf("STUB: D3DPOLY_DrawOTag: 0x%08x\n", tag);

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
			//printf("just think, we could be drawing now... %d, %d\n", cmd >> 2, cmd & 0xfffffffc);
			if (!(cmd & 0x80)) {
				switch(cmd >> 2) {
					case 8: 
						printf("renderPolyF3\n");
						break;
					case 9: 
						printf("renderPolyFT3\n");
						break;
					case 10: 
						printf("renderPolyF4\n");
						break;
					case 11: 
						renderPolyFT4(tag);
						break;
					case 12: 
						printf("renderPolyG3\n");
						break;
					case 13: 
						printf("renderPolyGT3\n");
						break;
					case 14: 
						renderPolyG4(tag);
						break;
					case 15: 
						printf("renderPolyGT4\n");
						break;
					case 16: 
						printf("renderLineF2\n");
						break;
					case 18: 
						printf("renderLineF3\n");
						break;
					case 20: 
						printf("renderLineF4\n");
						break;
					case 21: 
						printf("renderLineG2\n");
						break;
					case 24: 
						printf("renderTile\n");
						break;
					case 26: 
						printf("renderTile1\n");
						break;
					case 28: 
						printf("renderTile8\n");
						break;
					case 30: 
						printf("renderTile16\n");
						break;
					default:
						printf("UNKNOWN RENDER COMMAND: %d\n", cmd >> 2);
				}
			} else if (cmd == 0xB0) {
				renderDXPoly(tag);
			} else if (cmd == 0xE1) {
				// maybe blend mode
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
}

int allocbytes = 0;

void *(__cdecl *mem_newx)(int, void *, void *, void *) = 0x0046e950;
void *__cdecl mem_new_wrapper(int size, void *b, void *c, int d) {
	if (size < 5) {
		size = 8;
	}
	if (d != 0) {
		d += 0x10;
	}

	printf("MEM_NEW WITH SIZE %d\n", size);

	int result = mem_newx(size, b, c, d);

	//printf("MEM_NEW WITH SIZE %d - 0x%08x\n", size, result);

	if (result) {
		result -= 4;
	}

	map_put(memmap, &result, sizeof(int), &size, sizeof(int));

	allocbytes += size;

	printf("MEM_NEW WITH SIZE %d - 0x%08x, TOTAL: %d\n", size, result, allocbytes);

	return result;
}

void (__cdecl *mem_deletex)(void *) = 0x0046ef90;
void __cdecl mem_delete_wrapper(int p) {
	mem_deletex(p + 4);

	int *sz = map_get(memmap, &p, sizeof(int));

	if (sz) {
		allocbytes -= *sz;

		map_del(memmap, &p, sizeof(int));
	} else {
		printf("0x%08x WASN'T FOUND\n", p);
		while(1) {
		
		}
	}
	

	printf("MEM_DELETE - 0x%08x, TOTAL: %d\n", p, allocbytes);
}

void *__cdecl mem_new_simple(int size, void *b, void *c, int d) {
	return malloc(size);
}

void __cdecl mem_delete_simple(void *p) {
	free(p);
}

void WINMAIN_Configure() {
	printf("STUB: WINMAIN_Configure\n");
}

void installGfxPatches() {
	patchJmp(0x0046f420, mem_new_wrapper);
	patchJmp(0x0046f460, mem_delete_wrapper);

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