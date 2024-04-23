#ifndef _EVENT_H_
#define _EVENT_H_

#include <SDL2/SDL.h>

typedef void (*eventHandler)(SDL_Event *);

void initEvents();
void handleEvents();
void registerEventHandler(eventHandler handler);

#endif