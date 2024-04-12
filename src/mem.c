#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <patch.h>
#include <util/hash.h>

map_t *memmap = NULL;
int allocbytes = 0;

void initMemAudit() {
	memmap = map_alloc(2048, NULL, NULL);
}

void *(__cdecl *mem_newx)(int, void *, void *, void *) = 0x0046e950;
void *__cdecl mem_new_wrapper(int size, void *b, void *c, int d) {
	if (size < 5) {
		size = 8;
	}
	if (d != 0) {
		d += 0x10;
	}

	printf("MEM_NEW WITH SIZE %d\n", size);

	int result = mem_newx(size, b, c, d);

	//printf("MEM_NEW WITH SIZE %d - 0x%08x\n", size, result);

	if (result) {
		result -= 4;
	}

	map_put(memmap, &result, sizeof(int), &size, sizeof(int));

	allocbytes += size;

	printf("MEM_NEW WITH SIZE %d - 0x%08x, TOTAL: %d\n", size, result, allocbytes);

	return result;
}

void (__cdecl *mem_deletex)(void *) = 0x0046ef90;
void __cdecl mem_delete_wrapper(int p) {
	mem_deletex(p + 4);

	int *sz = map_get(memmap, &p, sizeof(int));

	if (sz) {
		allocbytes -= *sz;

		map_del(memmap, &p, sizeof(int));
	} else {
		printf("0x%08x WASN'T FOUND\n", p);
		while(1) {
		
		}
	}
	

	printf("MEM_DELETE - 0x%08x, TOTAL: %d\n", p, allocbytes);
}

void installMemAudit() {
	patchJmp(0x0046f420, mem_new_wrapper);
	patchJmp(0x0046f460, mem_delete_wrapper);
}

void *__cdecl mem_new(int size, int a, int b, int c) {
	
	uint32_t *result = malloc(size + 4);

	*result = size;

	#ifdef MEM_AUDIT
	allocbytes += size;
	printf("allocated %d bytes at 0x%08x (total %d)\n", size, result, allocbytes);
	#endif

	return result + 1;
}

void __cdecl mem_delete(void *p) {
	if (p) {
		uint32_t *pp = ((uint32_t *)p) - 1;

		#ifdef MEM_AUDIT
		allocbytes -= *pp;

		printf("mem delete for 0x%08x (total allocated %d)\n", pp, allocbytes);
		#endif

		free(pp);
	}
}

void __cdecl mem_copy(void *dst, void *src, int size) {
	memcpy(dst, src, size);
}

void __cdecl mem_copybytes(void *dst, void *src, int size) {
	memcpy(dst, src, size);
}

void __cdecl mem_shrink(void *p, int newsize) {
	//printf("mem shrink 0x%08x to %d (not implemented)\n", p, newsize);
}

int __cdecl mem_getblocksize(void *p) {
	if (p) {
		uint32_t *pp = ((uint32_t *)p) - 1;

		return *pp;
	} else {
		return 0;
	}
}

void installAltMemManager() {
	patchJmp(0x0046f420, mem_new);
	patchJmp(0x0046f460, mem_delete);
	patchJmp(0x0046f270, mem_copy);
	patchJmp(0x0046f340, mem_copybytes);
	patchJmp(0x0046f470, mem_shrink);
	patchJmp(0x0046f4a0, mem_getblocksize);
}