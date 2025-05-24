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
#include <mem.h>
#include <event.h>
#include <window.h>
#include <log.h>

struct {
	void *create_window;	// 0x004f4ff1

	void *winyield;			// 0x004f4d70
	void *window_setup;		// 0x004f502b

	// having trouble finding these...
	void *options_menu_constructor; // 0x0047eb00
	void *options_menu_constructor_call; // 0x0048185d

	void *player_controls; // 0x0047f1f0
	// we'll leave the menu stuff for later

	void *save_open;	// 0x004e6249

	void *autokick_1;	// 0x00567038
	void *autokick_2;	// 0x0055c88c

	void *gameShutdown;	// 0x004e46c0;
	void *WINMAIN_ShutDown;	// 0x004f5750;
	void *operator_delete;	// 0x004fd323;
	void *PCMemBuffer;	// 0x0055ec78;
} mainOffsets;

void findMainOffsets() {
	uint8_t result = 1;
	result &= findPattern("83 ec 1c a1 ?? ?? ?? ?? 8d 4c 24 00", base_addr, mod_size, (uint32_t *)&(mainOffsets.winyield));
	result &= findPattern("6a 05 56 ff 15 ?? ?? ?? ?? 56 ff 15", base_addr, mod_size, (uint32_t *)&(mainOffsets.window_setup));

	//result &= findPattern("8b c8 e8 ?? ?? ?? ?? 8b f0 eb 02 33 f6 8b 0d ?? ?? ?? ?? 68 ?? ?? ?? ?? c7 44 24 1c ff ff ff ff")

	result &= findPattern("6a 02 50 51 be 01 00 00 00 e8", base_addr, mod_size, (uint32_t *)&(mainOffsets.save_open));

	if (!result) {
		fatalError("Failed to find main offsets! This version of the game may be unsupported.");
	}
}

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
	patchByte(((uint32_t)(mainOffsets.save_open)) + 1, 0);	// change file open for loading saves/replays to read instead of read and write
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

int WinYield() {
	int result = 0x75;

	// this gets called every frame, so hack in autokick override here (dumb but it works)
	handleAutokickOverride();

	handleEvents();

	return result;
}

void installPatches();

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

	findMainOffsets();
	installPatches();

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

void patchInit() {
	patchNop(mainOffsets.create_window, 47);
	patchCall(mainOffsets.create_window, initPatch);
}

void patchEventHandlerAndWindow() {
	patchJmp(mainOffsets.winyield, WinYield);

	patchNop(mainOffsets.window_setup, 39);	// patch out window setup stuff that we no longer need
}

void installPatches() {
	patchEventHandlerAndWindow();
	installWindowPatches(mainOffsets.create_window);
	installInputPatches();
	installGfxPatches();
	installMemPatches();
	patchSaveOpen();
	patchOptionsMenu();

	//installAltMemManager();
}

uint8_t findCreateWindowOffset() {
	return findPattern("50 53 50 50 6a 01 8b 35", base_addr, mod_size, (uint32_t *)&(mainOffsets.create_window));
}

void getModuleInfo() {
	void *mod = GetModuleHandle(NULL);

	base_addr = mod;
	void *end_addr = NULL;

	PIMAGE_DOS_HEADER dos_header = mod;
	PIMAGE_NT_HEADERS nt_headers = (uint8_t *)mod + dos_header->e_lfanew;

	for (int i = 0; i < nt_headers->FileHeader.NumberOfSections; i++) {
		PIMAGE_SECTION_HEADER section = (uint8_t *)nt_headers->OptionalHeader.DataDirectory + nt_headers->OptionalHeader.NumberOfRvaAndSizes * sizeof(IMAGE_DATA_DIRECTORY) + i * sizeof(IMAGE_SECTION_HEADER);

		uint32_t section_size;
		if (section->SizeOfRawData != 0) {
			section_size = section->SizeOfRawData;
		}
		else {
			section_size = section->Misc.VirtualSize;
		}

		if (section->Characteristics & IMAGE_SCN_MEM_EXECUTE) {
			end_addr = (uint8_t *)base_addr + section->VirtualAddress + section_size;
		}

		if ((i == nt_headers->FileHeader.NumberOfSections - 1) && end_addr == NULL) {
			end_addr = (uint32_t)base_addr + section->PointerToRawData + section_size;
		}
	}

	mod_size = (uint32_t)end_addr - (uint32_t)base_addr;
}

__declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	// Perform actions based on the reason for calling.
	switch(fdwReason) { 
		case DLL_PROCESS_ATTACH:
			// Initialize once for each new process.
			// Return FALSE to fail DLL load.

			getModuleInfo();
			if (!findCreateWindowOffset()) {
				return FALSE;
			}

			// install patches
			patchInit();

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
