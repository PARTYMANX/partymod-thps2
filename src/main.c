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
#include <gfx/gfx.h>
#include <mem.h>
#include <event.h>

#define VERSION_NUMBER_MAJOR 0
#define VERSION_NUMBER_MINOR 1
#define VERSION_NUMBER_PATCH 0

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

	initEvents();

	printf("Patch Initialized\n");
}

void quitGame() {
	void (*gameShutdown)() = 0x004e46c0;
	void (*WINMAIN_ShutDown)() = 0x004f5750;
	void (*operator_delete)(void *) = 0x004fd323;
	void **PCMemBuffer = 0x0055ec78;

	gameShutdown();
	WINMAIN_ShutDown();
	if (*PCMemBuffer) {
		operator_delete(PCMemBuffer);
		*PCMemBuffer = NULL;
	}

	exit(1);
}

int WinYield() {
	int result = 0x75;

	handleEvents();

	return result;
}

void drawSync() {
	//SDL_Delay(16);
};

void patchWindowAndInit() {
	patchNop(0x004f4ff1, 47);
	patchCall(0x004f4ff1, initPatch);
	
	patchJmp(0x004f4d70, WinYield);

	patchNop(0x004f502b, 39);	// patch out window setup stuff that we no longer need

	//patchJmp(0x004e5910, drawSync);

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
			installWindowPatches();
			installInputPatches();
			installGfxPatches();
			installMemPatches();

			//installAltMemManager();

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
