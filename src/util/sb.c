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
		buffer->data = realloc(buffer->data, buffer->capacity);
	}

	memcpy(((uint8_t *)buffer->data) + (buffer->unit * buffer->count), data, buffer->unit);

	buffer->count++;
}

void sb_pop(struct stretchyBuffer *buffer, void *dst) {
	if (dst) {
		memcpy(dst, ((uint8_t *)buffer->data) + (buffer->unit * (buffer->count - 1)), buffer->unit);
	}

	buffer->count--;
}