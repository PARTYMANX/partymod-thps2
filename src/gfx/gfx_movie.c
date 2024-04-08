#include <patch.h>

uint32_t __cdecl startBinkMovie(char *name, uint32_t unk1, uint32_t unk2) {
	printf("STUB: startBinkMovie %s, %d, %d\n", name, unk1, unk2);

	return 0;
}

uint32_t __cdecl createBinkSurface() {
	printf("STUB: createBinkSurface\n");

	return 0;
}

void __cdecl stopBinkMovie() {
	printf("STUB: stopBinkMovie\n");
}

uint32_t __cdecl advanceBinkMovie() {
	printf("STUB: advanceBinkMovie\n");

	return 0;
}

void __cdecl drawBinkSurface() {
	printf("STUB: drawBinkSurface\n");
}

void installMoviePatches() {
	patchJmp(0x004e3760, startBinkMovie);
	patchJmp(0x004e3f70, stopBinkMovie);
	patchJmp(0x004e3a60, advanceBinkMovie);

	patchJmp(0x004e31f0, createBinkSurface);
	patchJmp(0x004e2c50, drawBinkSurface);

	//patchJmp(0x004e1390, maybe_isBinkMovieOver); // actually i think this is controller stuff
}