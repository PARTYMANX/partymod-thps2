#include <stdint.h>

#include <patch.h>

uint32_t supportsVibration() {
	return 1;
}

// so it turns out the PS1 controls menu is just there as dead code.  so we can restore it extremely easily
void __fastcall Options_TransitionToControlMenu(uint32_t options_menu) {
	void *(__cdecl *CClass_OpNew)(size_t sz) = 0x00466b00;
	void *(__fastcall *ControlsMenuConstructor)(void *, void *, uint32_t, void *, uint32_t, uint32_t, uint32_t, uint32_t, void *, void *) = 0x00484d80;

	*((uint32_t *)(options_menu + 0x970)) = 0x1f;	// set transition to controls menu

	// create the controls menu
	void *controls_menu = CClass_OpNew(0x260);	// we may be overallocating here, but we don't really have a great way of knowing how big it's supposed to be

	if (controls_menu) {
		*((void **)(options_menu + 0x994)) = ControlsMenuConstructor(controls_menu, NULL, 0, options_menu + 0xb4, 0, 0, -1, -1, *((void**)(options_menu + 0x964)), *((void**)(options_menu + 0x968)));
	} else {
		*((void **)(options_menu + 0x994)) = NULL;
	}
}

void patchOptionsMenu() {
	// get rid of display controls menu
	patchNop(0x0047f22d, 5);
	patchByte(0x0047f240, 0xeb);

	uint8_t movEcxEbp[] = { 0x89, 0xe9 };

	patchNop(0x0048097b, 31);	// clear transition to PC controls menu
	patchBytes(0x0048097b, movEcxEbp, 2);	// move options menu object to ecx for fast/thiscall
	patchCall(0x0048097b + 2, Options_TransitionToControlMenu);	// call the transition which does the setup for the menu
	patchJmp(0x0048097b + 7, 0x00480bfb);	// jmp to after switch statement

	patchCall(0x00485772, supportsVibration);	// always return true when checking for controller vibration support (to make sure vibration option is accessible)
}

void installOptionsPatches() {
	patchOptionsMenu();
}