#ifndef _WINDOW_H_
#define _WINDOW_H_

void installWindowPatches(void *window_offset);
void getWindowSize(int *w, int *h);
void createErrorMessageBox(const char *msg);

#endif