#ifndef _SB_H_
#define _SB_H_

struct stretchyBuffer {
	void *data;
	size_t unit;
	size_t capacity;
	size_t count;
};

struct stretchyBuffer *sb_alloc(size_t unit, size_t initial_capacity);
void sb_push_back(struct stretchyBuffer *buffer, void *data);
void sb_pop(struct stretchyBuffer *buffer, void *dst);
void sb_pop_front(struct stretchyBuffer *buffer, void *dst);

#endif