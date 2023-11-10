struct D3D7Device {
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
    /*** IUnknown methods ***/
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