#include <windows.h>

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include <patch.h>
#include <global.h>
#include <input.h>
#include <config.h>
#include <script.h>
#include <gfx.h>
#include <mem.h>

#define VERSION_NUMBER_MAJOR 0
#define VERSION_NUMBER_MINOR 1
#define VERSION_NUMBER_PATCH 0

/*void enforceMaxResolution() {
	DEVMODE deviceMode;
	int i = 0;
	uint8_t isValidX = 0;
	uint8_t isValidY = 0;

	while (EnumDisplaySettings(NULL, i, &deviceMode)) {
		if (deviceMode.dmPelsWidth >= *resX) {
			isValidX = 1;
		}
		if (deviceMode.dmPelsHeight >= *resY) {
			isValidY = 1;
		}

		i++;
	}

	if (!isValidX || !isValidY) {
		*resX = 0;
		*resY = 0;
	}
}*/

SDL_Window *window;

HWND initWindow() {
	SDL_Init(SDL_INIT_VIDEO);

	//SDL_WindowFlags flags = *isWindowed ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN;

	/*if (borderless && *isWindowed) {
		flags |= SDL_WINDOW_BORDERLESS;
	}*/

	//enforceMaxResolution();

	/*if (*resX == 0 || *resY == 0) {
		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(0, &displayMode);
		*resX = displayMode.w;
		*resY = displayMode.h;
	}*/
		
	/*if (*resX < 640) {
		*resX = 640;
	}
	if (*resY < 480) {
		*resY = 480;
	}*/

	window = SDL_CreateWindow("THPS2 - PARTYMOD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1440, SDL_WINDOW_SHOWN);   // TODO: fullscreen

	if (!window) {
		printf("Failed to create window! Error: %s\n", SDL_GetError());
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND windowHandle = wmInfo.info.win.window;

	return windowHandle;
}

void initPatch() {
	GetModuleFileName(NULL, &executableDirectory, filePathBufLen);

	// find last slash
	char *exe = strrchr(executableDirectory, '\\');
	if (exe) {
		*(exe + 1) = '\0';
	}

	char configFile[1024];
	sprintf(configFile, "%s%s", executableDirectory, CONFIG_FILE_NAME);

	//int isDebug = getIniBool("Miscellaneous", "Debug", 0, configFile);
	int isDebug = 1;

	if (isDebug) {
		AllocConsole();

		FILE *fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}
	printf("PARTYMOD for THPS2 %d.%d.%d\n", VERSION_NUMBER_MAJOR, VERSION_NUMBER_MINOR, VERSION_NUMBER_PATCH);

	printf("DIRECTORY: %s\n", executableDirectory);

	#ifdef MEM_AUDIT
	initMemAudit();
	#endif

	printf("Patch Initialized\n");
}

int eventHandler() {
	int result = 0x75;

	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_KEYDOWN:
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_QUIT:
				result = 0;
				break;
		}

		//processEvent(&e);
	}

	return result;
}

void patchWindowAndInit() {
	patchNop(0x004f4ff1, 47);
	patchCall(0x004f4ff1, initPatch);
	patchCall(0x004f4ff1 + 5, initWindow);

	patchJmp(0x004f4d70, eventHandler);

	patchNop(0x004f502b, 39);	// patch out window setup stuff that we no longer need

	//patchByte(0x004f544d + 1, 0x8); // windowed mode
	//patchByte(0x004f552e + 7, 0x1); // windowed mode
	//patchByte(0x004f5539 + 7, 0); // windowed mode
	//patchByte(0x004f5539 + 8, 0x2); // windowed mode
}

__declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	// Perform actions based on the reason for calling.
	switch(fdwReason) { 
		case DLL_PROCESS_ATTACH:
			// Initialize once for each new process.
			// Return FALSE to fail DLL load.

			// install patches
			patchWindowAndInit();
			installGfxPatches();

			#ifdef MEM_AUDIT
			installMemAudit();
			#endif

			break;

		case DLL_THREAD_ATTACH:
			// Do thread-specific initialization.
			break;

		case DLL_THREAD_DETACH:
			// Do thread-specific cleanup.
			break;

		case DLL_PROCESS_DETACH:
			// Perform any necessary cleanup.
			break;
	}
	return TRUE;
}
