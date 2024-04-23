#include <SDL2/SDL.h>

#include <event.h>
#include <util/sb.h>

struct stretchyBuffer *eventHandlers;

void initEvents() {
	eventHandlers = sb_alloc(sizeof(eventHandler), 4);
}

void registerEventHandler(eventHandler handler) {
	sb_push_back(eventHandlers, &handler);
}

void handleEvents() {
	eventHandler *handlers = eventHandlers->data;

	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		for (int i = 0; i < eventHandlers->count; i++) {
			handlers[i](&e);
		}
	}
}
