#include <stdlib.h>
#include <stdint.h>

#include <vulkan/vulkan.h>

//#include "hash.h"
//#include "handle.h"
//#include "vid.h"
//#include "vid_vk.h"

#include <gfx/vk/vk.h>
#include <log.h>

/*
	QUEUE
*/

VkResult pmVkCreateCommandQueue(struct pmVkDevice *device, VkCommandPoolCreateFlags flags, struct pmVkCommandQueue **queue) {
	struct pmVkCommandQueue *result = malloc(sizeof(struct pmVkCommandQueue));

	if (!result) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	*queue = NULL;

	result->device = device;

	vkGetDeviceQueue(device->device, device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_GRAPHICS], 0, &(result->queue));
	vkGetDeviceQueue(device->device, device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_PRESENT], 0, &(result->presentQueue));

	VkCommandPoolCreateInfo commandPoolInfo;
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.pNext = NULL;
	commandPoolInfo.flags = flags;
	commandPoolInfo.queueFamilyIndex = device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_GRAPHICS];

	VkResult r = vkCreateCommandPool(device->device, &commandPoolInfo, NULL, &(result->commandPool));
	if (r) {
		log_printf(LL_ERROR, "ERROR: Failed to create device command pool!");
		return r;
	}

	*queue = result;

	log_printf(LL_DEBUG, "Command pool created\n");

	return r;
}

void pmVkDestroyCommandQueue(struct pmVkCommandQueue *queue) {
	vkDestroyCommandPool(queue->device->device, queue->commandPool, NULL);
	free(queue);
}

/*
	COMMAND BUFFERS
*/

VkResult createRenderCommandBuffer(partyRenderer *renderer) {
	VkCommandBufferAllocateInfo bufferAllocateInfo;
	bufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	bufferAllocateInfo.pNext = NULL;
	bufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	bufferAllocateInfo.commandPool = renderer->queue->commandPool;
	bufferAllocateInfo.commandBufferCount = 1;

	VkResult r = vkAllocateCommandBuffers(renderer->device->device, &bufferAllocateInfo, &(renderer->renderCommandBuffer));
	if (r != VK_SUCCESS) {
		log_printf(LL_ERROR, "ERROR: Failed to create render command buffer!");
		return r;
	}

	log_printf(LL_DEBUG, "Successfully created command buffer!\n");

	return r;
}

void destroyRenderCommandBuffer(partyRenderer *renderer) {
	vkFreeCommandBuffers(renderer->device->device, renderer->queue->commandPool, 1, &(renderer->renderCommandBuffer));
}

VkCommandBuffer startStagingCommandBuffer(partyRenderer *renderer) {
	VkCommandBufferAllocateInfo cmdbufAllocInfo;
	cmdbufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdbufAllocInfo.pNext = NULL;
	cmdbufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdbufAllocInfo.commandPool = renderer->memQueue->commandPool;
	cmdbufAllocInfo.commandBufferCount = 1;

	VkCommandBuffer cmdbuf;
	vkAllocateCommandBuffers(renderer->device->device, &cmdbufAllocInfo, &cmdbuf);

	VkCommandBufferBeginInfo cmdbufBeginInfo;
	cmdbufBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdbufBeginInfo.pNext = NULL;
	cmdbufBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	cmdbufBeginInfo.pInheritanceInfo = NULL;

	vkBeginCommandBuffer(cmdbuf, &cmdbufBeginInfo);

	return cmdbuf;
}

void endStagingCommandBuffer(partyRenderer *renderer, VkCommandBuffer cmdbuf) {
	vkEndCommandBuffer(cmdbuf);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdbuf;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = NULL;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = NULL;
	submitInfo.pWaitDstStageMask = NULL;

	vkQueueSubmit(renderer->memQueue->queue, 1, &submitInfo, VK_NULL_HANDLE);
	//vkQueueWaitIdle(renderer->memQueue->queue);	// TODO: maybe make a queue of sorts of the command buffers to free?  lets us load faster

	//vkFreeCommandBuffers(renderer->device->device, renderer->memQueue->commandPool, 1, &cmdbuf);
}
