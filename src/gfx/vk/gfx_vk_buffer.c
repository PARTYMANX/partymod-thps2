#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <gfx/vk/vk.h>


// why's this a function anyway?
void createBuffer(partyRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage usage, rbVkBuffer *dst) {
	rbVkBuffer result;

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
		printf("Failed allocation of size %lld: 0x%08x!\n", size, r);
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

// TODO: access hints or something
/*vid2_buffer_t *vid2_createBuffer(vid2_renderer_t *renderer, size_t size, vid2_memoryUsage_t usage) {
	vid2_buffer_t *result = malloc(sizeof(vid2_buffer_t));

	result->renderer = renderer;
	//result->usage = usage;
	result->size = size;

	VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

	if (usage == VID2_MEMORY_USAGE_GPU_ONLY) {
		usageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}

	//VkMemoryPropertyFlags propFlags = (props) ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

	createBuffer(renderer, size, usageFlags, usage, &(result->buffer), &(result->memory));

	return result;
}

vid2_buffer_t *createStagingBuffer(vid2_renderer_t *renderer, VkDeviceSize size) {
	vid2_buffer_t *result = malloc(sizeof(vid2_buffer_t));

	result->renderer = renderer;
	result->size = size;

	VkBufferUsageFlags usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	vid2_memoryUsage_t memUsage = VID2_MEMORY_USAGE_CPU_TO_GPU;

	createBuffer(renderer, size, usageFlags, memUsage, &(result->buffer), &(result->memory));

	return result;
}*/

void destroyBuffer(partyRenderer *renderer, rbVkBuffer *buffer) {
	vkDestroyBuffer(renderer->device->device, buffer->buffer, NULL);
	vmaFreeMemory(renderer->memoryManager->allocator, buffer->allocation);
}

void *mapBuffer(partyRenderer *renderer, rbVkBuffer *buffer) {
	void *result;
	
	if (vmaMapMemory(renderer->memoryManager->allocator, buffer->allocation, &result) != VK_SUCCESS) {
		printf("FAILED TO MAP MEMORY\n");
	}

	return result;
}

void unmapBuffer(partyRenderer *renderer, rbVkBuffer *buffer) {
	vmaUnmapMemory(renderer->memoryManager->allocator, buffer->allocation);
}

/*void setBuffer(partyRenderer *renderer, rbVkBuffer *buffer, void *data, size_t sz) {
	void *buf;

	if (!buffer->memory->usage == VID2_MEMORY_USAGE_GPU_ONLY) {	// memory is cpu accessible (used for uniforms)
		buf = vid2_mapBuffer(buffer);

		memcpy(buf, data, sz);

		vid2_unmapBuffer(buffer);
	} else {	// memory is not cpu accessible
		vid2_buffer_t *stagingBuffer = createStagingBuffer(buffer->renderer, sz);

		buf = mapMemory(stagingBuffer->memory);
		memcpy(buf, data, sz);
		unmapMemory(stagingBuffer->memory);

		VkCommandBuffer cmdbuf = startStagingCommandBuffer(buffer->renderer);

		VkBufferCopy bufferCopy;
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = 0;
		bufferCopy.size = buffer->size;
		
		vkCmdCopyBuffer(cmdbuf, stagingBuffer->buffer, buffer->buffer, 1, &bufferCopy);

		endStagingCommandBuffer(buffer->renderer, cmdbuf);

		vid2_destroyBuffer(stagingBuffer);
	}
}*/