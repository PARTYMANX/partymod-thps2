#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <gfx/vk/vk.h>
#include <log.h>


// why's this a function anyway?
void createBuffer(partyRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage usage, pmVkBuffer *dst) {
	pmVkBuffer result;

	VkBufferCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	info.pNext = NULL;
	info.flags = 0;
	info.size = size;
	info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.queueFamilyIndexCount = 0;
	info.pQueueFamilyIndices = NULL;
	info.usage = usageFlags;

	//printf("TEST! %lld 0x%08x\n", size, info.usage);

	VkResult r = vkCreateBuffer(renderer->device->device, &info, NULL, &(result.buffer));
	if (r != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed allocation of size %lld: 0x%08x!\n", size, r);
		exit(1);
	}

	VmaAllocationCreateInfo allocInfo;
	allocInfo.flags = 0;
	allocInfo.usage = usage;
	allocInfo.requiredFlags = 0;
	allocInfo.preferredFlags = 0;
	allocInfo.memoryTypeBits = 0;
	allocInfo.pool = VK_NULL_HANDLE;
	allocInfo.pUserData = NULL;
	allocInfo.priority = 0.0f;

	vmaAllocateMemoryForBuffer(renderer->memoryManager->allocator, result.buffer, &allocInfo, &(result.allocation), NULL);

	vmaBindBufferMemory(renderer->memoryManager->allocator, result.allocation, result.buffer);

	*dst = result;
}

void destroyBuffer(partyRenderer *renderer, pmVkBuffer *buffer) {
	vkDestroyBuffer(renderer->device->device, buffer->buffer, NULL);
	vmaFreeMemory(renderer->memoryManager->allocator, buffer->allocation);
}

void *mapBuffer(partyRenderer *renderer, pmVkBuffer *buffer) {
	void *result;
	
	if (vmaMapMemory(renderer->memoryManager->allocator, buffer->allocation, &result) != VK_SUCCESS) {
		log_printf(LL_ERROR, "FAILED TO MAP MEMORY\n");
	}

	return result;
}

void unmapBuffer(partyRenderer *renderer, pmVkBuffer *buffer) {
	vmaUnmapMemory(renderer->memoryManager->allocator, buffer->allocation);
}
