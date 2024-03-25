#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <util/sb.h>

struct stretchyBuffer *sb_alloc(size_t unit, size_t initial_capacity){
	struct stretchyBuffer *result = malloc(sizeof(struct stretchyBuffer));

	result->unit = unit;
	result->capacity = initial_capacity;
	result->count = 0;
	result->data = malloc(unit * initial_capacity);

	return result;
}

void sb_push_back(struct stretchyBuffer *buffer, void *data) {
	if (buffer->count >= buffer->capacity) {
		buffer->capacity *= 2;
		//printf("resizing stretchy buffer to %d!\n", buffer->capacity);
		buffer->data = realloc(buffer->data, buffer->unit * buffer->capacity);
	}

	memcpy(((uint8_t *)buffer->data) + (buffer->unit * buffer->count), data, buffer->unit);

	buffer->count++;
}

void sb_pop(struct stretchyBuffer *buffer, void *dst) {
	if (buffer->count > 0) {
		if (dst) {
			memcpy(dst, ((uint8_t *)buffer->data) + (buffer->unit * (buffer->count - 1)), buffer->unit);
		}

		buffer->count--;
	}
}

void sb_pop_front(struct stretchyBuffer *buffer, void *dst) {
	if (dst) {
		memcpy(dst, buffer->data, buffer->unit);
		memmove(buffer->data, ((uint8_t *)buffer->data) + (buffer->unit), buffer->unit * (buffer->count - 1));
	}

	buffer->count--;
}