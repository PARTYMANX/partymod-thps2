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
#include <gfx/gfx.h>
#include <sfx.h>
#include <mem.h>
#include <event.h>
#include <window.h>
#include <log.h>

// disable the options menu entries for control and display options as they're no longer relevant
void __fastcall OptionsMenuConstructorWrapper(uint8_t **optionsMenu) {
	//void (__fastcall *OptionsMenuConstructor)(uint8_t **) = 0x0048185d;
	void (__fastcall *OptionsMenuConstructor)(uint8_t **) = 0x0047eb00;

	OptionsMenuConstructor(optionsMenu);

	//optionsMenu[0xd9][0xc] = 0;
	//optionsMenu[0xda][0xc] = 0;
}

void patchOptionsMenu() {

	patchCall(0x0048185d, OptionsMenuConstructorWrapper);
	// get rid of player controls menu
	patchNop(0x0047f1f0, 5);
	patchByte(0x0047f203, 0xeb);

	// get rid of display controls menu
	patchNop(0x0047f22d, 5);
	patchByte(0x0047f240, 0xeb);
}

// load file patch
void patchSaveOpen() {
	patchByte(0x004e6249 + 1, 0);	// change file open for loading saves/replays to read instead of read and write
}

// bad autokick patch - not very graceful but it works until we can integrate it back into the menus
uint32_t autokickSetting = 1;

void handleAutokickOverride() {
	uint32_t *autokickstate = 0x00567038;
	uint32_t *autokickstate2 = 0x0055c88c;

	*autokickstate = autokickSetting;
	*autokickstate2 = autokickSetting;
}

void loadAutokickSetting() {
	autokickSetting = getConfigBool("Miscellaneous", "Autokick", 1);
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

	initConfig();
	
	int isDebug = getConfigBool("Miscellaneous", "Debug", 0);

	configureLogging(isDebug);

	if (isDebug) {
		AllocConsole();

		FILE *fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}
	log_printf(LL_INFO, "PARTYMOD for THPS2 %d.%d.%d\n", VERSION_NUMBER_MAJOR, VERSION_NUMBER_MINOR, VERSION_NUMBER_PATCH);

	log_printf(LL_INFO, "DIRECTORY: %s\n", executableDirectory);

	log_printf(LL_INFO, "LOG LEVEL: %s\n", get_log_level());

#ifdef MEM_AUDIT
	initMemAudit();
#endif

	initEvents();

	loadAutokickSetting();

	log_printf(LL_INFO, "Patch Initialized\n");
}

void fatalError(const char *msg) {
	log_printf(LL_ERROR, "FATAL ERROR: %s\n", msg);
	createErrorMessageBox(msg);
	exit(1);
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

	exit(0);
}

int WinYield() {
	int result = 0x75;

	// this gets called every frame, so hack in autokick override here (dumb but it works)
	handleAutokickOverride();

	handleEvents();

	return result;
}

void patchWindowAndInit() {
	patchNop(0x004f4ff1, 47);
	patchCall(0x004f4ff1, initPatch);
	
	patchJmp(0x004f4d70, WinYield);

	patchNop(0x004f502b, 39);	// patch out window setup stuff that we no longer need
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
			installSfxPatches();
			patchSaveOpen();
			patchOptionsMenu();

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
