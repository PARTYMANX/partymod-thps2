#include <stdlib.h>
#include <stdio.h>

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

void *__cdecl mem_new_simple(int size, void *b, void *c, int d) {
	return malloc(size);
}

void __cdecl mem_delete_simple(void *p) {
	free(p);
}

void installMemAudit() {
	patchJmp(0x0046f420, mem_new_wrapper);
	patchJmp(0x0046f460, mem_delete_wrapper);
}