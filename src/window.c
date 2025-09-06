#include <windows.h>

#include <patch.h>
#include <event.h>
#include <config.h>
#include <log.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

void handleWindowEvents(SDL_Event *e) {
	switch(e->type) {
		case SDL_WINDOWEVENT_CLOSE:
		case SDL_QUIT:
			quitGame();
			break;
	}
}

void enforceMaxResolution(int *resX, int *resY) {
	uint8_t isValidX = 0;
	uint8_t isValidY = 0;

	int numDisplayModes = SDL_GetNumDisplayModes(0);

	for (int i = 0; i < numDisplayModes; i++) {
		SDL_DisplayMode displayMode;
		SDL_GetDisplayMode(0, i, &displayMode);

		log_printf(LL_TRACE, "found display mode: %dx%d\n", displayMode.w, displayMode.h);

		if (displayMode.w >= *resX) {
			isValidX = 1;
		}
		if (displayMode.h >= *resY) {
			isValidY = 1;
		}
	}

	if (!isValidX || !isValidY) {
		*resX = 0;
		*resY = 0;
	}
}

SDL_Window *window;
int windowResX;
int windowResY;
int isWindowed;
int borderless;

#define WINDOW_SECTION "Window"

void createErrorMessageBox(const char *msg) {
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", msg, window);
}

void configWindow() {
	windowResX = getConfigInt(WINDOW_SECTION, "ResolutionX", 640);
	windowResY = getConfigInt(WINDOW_SECTION, "ResolutionY", 480);
	isWindowed = getConfigBool(WINDOW_SECTION, "Windowed", 1);
	borderless = getConfigBool(WINDOW_SECTION, "Borderless", 0);
}

HWND initWindow() {
	log_printf(LL_INFO, "Creating window\n");

	

	configWindow();

	SDL_Init(SDL_INIT_VIDEO);

	SDL_WindowFlags flags = isWindowed ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN;

	if (borderless && isWindowed) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	enforceMaxResolution(&windowResX, &windowResY);

	if (windowResX == 0 || windowResY == 0) {
		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(0, &displayMode);
		windowResX = displayMode.w;
		windowResY = displayMode.h;
	}
		
	if (windowResX < 640) {
		windowResX = 640;
	}
	if (windowResY < 480) {
		windowResY = 480;
	}

	SDL_DisplayMode displayMode;
	SDL_GetDesktopDisplayMode(0, &displayMode);

	flags |= SDL_WINDOW_ALLOW_HIGHDPI;

	window = SDL_CreateWindow("THPS2 - PARTYMOD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowResX, windowResY, SDL_WINDOW_SHOWN | flags);

	if (!window) {
		log_printf(LL_ERROR, "Failed to create window! Error: %s\n", SDL_GetError());
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND windowHandle = wmInfo.info.win.window;

	log_printf(LL_INFO, "Created window\n");

	registerEventHandler(handleWindowEvents);

	return windowHandle;
}

void getWindowSize(int *w, int *h) {
	SDL_GetWindowSize(window, w, h);
}

void installWindowPatches() {
	patchCall(0x004f4ff1 + 5, initWindow);
}