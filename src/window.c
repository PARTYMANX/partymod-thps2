#include <windows.h>

#include <patch.h>
#include <event.h>

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
	printf("Creating window\n");

	SDL_Init(SDL_INIT_VIDEO);

	printf("test sdl???\n");

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

	SDL_DisplayMode displayMode;
	SDL_GetDesktopDisplayMode(0, &displayMode);

	window = SDL_CreateWindow("THPS2 - PARTYMOD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);   // TODO: fullscreen
	//window = SDL_CreateWindow("THPS2 - PARTYMOD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);   // TODO: fullscreen

	if (!window) {
		printf("Failed to create window! Error: %s\n", SDL_GetError());
	}

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND windowHandle = wmInfo.info.win.window;

	printf("Created window\n");

	registerEventHandler(handleWindowEvents);

	return windowHandle;
}

void getWindowSize(int *w, int *h) {
	SDL_GetWindowSize(window, w, h);
}

void installWindowPatches() {
	patchCall(0x004f4ff1 + 5, initWindow);
}