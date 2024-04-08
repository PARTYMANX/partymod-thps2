#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

//#include "vid_vk.h"
#include <gfx/vk/vk.h>

#define BUDDY_FREE 0x00
#define BUDDY_OCCUPIED 0x01
#define BUDDY_SPLIT 0x06
#define BUDDY_LEFT 0x02
#define BUDDY_RIGHT 0x04

#define BLOCK_SIZE 1024

// NOTE: VulkanMemoryAllocator allocates 256MiB or (heapSize / 8) if the heap is < 1GiB
// probably follow that, gpu's have a ton of memory these days

/*
	STRUCTS
*/

/*typedef struct {
	uint64_t idx;
} handle_t;

typedef struct {
	uint8_t order, occupied;
} buddyBlock_t;

typedef struct {
	uint8_t highestOrder;

	buddyBlock_t *blocks;
	void *data;
} memory_t;*/

/*
	BUDDY FUNCTIONS
*/

/*uint64_t lchild(uint64_t idx) {
	return (idx << 1) + 1;
}

uint64_t rchild(uint64_t idx) {
	return (idx << 1) + 2;
}

uint64_t parent(uint64_t idx) {
	return ((idx + 1) >> 1) - 1;
}

uint64_t reqBlocks(uint8_t order) {
	return (1 << (order + 1)) - 1;
}

uint8_t highestPow(uint64_t n) {
	uint8_t i = 0;
	while (n) {
		n >>= 1;
		i++;
	}

	return i;
}

uint64_t findBlockR(buddyBlock_t *pBlocks, uint64_t idx, uint8_t minOrder) {
	if (!pBlocks[idx].occupied)
		return idx;

	return 0;
}

uint64_t findBlock(buddyBlock_t *pBlocks, size_t sz) {
	uint8_t minOrder = highestPow((uint64_t)sz);

	return findBlockR(pBlocks, 0, minOrder);
}

// NOTE: unused
void fillBlocksR(buddyBlock_t *pBlocks, uint64_t idx, uint8_t order) {
	pBlocks[idx].order = order;
	pBlocks[idx].occupied = BUDDY_FREE;

	if (order == 0)
		return;
	else {
		fillBlocksR(pBlocks, lchild(idx), order - 1);
		fillBlocksR(pBlocks, rchild(idx), order - 1);
	}
}

void copyBlocksR(buddyBlock_t *pDst, buddyBlock_t *pSrc, uint64_t idxDst, uint64_t idxSrc) {
	pDst[idxDst].order = pSrc[idxSrc].order;
	pDst[idxDst].occupied = pSrc[idxSrc].occupied;

	if (pSrc[idxSrc].order == 0)
		return;
	else {
		copyBlocksR(pDst, pSrc, lchild(idxDst), lchild(idxSrc));
		copyBlocksR(pDst, pSrc, rchild(idxDst), rchild(idxSrc));
	}
}

// FIXME: never finds valid children
// what about min order?
uint64_t findBuddy(buddyBlock_t *b, uint64_t idx, uint8_t minOrder) {
	if (!b[idx].occupied) {
		if (b[idx].order >= minOrder)
			return idx;	// found a free block!
		else
			return UINT64_MAX;
	}

	if (b[idx].occupied & BUDDY_OCCUPIED || b[idx].order <= 1)
		return UINT64_MAX;	// block holds data or is bottom

	uint64_t l = UINT64_MAX;
	uint64_t r = UINT64_MAX;

	// if this block is split, check left/right
	if (b[idx].occupied & BUDDY_SPLIT) {
		l = findBuddy(b, lchild(idx), minOrder);
		r = findBuddy(b, rchild(idx), minOrder);
	}

	if (l == UINT64_MAX) {
		return r;
	} else if (r == UINT64_MAX) {
		return l;
	} else {
		// if right order is lower than left order, that's the only case we want right (left either has lower idx or lower order otherwise)
		if (b[r].order < b[l].order)
			return r;
		else
			return l;
	}
}

uint64_t allocateBlock(buddyBlock_t *b, uint64_t idx, uint8_t order) {
	if (b[idx].order == order) {
		b[idx].occupied = BUDDY_OCCUPIED;
		printf("allocated block %llu\n", idx);
		return idx;
	}

	// split the block!!
	b[idx].occupied = BUDDY_LEFT;

	// put data in left child!!
	uint64_t l = lchild(idx);
	b[l].order = b[idx].order - 1;
	b[rchild(idx)].order = b[idx].order - 1;

	return allocateBlock(b, l, order);
}

void destroyLink(buddyBlock_t *b, uint64_t idx) {
	if (idx == 0)
		return;

	uint64_t par = parent(idx);

	if (idx == lchild(par))
		b[par].occupied -= BUDDY_LEFT;
	else
		b[par].occupied -= BUDDY_RIGHT;

	if (!b[par].occupied)
		destroyLink(b, par);
}

void freeBlock(buddyBlock_t *b, uint64_t idx) {
	b[idx].occupied = BUDDY_FREE;

	destroyLink(b, idx);
}*/

/*
	MEMORY FUNCTIONS
*/

// TODO: make this less init, more oriented toward superblocks
/*memory_t *memoryCreate(uint8_t initOrder) {
	memory_t *result = malloc(sizeof(memory_t));

	result->highestOrder = initOrder;

	size_t blocksz = reqBlocks(result->highestOrder);
	result->blocks = malloc(sizeof(buddyBlock_t) * blocksz);
	memset(result->blocks, 0, sizeof(buddyBlock_t) * blocksz);

	result->blocks[0].order = result->highestOrder;
	result->blocks[0].occupied = BUDDY_FREE;

	result->data = malloc(BLOCK_SIZE * (1 << result->highestOrder));

	VkMemoryAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.allocationSize = BLOCK_SIZE * (1 << result->highestOrder);
	allocInfo.memoryTypeIndex = 0;	// TODO: get a type

	//vkAllocateMemory(TODO: device, &allocInfo, NULL, &result->data);	// TODO: make data the correct type
	// TODO: store type in result

	return result;
}

// TODO: remove
void memoryIncrease(memory_t *mem) {
	mem->highestOrder++;

	buddyBlock_t *oldBlocks = mem->blocks;

	size_t blocksz = reqBlocks(mem->highestOrder);
	mem->blocks = malloc(sizeof(buddyBlock_t) * blocksz);
	memset(mem->blocks, 0, sizeof(buddyBlock_t) * blocksz);

	mem->blocks[0].order = mem->highestOrder;
	mem->blocks[0].occupied = (oldBlocks[0].occupied) ? BUDDY_LEFT : BUDDY_FREE;
	copyBlocksR(mem->blocks, oldBlocks, lchild(0), 0);

	free(oldBlocks);

	mem->data = realloc(mem->data, BLOCK_SIZE * (1 << mem->highestOrder));
}

// TODO: vkFreeMemory
void memoryDestroy(memory_t *mem) {
	free(mem->blocks);
	free(mem->data);
	free(mem);
}

handle_t *memoryAllocate(memory_t *mem, size_t sz) {
	uint8_t minOrder = highestPow(sz / BLOCK_SIZE);
	printf("minOrder = %u, order = %u\n", minOrder, mem->highestOrder);

	uint64_t idx = findBuddy(mem->blocks, 0, minOrder);

	// TODO: allocate new superblock
	if (idx == UINT64_MAX) {
		printf("increasing size of mem!\n");
		memoryIncrease(mem);
		idx = rchild(idx);
	}

	printf("found idx = %llu\n", idx);

	handle_t *result = malloc(sizeof(handle_t));

	result->idx = allocateBlock(mem->blocks, idx, minOrder);

	return result;
}

// TODO: vkFreeMemory
void memoryFree(memory_t *mem, handle_t *hdl) {
	freeBlock(mem->blocks, hdl->idx);

	free(hdl);
} */

/*
	HANDLE FUNCTIONS
*/

/*
int main(int argc, char **argv) {
	memory_t *mem = memoryCreate(12);

	handle_t *hdl = memoryAllocate(mem, 500000);
	memoryFree(mem, hdl);
	hdl = memoryAllocate(mem, 500000);
	memoryFree(mem, hdl);
	hdl = memoryAllocate(mem, 50000);
	memoryFree(mem, hdl);
	hdl = memoryAllocate(mem, 500000);
	memoryFree(mem, hdl);
	hdl = memoryAllocate(mem, 50000);
	memoryFree(mem, hdl);

	memoryDestroy(mem);

	return 0;
}
*/

/*
	memory manager design

	everything's stored in a central memory manager struct
	when an allocation happens, first it checks for a suitable memory 'heap':
	1. by type
	2. by size (if it can fit a valid block)
	if it cannot find a suitable heap, one is allocated by searching for a suitable physical heap
	a heap, by default, will have a constant size (maybe 128MiB?), or if the physical heap is smaller than a constant size (maybe 512MiB?), it will just allocate a certain fraction (1/8?) of that
	if the allocation is larger than a heap, a heap of the next power of two will be allocated (if that's possible), if there is not enough space for that, allocation will fail.

	to start with, it will only use the buddy system with fixed block sizes (1k?) for simplicity.  i would like to make more granular management at some point
*/

// TODO: memory types

/*
uint8_t findMemoryType(vid2_renderer_t *renderer, uint32_t filter, VkMemoryPropertyFlags flags, uint32_t *pType) {
	VkPhysicalDeviceMemoryProperties memProp;
	vkGetPhysicalDeviceMemoryProperties(renderer->device->physicalDevice.device, &memProp);

	for (uint32_t i = 0; i < memProp.memoryTypeCount; i++) {
		if ((filter & (1 << i)) && (memProp.memoryTypes[i].propertyFlags & flags) == flags) {
			*pType = i;
			return 1;
		}
	}

	return 0;
}

uint8_t vMemoryAlloc(vid2_renderer_t *renderer, VkMemoryAllocateInfo allocInfo, VkDeviceMemory *memory) {
	// TODO: use memory manager

	if (vkAllocateMemory(renderer->device->device, &allocInfo, NULL, memory) != VK_SUCCESS) {
		printf("OOPS!  DARN!\n");
		return 0;
	}

	return 1;
}
*/

/*const VmaMemoryUsage usageLUT[] = {
	[VID2_MEMORY_USAGE_CPU_ONLY] = VMA_MEMORY_USAGE_CPU_ONLY,
	[VID2_MEMORY_USAGE_CPU_TO_GPU] = VMA_MEMORY_USAGE_CPU_TO_GPU,
	[VID2_MEMORY_USAGE_GPU_ONLY] = VMA_MEMORY_USAGE_GPU_ONLY,
	[VID2_MEMORY_USAGE_GPU_TO_CPU] = VMA_MEMORY_USAGE_GPU_TO_CPU
};*/

VkResult rbVkInitMemoryManager(partyRenderer *renderer, struct rbVkMemoryManager **memManager) {
	struct rbVkMemoryManager *result = malloc(sizeof(struct rbVkMemoryManager));

	if (!result) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	*memManager = NULL;

	result->renderer = renderer;

	VmaAllocatorCreateInfo info;
	info.flags = 0;
	info.physicalDevice = renderer->device->physicalDevice.device;
	info.device = renderer->device->device;
	info.preferredLargeHeapBlockSize = 0;
	info.pAllocationCallbacks = NULL;
	info.pDeviceMemoryCallbacks = NULL;
	info.frameInUseCount = 0;
	info.pHeapSizeLimit = NULL;
	info.pVulkanFunctions = NULL;
	info.pRecordSettings = NULL;
	info.instance = instance;
	info.vulkanApiVersion = VK_API_VERSION_1_3;

	VkResult r = vmaCreateAllocator(&info, &(result->allocator));

	if (r) {
		free(result);
		return r;
	}

	*memManager = result;

	return VK_SUCCESS;
}

void rbVkDestroyMemoryManager(struct rbVkMemoryManager *memoryManager) {
	vmaDestroyAllocator(memoryManager->allocator);

	free(memoryManager);
}

/*memory_t *allocateVidMemory(vid2_renderer_t *renderer, memAllocInfo_t *allocInfo) {
	memory_t *result = malloc(sizeof(memory_t));

	result->allocator = renderer->memoryManager->allocator;
	result->usage = allocInfo->usage;

	VmaAllocationCreateInfo info;
	info.flags = 0;
	info.usage = usageLUT[allocInfo->usage];
	info.requiredFlags = 0;
	info.preferredFlags = 0;
	info.memoryTypeBits = 0;
	info.pool = VK_NULL_HANDLE;
	info.pUserData = NULL;
	info.priority = 0.0f;

	// TODO: deal with the result of the allocation
	switch(allocInfo->type) {
		case VID2_MEMORY_TYPE_BUFFER:
			vmaAllocateMemoryForBuffer(renderer->memoryManager->allocator, allocInfo->buffer, &info, &result->allocation, NULL);
			break;
		case VID2_MEMORY_TYPE_IMAGE:
			vmaAllocateMemoryForImage(renderer->memoryManager->allocator, allocInfo->image, &info, &result->allocation, NULL);
			break;
	}
	
	return result;
}

void freeVidMemory(memory_t *mem) {
	vmaFreeMemory(mem->allocator, mem->allocation);

	free(mem);
}

void bindBufferMemory(memory_t *mem, VkBuffer buffer) {
	vmaBindBufferMemory(mem->allocator, mem->allocation, buffer);
}

void bindImageMemory(memory_t *mem, VkImage image) {
	vmaBindImageMemory(mem->allocator, mem->allocation, image);
}

void *mapMemory(memory_t *mem) {
	void *result;
	
	// TODO: deal with result
	vmaMapMemory(mem->allocator, mem->allocation, &result);

	return result;
}

void unmapMemory(memory_t *mem) {
	vmaUnmapMemory(mem->allocator, mem->allocation);
}*/