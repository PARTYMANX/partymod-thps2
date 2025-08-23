#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <volk.h>
//#include <vulkan/vulkan.h>

#include "vma_usage.h"

#include <gfx/vk/vk.h>
#include <log.h>


// why's this a function anyway?
void createBuffer(partyRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage usage, pmVkBuffer *dst) {
	pmVkBuffer result;

	VkBufferCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.size = size,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = NULL,
		.usage = usageFlags,
	};

	//printf("TEST! %lld 0x%08x\n", size, info.usage);

	VkResult r = vkCreateBuffer(renderer->device->device, &info, NULL, &(result.buffer));
	if (r != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed allocation of size %lld: 0x%08x!\n", size, r);
		exit(1);
	}

	VmaAllocationCreateInfo allocInfo = {
		.flags = 0,
		.usage = usage,
		.requiredFlags = 0,
		.preferredFlags = 0,
		.memoryTypeBits = 0,
		.pool = VK_NULL_HANDLE,
		.pUserData = NULL,
		.priority = 0.0f,
	};

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
