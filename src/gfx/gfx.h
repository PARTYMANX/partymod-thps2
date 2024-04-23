#ifndef _GFX_H_
#define _GFX_H_

void installGfxPatches();
void toGameScreenCoord(int x, int y, int *xOut, int *yOut);
void getGameResolution(int *w, int *h);

#endif