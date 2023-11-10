#include <iostream>
#include <Windows.h>
#include <detours/detours.h>


int main(int argc, char **argv) {
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);

	DetourCreateProcessWithDll(NULL, "THawk2.exe", NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi, "partymod.dll", NULL);

    return 0;
}