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

	int result = mem_newx(size, b, c, d);	// subtle difference from original here: allocating four extra bytes as padding.  hopefully fixes out of bounds access the game sometimes does

#ifdef MEM_AUDIT
	printf("MEM_NEW WITH SIZE %d\n", size);

	int headersize = *((uint32_t *)(result - 8)) >> 4;

	if (headersize != size) {
		printf("RESULT DIFFERENT SIZE: %d -> %d\n", size, headersize);
	}
#endif

	if (result) {
		result -= 4;
	}

#ifdef MEM_AUDIT
	if (result & 0x3) {
		printf("NEW 0x%08x NOT LWORD ALIGNED\n");
		while(1) {
		
		}
	}

	map_put(memmap, &result, sizeof(int), &headersize, sizeof(int));

	allocbytes += size;

	printf("MEM_NEW WITH SIZE %d - 0x%08x, TOTAL: %d\n", size, result, allocbytes);
#endif

	return result;
}

void (__cdecl *mem_deletex)(void *) = 0x0046ef90;
void __cdecl mem_delete_wrapper(int p) {
#ifdef MEM_AUDIT
	int headersize = *((uint32_t *)(p - 4)) >> 4;
#endif

	mem_deletex(p + 4);

#ifdef MEM_AUDIT
	int *sz = map_get(memmap, &p, sizeof(int));

	if (sz) {
		if (*sz != headersize) {
			printf("FOUND CORRUPT BLOCK WHILE DELETING 0x%08x - HEADER SIZE: %d, KNOWN SIZE: %d\n", p, headersize, *sz);
			while(1) {

			}
		}
		allocbytes -= *sz;

		map_del(memmap, &p, sizeof(int));
	} else {
		printf("0x%08x WASN'T FOUND\n", p);
		while(1) {
		
		}
	}

	printf("MEM_DELETE - 0x%08x, TOTAL: %d\n", p, allocbytes);
#endif
}

void (__cdecl *mem_shrinkx)(void *, int) = 0x0046f070;
void __cdecl mem_shrink_wrapper(int p, int newsize) {
	if (newsize < 5) {
		newsize = 8;
	}

#ifdef MEM_AUDIT
	if (newsize & 0x3) {
		printf("SHRINKING 0x%08x TO %d - NOT LWORD ALIGNED\n", p, newsize);
		while(1) {
		
		}
	}

	int headersize = *((uint32_t *)(p - 4)) >> 4;
#endif

	mem_shrinkx(p + 4, newsize);

#ifdef MEM_AUDIT
	int *sz = map_get(memmap, &p, sizeof(int));

	if (sz) {
		if (*sz != headersize) {
			printf("FOUND CORRUPT BLOCK WHILE SHRINKING 0x%08x - HEADER SIZE: %d, KNOWN SIZE: %d\n", p, headersize, *sz);
			while(1) {

			}
		}

		allocbytes -= *sz - newsize;

		map_del(memmap, &p, sizeof(int));
		map_put(memmap, &p, sizeof(int), &newsize, sizeof(int));
	} else {
		printf("0x%08x WASN'T FOUND\n", p);
		while(1) {
		
		}
	}
	
	printf("MEM_SHRINK - 0x%08x, %d, TOTAL: %d\n", p, newsize, allocbytes);
#endif
}

void __cdecl bang_wrapper() {
	printf("OUT OF MEMORY!!!!\n");
	while(1) {
	
	}
}

void installMemPatches() {
	patchJmp(0x0046f420, mem_new_wrapper);
	patchJmp(0x0046f460, mem_delete_wrapper);
	patchJmp(0x0046f470, mem_shrink_wrapper);

	patchJmp(0x0046edb0, bang_wrapper);	// OOM crash
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

void __cdecl mem_swap(void *a, void *b, int size) {
	uint8_t *ab = a;
	uint8_t *bb = b;

	for (int i = 0; i < size; i++) {
		uint8_t tmp = ab[i];
		ab[i] = bb[i];
		bb[i] = tmp;
	}
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
	/*patchJmp(0x0046f420, mem_new);
	patchJmp(0x0046f460, mem_delete);
	patchJmp(0x0046f270, mem_copy);
	patchJmp(0x0046f340, mem_copybytes);
	patchJmp(0x0046f370, mem_swap);
	patchJmp(0x0046f470, mem_shrink);
	patchJmp(0x0046f4a0, mem_getblocksize);*/



	/*patchJmp(0x0046f420, 0x0046e950);
	patchJmp(0x0046f460, 0x0046ef90);
	patchJmp(0x0046f470, 0x0046f070);
	patchJmp(0x0046f4a0, 0x0046f1e0);*/

	//patchJmp(0x0046edb0, Bang);	// OOM crash
}