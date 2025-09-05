#include <stdio.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <global.h>
#include <patch.h>
#include <event.h>
#include <gfx/gfx.h>
#include <config.h>
#include <log.h>

//

struct keybinds {
	SDL_Scancode menu;
	SDL_Scancode cameraToggle;

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
};

//

int controllerCount;
int controllerListSize;
SDL_GameController **controllerList;
int activeController = -1;
struct keybinds keybinds;
struct controllerbinds padbinds;

uint8_t isCursorActive = 1;
uint8_t isUsingKeyboard = 1;
uint8_t isUsingHardCodeControls = 1;
uint8_t anyButtonPressed = 0;

void setUsingKeyboard(uint8_t usingKeyboard) {
	isUsingKeyboard = usingKeyboard;

	if (usingKeyboard) {
		activeController = -1;
	}

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
	log_printf(LL_INFO, "Detected controller \"%s\"\n", SDL_GameControllerNameForIndex(idx));

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
		//activeController = controllerCount;
		controllerCount++;
	}
}

void removeController(SDL_GameController *controller) {
	log_printf(LL_INFO, "Controller \"%s\" disconnected\n", SDL_GameControllerName(controller));

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

		if (activeController == i) {
			activeController = -1;
		}
	} else {
		log_printf(LL_WARN, "Did not find disconnected controller in list\n");
	}
}

void setActiveController(SDL_GameController *controller) {
	int i = 0;

	while (i < controllerCount && controllerList[i] != controller) {
		i++;
	}

	if (controllerList[i] == controller) {
		activeController = i;
	}
}

void initSDLControllers() {
	log_printf(LL_INFO, "Initializing Controller Input\n");

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

void getStick(SDL_GameController *controller, controllerStick stick, int16_t *xOut, int16_t *yOut) {
	if (stick == CONTROLLER_STICK_LEFT) {
		*xOut = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
		*yOut = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
	} else if (stick == CONTROLLER_STICK_RIGHT) {
		*xOut = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
		*yOut = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
	} else {
		*xOut = 0;
		*yOut = 0;
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

		int16_t moveX, moveY;
		getStick(controller, padbinds.movement, &moveX, &moveY);
		if (moveX > 16383) {
			result |= 0x01 << 13;	// right
		} else if (moveX < -16383) {
			result |= 0x01 << 15;	// left
		}

		if (moveY > 16383) {
			result |= 0x01 << 14;	// up
		} else if (moveY < -16383) {
			result |= 0x01 << 12;	// down
		}

		if (result) {
			anyButtonPressed |= 1;
		}
	}

	return result;
}

// global store for which hardcoded keys are overriden by user binds
struct keyOverrides {
	uint8_t menu;
	uint8_t accept;
	uint8_t up;
	uint8_t down;
	uint8_t left;
	uint8_t right;
} keyOverrides;

void setKeyOverrides() {
	// initialize overrides to all false
	memset(&keyOverrides, 0, sizeof(keyOverrides));

	// pretty gnarly code, but iterate through all keybinds and mark any overriden keys
	for (SDL_Scancode* i = &keybinds; i < &keybinds + (sizeof(keybinds) / sizeof(SDL_Scancode)); i++) {
		switch (*i) {
		case SDL_SCANCODE_ESCAPE:
			keyOverrides.menu = 1;
			break;
		case SDL_SCANCODE_RETURN:
			keyOverrides.accept = 1;
			break;
		case SDL_SCANCODE_UP:
			keyOverrides.up = 1;
			break;
		case SDL_SCANCODE_DOWN:
			keyOverrides.down = 1;
			break;
		case SDL_SCANCODE_LEFT:
			keyOverrides.left = 1;
			break;
		case SDL_SCANCODE_RIGHT:
			keyOverrides.right = 1;
			break;
		default:
			break;
		}
	}
}

uint32_t escState = 0;

uint16_t pollKeyboard() {
	int *gShellMode = 0x006a35b4;
	uint8_t *isMenuOpen = 0x0069e050;

	uint32_t numKeys = 0;
	uint8_t *keyboardState = SDL_GetKeyboardState(&numKeys);

	// check keyboard state 
	for (int i = 0; i < numKeys; i++) {
		if (keyboardState[i]) {
			anyButtonPressed |= 1;
		}
	}

	//printf("gShellMode: %d\n", *isMenuOpen);

	uint16_t result = 0;

	// slight deviation from original behavior: esc goes back in pause/end run menu
	// to prevent esc double-pressing on menu transitions, process its state here.  
	// escState = 1 means esc was pressed in menu, 2 means pressed out of menu, 0 is unpressed
	if ((keyboardState[SDL_SCANCODE_ESCAPE] && !keyOverrides.menu) && !escState) {
		escState = (*isMenuOpen) ? 1 : 2;
	} else if (!keyboardState[SDL_SCANCODE_ESCAPE]) {
		escState = 0;
	}

	// buttons
	if (keyboardState[keybinds.menu] || escState == 2) {	// is esc is bound to menu, it will only interfere with hardcoded keybinds.  similar effect on enter but i can't detect the things needed to work there
		result |= 0x01 << 11;
	}
	if (keyboardState[keybinds.cameraToggle]) {
		result |= 0x01 << 8;
	}

	if (keyboardState[keybinds.grind] || escState == 1) {
		result |= 0x01 << 4;
	}
	if (keyboardState[keybinds.grab]) {
		result |= 0x01 << 5;
	}
	if (keyboardState[keybinds.ollie] || (keyboardState[SDL_SCANCODE_RETURN] && !keyOverrides.accept)) {
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
	if (keyboardState[keybinds.up] || (keyboardState[SDL_SCANCODE_UP] && !keyOverrides.up)) {
		result |= 0x01 << 12;
	}
	if (keyboardState[keybinds.right] || (keyboardState[SDL_SCANCODE_RIGHT] && !keyOverrides.right)) {
		result |= 0x01 << 13;
	}
	if (keyboardState[keybinds.down] || (keyboardState[SDL_SCANCODE_DOWN] && !keyOverrides.down)) {
		result |= 0x01 << 14;
	}
	if (keyboardState[keybinds.left] || (keyboardState[SDL_SCANCODE_LEFT] && !keyOverrides.left)) {
		result |= 0x01 << 15;
	}

	return result;
}

// returns 1 if a text entry prompt is on-screen so that keybinds don't interfere with text entry confirmation/cancellation
int isKeyboardTyping() {
	return 0;
}

void __cdecl processController() {
	int *gShellMode = 0x006a35b4;

	anyButtonPressed &= 2;
	uint16_t controlData = 0;

	if (!isKeyboardTyping()) {
		controlData |= pollKeyboard();
	}

	// TODO: maybe smart selection of active controller?
	for (int i = 0; i < controllerCount; i++) {
		controlData |= pollController(controllerList[i]);
	}

	uint32_t *gForcedButtons = 0x006a35bc;
	uint32_t *gLockForcedButtons = 0x006a35c0;

	if (*gLockForcedButtons > 0) {
		*gLockForcedButtons = *gLockForcedButtons - 1;
	}

	if (*gForcedButtons) {
		controlData |= *gForcedButtons;
		*gForcedButtons = 0;
	}

	uint16_t *pControlData = 0x006a0b6c;

	if (*gShellMode == 0) {
		int32_t *netPlayer = 0x006a0978;

		pControlData[*netPlayer] = controlData;
	} else {
		int32_t *gActivePlayer = 0x005674e8;

		pControlData[*gActivePlayer] = controlData;
	}
}

uint32_t click_vblank = 0;

void processMouse() {
	uint32_t *vblanks = 0x0056af7c;
	uint8_t *shouldCursorBeShown = 0x0069e050;
	if (*shouldCursorBeShown && !isCursorActive) {
		setCursorActive();
	} else if (!(*shouldCursorBeShown) && isCursorActive) {
		setCursorInactive();
	}

	uint32_t mouseX, mouseY, resX, resY;
	uint32_t mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
	mouseButtons &= 0x01;

	toGameScreenCoord(mouseX, mouseY, &mouseX, &mouseY);

	mouseX *= 512;
	mouseY *= 240;
	
	getGameResolution(&resX, &resY);
	mouseX /= resX;
	mouseY /= resY;

	uint32_t *gameMouseX = 0x0069e040;
	uint32_t *gameMouseY = 0x0069e044;
	uint32_t *gameMouseButtons = 0x0069e048;

	uint8_t *mouseClicked = 0x0069e04c;
	uint8_t *mouseChanged = 0x0069e04d;
	uint8_t *mouseUnk1 = 0x0069e04f;
	uint32_t *mouseUnk2 = 0x0069e054;

	if (*vblanks != click_vblank) {
		*mouseClicked = 0;
	}
	
	if (*gameMouseX == mouseX && *gameMouseY == mouseY && *gameMouseButtons == mouseButtons) {
		*mouseChanged = 0;
	} else {
		*mouseChanged = 1;
		*mouseUnk1 = 0;
		*mouseUnk2 = 0;
		if (*gameMouseButtons != mouseButtons && mouseButtons == 0) {
			click_vblank = *vblanks;
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
				log_printf(LL_INFO, "Not a game controller: %s\n", SDL_JoystickNameForIndex(e->cdevice.which));
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
			log_printf(LL_DEBUG, "Joystick added: %s\n", SDL_JoystickNameForIndex(e->jdevice.which));
			setUsingKeyboard(0);
			return;
		case SDL_CONTROLLERBUTTONDOWN:
			setActiveController(SDL_GameControllerFromInstanceID(e->cbutton.which));
			setUsingKeyboard(0);
			return;
		case SDL_CONTROLLERAXISMOTION:
			setActiveController(SDL_GameControllerFromInstanceID(e->caxis.which));
			setUsingKeyboard(0);
			return;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION: {
			setUsingKeyboard(1);
			return;
		}
		case SDL_KEYDOWN: 
			setUsingKeyboard(1);
			return;
		default:
			return 0;
	}
}

#define KEYBIND_SECTION "Keybinds"
#define GAMEPAD_SECTION "Gamepad"

void configureControls() {
	// Keyboard
	keybinds.menu = getConfigInt(KEYBIND_SECTION, "Pause", SDL_SCANCODE_P);
	keybinds.cameraToggle = getConfigInt(KEYBIND_SECTION, "ViewToggle", SDL_SCANCODE_O);
	
	keybinds.ollie = getConfigInt(KEYBIND_SECTION, "Ollie", SDL_SCANCODE_KP_2);
	keybinds.kick = getConfigInt(KEYBIND_SECTION, "Flip", SDL_SCANCODE_KP_4);
	keybinds.grab = getConfigInt(KEYBIND_SECTION, "Grab", SDL_SCANCODE_KP_6);
	keybinds.grind = getConfigInt(KEYBIND_SECTION, "Grind", SDL_SCANCODE_KP_8);
	
	keybinds.leftSpin = getConfigInt(KEYBIND_SECTION, "SpinLeft", SDL_SCANCODE_KP_7);
	keybinds.rightSpin = getConfigInt(KEYBIND_SECTION, "SpinRight", SDL_SCANCODE_KP_9);
	keybinds.nollie = getConfigInt(KEYBIND_SECTION, "Nollie", SDL_SCANCODE_KP_1);
	keybinds.switchRevert = getConfigInt(KEYBIND_SECTION, "Switch", SDL_SCANCODE_KP_3);
	
	keybinds.left = getConfigInt(KEYBIND_SECTION, "Left", SDL_SCANCODE_A);
	keybinds.right = getConfigInt(KEYBIND_SECTION, "Right", SDL_SCANCODE_D);
	keybinds.up = getConfigInt(KEYBIND_SECTION, "Up", SDL_SCANCODE_W);
	keybinds.down = getConfigInt(KEYBIND_SECTION, "Down", SDL_SCANCODE_S);

	// Gamepad
	padbinds.menu = getConfigInt(GAMEPAD_SECTION, "Pause", CONTROLLER_BUTTON_START);
	padbinds.cameraToggle = getConfigInt(GAMEPAD_SECTION, "ViewToggle", CONTROLLER_BUTTON_BACK);

	padbinds.ollie = getConfigInt(GAMEPAD_SECTION, "Ollie", CONTROLLER_BUTTON_A);
	padbinds.kick = getConfigInt(GAMEPAD_SECTION, "Flip", CONTROLLER_BUTTON_X);
	padbinds.grab = getConfigInt(GAMEPAD_SECTION, "Grab", CONTROLLER_BUTTON_B);
	padbinds.grind = getConfigInt(GAMEPAD_SECTION, "Grind", CONTROLLER_BUTTON_Y);

	padbinds.leftSpin = getConfigInt(GAMEPAD_SECTION, "SpinLeft", CONTROLLER_BUTTON_LEFTSHOULDER);
	padbinds.rightSpin = getConfigInt(GAMEPAD_SECTION, "SpinRight", CONTROLLER_BUTTON_RIGHTSHOULDER);
	padbinds.nollie = getConfigInt(GAMEPAD_SECTION, "Nollie", CONTROLLER_BUTTON_LEFTTRIGGER);
	padbinds.switchRevert = getConfigInt(GAMEPAD_SECTION, "Switch", CONTROLLER_BUTTON_RIGHTTRIGGER);

	padbinds.left = getConfigInt(GAMEPAD_SECTION, "Left", CONTROLLER_BUTTON_DPAD_LEFT);
	padbinds.right = getConfigInt(GAMEPAD_SECTION, "Right", CONTROLLER_BUTTON_DPAD_RIGHT);
	padbinds.up = getConfigInt(GAMEPAD_SECTION, "Up", CONTROLLER_BUTTON_DPAD_UP);
	padbinds.down = getConfigInt(GAMEPAD_SECTION, "Down", CONTROLLER_BUTTON_DPAD_DOWN);

	padbinds.movement = getConfigInt(GAMEPAD_SECTION, "MovementStick", CONTROLLER_STICK_LEFT);

	setKeyOverrides();
}

void InitDirectInput(void *hwnd, void *hinstance) {
	log_printf(LL_INFO, "Initializing Input!\n");

	// init sdl here
	SDL_Init(SDL_INIT_GAMECONTROLLER);

	SDL_SetHint(SDL_HINT_GAMECONTROLLER_USE_BUTTON_LABELS, "false");

	char *controllerDbPath[filePathBufLen];
	int result = sprintf_s(controllerDbPath, filePathBufLen,"%s%s", executableDirectory, "gamecontrollerdb.txt");

	if (result) {
		result = SDL_GameControllerAddMappingsFromFile(controllerDbPath);
		if (result) {
			log_printf(LL_INFO, "Loaded controller database\n");
		} else {
			log_printf(LL_WARN, "Failed to load %s\n", controllerDbPath);
		}
		
	}

	initSDLControllers();
	configureControls();

	registerEventHandler(processInputEvent);
}

void ReadDirectInput() {
	handleEvents();
	processController();
	processMouse();
}

void PCINPUT_ActuatorOn(uint32_t controllerIdx, uint32_t duration, uint32_t motor, uint32_t str) {
	// turns out most of the parameters don't matter.  the ps1 release only seems to respond to grinds, with full strength high frequency vibration
	// pc release only responds with half-strength on both motors.  i think that's the preferred behavior here
	//str = (uint16_t)(((float)str / 255.0f) * 65535.0f);
	if (activeController >= 0) {
		duration *= 66;

		if (motor == 0) {
			SDL_GameControllerRumble(controllerList[activeController], 32767, 32767, duration);
		} else {
			SDL_GameControllerRumble(controllerList[activeController], 32767, 32767, duration);
		}
	}
}

void PCINPUT_ShutDown() {
	//printf("STUB: PCINPUT_ShutDown()\n");
}

void PCINPUT_Load() {
	//printf("STUB: PCINPUT_Load()\n");
}

void PCINPUT_Save() {
	//printf("STUB: PCINPUT_Save()\n");
}

int __cdecl getSomethingIdk(int a) {
	// checks that all buttons are released maybe?  either way i don't think we need it anymore
	//printf("STUB: idk %d\n", a);

	return 0;
}

int getSkipInput() {
	if (anyButtonPressed == 1) {
		anyButtonPressed |= 2;
		return -1;
	} else {
		if (!(anyButtonPressed & 1)) {
			anyButtonPressed = 0;
		}
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