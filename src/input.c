#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <global.h>
#include <patch.h>
#include <event.h>
#include <gfx/gfx.h>

//

struct keybinds {
	SDL_Scancode menu;
	SDL_Scancode cameraToggle;
	SDL_Scancode cameraSwivelLock;

	SDL_Scancode grind;
	SDL_Scancode grab;
	SDL_Scancode ollie;
	SDL_Scancode kick;

	SDL_Scancode leftSpin;
	SDL_Scancode rightSpin;
	SDL_Scancode nollie;
	SDL_Scancode switchRevert;

	SDL_Scancode right;
	SDL_Scancode left;
	SDL_Scancode up;
	SDL_Scancode down;

	SDL_Scancode cameraRight;
	SDL_Scancode cameraLeft;
	SDL_Scancode cameraUp;
	SDL_Scancode cameraDown;
};

// a recreation of the SDL_GameControllerButton enum, but with the addition of right/left trigger
typedef enum {
	CONTROLLER_UNBOUND = -1,
	CONTROLLER_BUTTON_A = SDL_CONTROLLER_BUTTON_A,
	CONTROLLER_BUTTON_B = SDL_CONTROLLER_BUTTON_B,
	CONTROLLER_BUTTON_X = SDL_CONTROLLER_BUTTON_X,
	CONTROLLER_BUTTON_Y = SDL_CONTROLLER_BUTTON_Y,
	CONTROLLER_BUTTON_BACK = SDL_CONTROLLER_BUTTON_BACK,
	CONTROLLER_BUTTON_GUIDE = SDL_CONTROLLER_BUTTON_GUIDE,
	CONTROLLER_BUTTON_START = SDL_CONTROLLER_BUTTON_START,
	CONTROLLER_BUTTON_LEFTSTICK = SDL_CONTROLLER_BUTTON_LEFTSTICK,
	CONTROLLER_BUTTON_RIGHTSTICK = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
	CONTROLLER_BUTTON_LEFTSHOULDER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	CONTROLLER_BUTTON_RIGHTSHOULDER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
	CONTROLLER_BUTTON_DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
	CONTROLLER_BUTTON_DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	CONTROLLER_BUTTON_DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	CONTROLLER_BUTTON_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	CONTROLLER_BUTTON_MISC1 = SDL_CONTROLLER_BUTTON_MISC1,
	CONTROLLER_BUTTON_PADDLE1 = SDL_CONTROLLER_BUTTON_PADDLE1,
	CONTROLLER_BUTTON_PADDLE2 = SDL_CONTROLLER_BUTTON_PADDLE2,
	CONTROLLER_BUTTON_PADDLE3 = SDL_CONTROLLER_BUTTON_PADDLE3,
	CONTROLLER_BUTTON_PADDLE4 = SDL_CONTROLLER_BUTTON_PADDLE4,
	CONTROLLER_BUTTON_TOUCHPAD = SDL_CONTROLLER_BUTTON_TOUCHPAD,
	CONTROLLER_BUTTON_RIGHTTRIGGER = 21,
	CONTROLLER_BUTTON_LEFTTRIGGER = 22,
} controllerButton;

typedef enum {
	CONTROLLER_STICK_UNBOUND = -1,
	CONTROLLER_STICK_LEFT = 0,
	CONTROLLER_STICK_RIGHT = 1
} controllerStick;

struct controllerbinds {
	controllerButton menu;
	controllerButton cameraToggle;
	controllerButton cameraSwivelLock;

	controllerButton grind;
	controllerButton grab;
	controllerButton ollie;
	controllerButton kick;

	controllerButton leftSpin;
	controllerButton rightSpin;
	controllerButton nollie;
	controllerButton switchRevert;

	controllerButton right;
	controllerButton left;
	controllerButton up;
	controllerButton down;

	controllerStick movement;
	controllerStick camera;
};

//

int controllerCount;
int controllerListSize;
SDL_GameController **controllerList;    // does this need to be threadsafe?
struct keybinds keybinds;
struct controllerbinds padbinds;

uint8_t isCursorActive = 1;
uint8_t isUsingKeyboard = 1;
uint8_t isUsingHardCodeControls = 1;
uint8_t anyButtonPressed = 0;

void setUsingKeyboard(uint8_t usingKeyboard) {
	isUsingKeyboard = usingKeyboard;

	if (isUsingKeyboard && isCursorActive) {
		SDL_ShowCursor(SDL_ENABLE);
	} else {
		SDL_ShowCursor(SDL_DISABLE);
	}
}

void setCursorActive() {
	isCursorActive = 1;

	if (isUsingKeyboard && isCursorActive) {
		SDL_ShowCursor(SDL_ENABLE);
	} else {
		SDL_ShowCursor(SDL_DISABLE);
	}
}

void setCursorInactive() {
	isCursorActive = 0;

	if (isUsingKeyboard && isCursorActive) {
		SDL_ShowCursor(SDL_ENABLE);
	} else {
		SDL_ShowCursor(SDL_DISABLE);
	}
}

void addController(int idx) {
	printf("Detected controller \"%s\"\n", SDL_GameControllerNameForIndex(idx));

	SDL_GameController *controller = SDL_GameControllerOpen(idx);

	if (controller) {
		if (controllerCount == controllerListSize) {
			int tmpSize = controllerListSize + 1;
			SDL_GameController **tmp = realloc(controllerList, sizeof(SDL_GameController *) * tmpSize);
			if (!tmp) {
				return; // TODO: log something here or otherwise do something
			}

			controllerListSize = tmpSize;
			controllerList = tmp;
		}

		controllerList[controllerCount] = controller;
		controllerCount++;
	}
}

void removeController(SDL_GameController *controller) {
	printf("Controller \"%s\" disconnected\n", SDL_GameControllerName(controller));

	int i = 0;

	while (i < controllerCount && controllerList[i] != controller) {
		i++;
	}

	if (controllerList[i] == controller) {
		SDL_GameControllerClose(controller);

		for (; i < controllerCount - 1; i++) {
			controllerList[i] = controllerList[i + 1];
		}
		controllerCount--;
	} else {
		printf("Did not find disconnected controller in list\n");
	}
}

void initSDLControllers() {
	printf("Initializing Controller Input\n");

	controllerCount = 0;
	controllerListSize = 1;
	controllerList = malloc(sizeof(SDL_GameController *) * controllerListSize);

	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			addController(i);
		}
	}

	// add event filter for newly connected controllers
	//SDL_SetEventFilter(controllerEventFilter, NULL);
}

uint8_t axisAbs(uint8_t val) {
	if (val > 127) {
		// positive, just remove top bit
		return val & 0x7F;
	} else {
		// negative
		return ~val & 0x7f;
	}
}

uint8_t getButton(SDL_GameController *controller, controllerButton button) {
	if (button == CONTROLLER_BUTTON_LEFTTRIGGER) {
		uint8_t pressure = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) >> 7;
		return pressure > 0x80;
	} else if (button == CONTROLLER_BUTTON_RIGHTTRIGGER) {
		uint8_t pressure = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) >> 7;
		return pressure > 0x80;
	} else {
		return SDL_GameControllerGetButton(controller, button);
	}
}

void getStick(SDL_GameController *controller, controllerStick stick, uint8_t *xOut, uint8_t *yOut) {
	if (stick == CONTROLLER_STICK_LEFT) {
		*xOut = (uint8_t)((SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) >> 8) + 128);
		*yOut = (uint8_t)((SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) >> 8) + 128);
	} else if (stick == CONTROLLER_STICK_RIGHT) {
		*xOut = (uint8_t)((SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) >> 8) + 128);
		*yOut = (uint8_t)((SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) >> 8) + 128);
	} else {
		*xOut = 0x80;
		*yOut = 0x80;
	}
}

uint16_t pollController(SDL_GameController *controller) {
	uint16_t result = 0;

	if (SDL_GameControllerGetAttached(controller)) {
		//printf("Polling controller \"%s\"\n", SDL_GameControllerName(controller));

		// buttons
		if (getButton(controller, padbinds.menu)) {
			result |= 0x01 << 11;
		}
		if (getButton(controller, padbinds.cameraToggle)) {
			result |= 0x01 << 8;
		}
		if (getButton(controller, padbinds.cameraSwivelLock)) {
			result |= 0x01 << 10;
		}

		if (getButton(controller, padbinds.grind)) {
			result |= 0x01 << 4;
		}
		if (getButton(controller, padbinds.grab)) {
			result |= 0x01 << 5;
		}
		if (getButton(controller, padbinds.ollie)) {
			result |= 0x01 << 6;
		}
		if (getButton(controller, padbinds.kick)) {
			result |= 0x01 << 7;
		}

		// shoulders
		if (getButton(controller, padbinds.leftSpin)) {
			result |= 0x01 << 2;
		}

		if (getButton(controller, padbinds.rightSpin)) {
			result |= 0x01 << 3;
		}

		if (getButton(controller, padbinds.nollie)) {
			result |= 0x01 << 0;
		}

		if (getButton(controller, padbinds.switchRevert)) {
			result |= 0x01 << 1;
		}
		
		// d-pad
		if (SDL_GameControllerGetButton(controller, padbinds.up)) {
			result |= 0x01 << 12;
		}
		if (SDL_GameControllerGetButton(controller, padbinds.right)) {
			result |= 0x01 << 13;
		}
		if (SDL_GameControllerGetButton(controller, padbinds.down)) {
			result |= 0x01 << 14;
		}
		if (SDL_GameControllerGetButton(controller, padbinds.left)) {
			result |= 0x01 << 15;
		}
	}

	return result;
}

uint16_t pollKeyboard() {
	uint8_t *keyboardState = SDL_GetKeyboardState(NULL);

	uint16_t result = 0;

	// buttons
	if (keyboardState[keybinds.menu] && keybinds.menu != SDL_SCANCODE_ESCAPE) {	// is esc is bound to menu, it will only interfere with hardcoded keybinds.  similar effect on enter but i can't detect the things needed to work there
		result |= 0x01 << 11;
	}
	if (keyboardState[keybinds.cameraToggle]) {
		result |= 0x01 << 8;
	}
	if (keyboardState[keybinds.cameraSwivelLock]) {
		result |= 0x01 << 10;
	}

	if (keyboardState[keybinds.grind]) {
		result |= 0x01 << 4;
	}
	if (keyboardState[keybinds.grab]) {
		result |= 0x01 << 5;
	}
	if (keyboardState[keybinds.ollie]) {
		result |= 0x01 << 6;
	}
	if (keyboardState[keybinds.kick]) {
		result |= 0x01 << 7;
	}

	// shoulders
	if (keyboardState[keybinds.leftSpin]) {
		result |= 0x01 << 2;
	}
	if (keyboardState[keybinds.rightSpin]) {
		result |= 0x01 << 3;
	}
	if (keyboardState[keybinds.nollie]) {
		result |= 0x01 << 0;
	}
	if (keyboardState[keybinds.switchRevert]) {
		result |= 0x01 << 1;
	}
		
	// d-pad
	if (keyboardState[keybinds.up] || (isUsingHardCodeControls && keyboardState[SDL_SCANCODE_UP])) {
		result |= 0x01 << 12;
	}
	if (keyboardState[keybinds.right] || (isUsingHardCodeControls && keyboardState[SDL_SCANCODE_RIGHT])) {
		result |= 0x01 << 13;
	}
	if (keyboardState[keybinds.down] || (isUsingHardCodeControls && keyboardState[SDL_SCANCODE_DOWN])) {
		result |= 0x01 << 14;
	}
	if (keyboardState[keybinds.left] || (isUsingHardCodeControls && keyboardState[SDL_SCANCODE_LEFT])) {
		result |= 0x01 << 15;
	}

	return result;
}

// returns 1 if a text entry prompt is on-screen so that keybinds don't interfere with text entry confirmation/cancellation
int isKeyboardTyping() {
	void *(__stdcall *getMenuFactorySingleton)(int) = (void *)0x004d12a0;
	void (*deleteMenuFactorySingleton)() = (void *)0x004d12f0;
	
	void *menuFactory = getMenuFactorySingleton(0);

	int result = *(int *)(((uint8_t *)menuFactory) + 0x154);
	
	deleteMenuFactorySingleton();

	return result;
}

void __cdecl processController() {
	int *gShellMode = 0x006a35b4;
	/*SDL_Event e;
	while(SDL_PollEvent(&e)) {
		processEvent(&e);
	}*/

	uint16_t controlData = 0;

	/*if (!isKeyboardTyping()) {
		pollKeyboard(dev);
	}*/

	// TODO: maybe smart selection of active controller?
	for (int i = 0; i < controllerCount; i++) {
		controlData |= pollController(controllerList[i]);
	}

	uint32_t *gForcedButtons = 0x006a35bc;
	uint32_t *gLockForcedButtons = 0x006a35c0;

	if (*gLockForcedButtons > 0) {
		*gLockForcedButtons = *gLockForcedButtons - 1;
		printf("Forced Buttons Lock: %d\n", *gLockForcedButtons);
	}

	if (*gForcedButtons) {
		controlData |= *gForcedButtons;
		printf("Forced Buttons: 0x%08x\n", *gForcedButtons);
		*gForcedButtons = 0;
	}

	uint16_t *pControlData = 0x006a0b6c;

	pControlData[0] = controlData;

	if (*gShellMode == 0) {
		
	} else {
		//int32_t *unk = 0x001981c8;
		//printf("unknown: 0x%08x\n", *unk);
	}

	//int (__stdcall *isVibrationOn)() = (void *)0x0041f910;
	//int vibe = isVibrationOn();
	//printf("IS VIBRATION ON: %d\n", vibe);	// enable vibration: 0041f970

	//printf("VIBRATION BUFFERS:\n");
	//printf("%08x%08x%08x%08x%08x%08x%08x%08x\n", dev->vibrationData_align[0], dev->vibrationData_align[1], dev->vibrationData_align[2], dev->vibrationData_align[3], dev->vibrationData_align[4], dev->vibrationData_align[5], dev->vibrationData_align[6], dev->vibrationData_align[7]);
	//printf("%08x%08x%08x%08x%08x%08x%08x%08x\n", dev->vibrationData_direct[0], dev->vibrationData_direct[1], dev->vibrationData_direct[2], dev->vibrationData_direct[3], dev->vibrationData_direct[4], dev->vibrationData_direct[5], dev->vibrationData_direct[6], dev->vibrationData_direct[7]);
	//printf("%08x%08x%08x%08x%08x%08x%08x%08x\n", dev->vibrationData_max[0], dev->vibrationData_max[1], dev->vibrationData_max[2], dev->vibrationData_max[3], dev->vibrationData_max[4], dev->vibrationData_max[5], dev->vibrationData_max[6], dev->vibrationData_max[7]);
	//printf("\n");
}

void processMouse() {
	uint32_t mouseX, mouseY, resX, resY;
	uint32_t mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
	mouseButtons &= 0x01;

	toGameScreenCoord(mouseX, mouseY, &mouseX, &mouseY);

	mouseX *= 512;
	mouseY *= 240;
	
	getGameResolution(&resX, &resY);
	mouseX /= resX;
	mouseY /= resY;

	// TODO: translate mouse position to game screen space

	uint32_t *gameMouseX = 0x0069e040;
	uint32_t *gameMouseY = 0x0069e044;
	uint32_t *gameMouseButtons = 0x0069e048;

	uint8_t *mouseClicked = 0x0069e04c;
	uint8_t *mouseChanged = 0x0069e04d;
	uint8_t *mouseUnk1 = 0x0069e04f;
	uint32_t *mouseUnk2 = 0x0069e054;

	*mouseClicked = 0;
	if (*gameMouseX == mouseX && *gameMouseY == mouseY && *gameMouseButtons == mouseButtons) {
		*mouseChanged = 0;
	} else {
		*mouseChanged = 1;
		*mouseUnk1 = 0;
		*mouseUnk2 = 0;
		if (*gameMouseButtons != mouseButtons && mouseButtons == 0) {
			*mouseClicked = 1;
		}
	}

	*gameMouseX = mouseX;
	*gameMouseY = mouseY;
	*gameMouseButtons = mouseButtons;
}

void processInputEvent(SDL_Event *e) {
	switch (e->type) {
		case SDL_CONTROLLERDEVICEADDED:
			if (SDL_IsGameController(e->cdevice.which)) {
				addController(e->cdevice.which);
			} else {
				printf("Not a game controller: %s\n", SDL_JoystickNameForIndex(e->cdevice.which));
			}
			return;
		case SDL_CONTROLLERDEVICEREMOVED: {
			SDL_GameController *controller = SDL_GameControllerFromInstanceID(e->cdevice.which);
			if (controller) {
				removeController(controller);
			}
			return;
		}
		case SDL_JOYDEVICEADDED:
			printf("Joystick added: %s\n", SDL_JoystickNameForIndex(e->jdevice.which));
			setUsingKeyboard(0);
			return;
		case SDL_CONTROLLERBUTTONDOWN:
			anyButtonPressed = 1;
		case SDL_CONTROLLERAXISMOTION:
			setUsingKeyboard(0);
			return;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION: {
			setUsingKeyboard(1);
			return;
		}
		case SDL_KEYDOWN: 
			if (!e->key.repeat) {
				//printf("KEY DOWN!!\n");
				anyButtonPressed = 1;
			}
			setUsingKeyboard(1);
			return;
		default:
			return 0;
	}
}

void configureController() {
	padbinds.menu = CONTROLLER_BUTTON_START;
	padbinds.cameraToggle = CONTROLLER_BUTTON_BACK;

	padbinds.ollie = CONTROLLER_BUTTON_A;
	padbinds.kick = CONTROLLER_BUTTON_X;
	padbinds.grab = CONTROLLER_BUTTON_B;
	padbinds.grind = CONTROLLER_BUTTON_Y;

	padbinds.leftSpin = CONTROLLER_BUTTON_LEFTSHOULDER;
	padbinds.rightSpin = CONTROLLER_BUTTON_RIGHTSHOULDER;
	padbinds.nollie = CONTROLLER_BUTTON_LEFTTRIGGER;
	padbinds.switchRevert = CONTROLLER_BUTTON_RIGHTTRIGGER;

	padbinds.left = CONTROLLER_BUTTON_DPAD_LEFT;
	padbinds.right = CONTROLLER_BUTTON_DPAD_RIGHT;
	padbinds.up = CONTROLLER_BUTTON_DPAD_UP;
	padbinds.down = CONTROLLER_BUTTON_DPAD_DOWN;
}

void InitDirectInput(void *hwnd, void *hinstance) {
	printf("Initializing Input!\n");

	// init sdl here
	SDL_Init(SDL_INIT_GAMECONTROLLER);

	SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "false");

	char *controllerDbPath[filePathBufLen];
	int result = sprintf_s(controllerDbPath, filePathBufLen,"%s%s", executableDirectory, "gamecontrollerdb.txt");

	if (result) {
		result = SDL_GameControllerAddMappingsFromFile(controllerDbPath);
		if (result) {
			printf("Loaded mappings\n");
		} else {
			printf("Failed to load %s\n", controllerDbPath);
		}
		
	}

	initSDLControllers();
	configureController();

	registerEventHandler(processInputEvent);
}

void ReadDirectInput() {
	anyButtonPressed = 0;
	handleEvents();
	processController();
	processMouse();
}

void PCINPUT_ActuatorOn(uint32_t controllerIdx, uint32_t duration, uint32_t motor, uint32_t str) {
	// turns out most of the parameters don't matter.  the ps1 release only seems to respond to grinds, with full strength high frequency vibration
	// pc release only responds with half-strength on both motors.  i think that's the preferred behavior here
	//str = (uint16_t)(((float)str / 255.0f) * 65535.0f);
	duration *= 66;


	for (int i = 0; i < controllerCount; i++) {
		if (motor == 0) {
			SDL_GameControllerRumble(controllerList[i], 32767, 32767, duration);
		} else {
			SDL_GameControllerRumble(controllerList[i], 32767, 32767, duration);
		}
	}
}

void PCINPUT_ShutDown() {
	printf("STUB: PCINPUT_ShutDown()\n");
}

void PCINPUT_Load() {
	printf("STUB: PCINPUT_Load()\n");
}

void PCINPUT_Save() {
	printf("STUB: PCINPUT_Save()\n");
}

int __cdecl getSomethingIdk(int a) {
	// checks that all buttons are released maybe?  either way i don't think we need it anymore
	//printf("STUB: idk %d\n", a);

	return 0;
}

int getSkipInput() {
	if (anyButtonPressed) {
		anyButtonPressed = 0;
		return -1;
	} else {
		return 0;
	}
}

void installInputPatches() {
	patchJmp(0x004e1860, InitDirectInput);
	patchJmp(0x004e1c60, ReadDirectInput);
	patchJmp(0x004e1e10, PCINPUT_ActuatorOn);
	patchJmp(0x004e1c30, PCINPUT_ShutDown);
	patchJmp(0x004e1ee0, PCINPUT_Load);
	patchJmp(0x004e2630, PCINPUT_Save);
	patchJmp(0x004e1430, getSkipInput);
	patchJmp(0x004e1390, getSomethingIdk);

	// remove windows event handling in movie player
	patchByte(0x004e3104, 0xeb);
	patchNop(0x004e30f3, 15);
}