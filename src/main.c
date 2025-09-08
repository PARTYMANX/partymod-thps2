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
#include <options.h>
#include <mem.h>
#include <event.h>
#include <window.h>
#include <log.h>
#include <thps1/thps1.h>

// load file patch
void patchSaveOpen() {
	patchByte(0x004e6249 + 1, 0);	// change file open for loading saves/replays to read instead of read and write
}

uint8_t flag_thps1career = 0;

void processArgs() {
	//printf("PROCESSING ARGS\n");
	WCHAR *cmd = GetCommandLineW();
	int argc = 0;
	char** argv = CommandLineToArgvW(cmd, &argc);

	//printf("%d ARGS IN %ls\n", argc, cmd);

	for (int i = 0; i < argc; i++) {
		//printf("ARG %d: %ls\n", i, argv[i]);
		if (wcscmp(argv[i], L"-thps1career") == 0) {
			flag_thps1career = 1;
		}
	}
}

void initPatch() {
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	GetModuleFileName(NULL, &executableDirectory, filePathBufLen);

	// find last slash
	char *exe = strrchr(executableDirectory, '\\');
	if (exe) {
		*(exe + 1) = '\0';
	}

	processArgs();

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

	log_printf(LL_INFO, "Patch Initialized\n");

	//dumpAudioBanks();

	if (getConfigBool("Miscellaneous", "THPS1Career", 0) || flag_thps1career) {
		log_printf(LL_INFO, "THPS1 Career Enabled!\n");
		patchTHPS1Career();
	} else {
		patchTHPS1LevelFixes();
	}
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

	handleEvents();

	return result;
}

void patchDebugLog() {
	patchJmp(0x004cca60, log_debug_printf);
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
			installOptionsPatches();
			patchSaveOpen();

			patchDebugLog();

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
