#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <gfx/vk/vk.h>

VkResult pmVkInitMemoryManager(partyRenderer *renderer, struct pmVkMemoryManager **memManager) {
	struct pmVkMemoryManager *result = malloc(sizeof(struct pmVkMemoryManager));

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
	info.pHeapSizeLimit = NULL;
	info.pVulkanFunctions = NULL;
	info.instance = instance;
	info.vulkanApiVersion = VK_API_VERSION_1_3;
	info.pTypeExternalMemoryHandleTypes = NULL;

	VkResult r = vmaCreateAllocator(&info, &(result->allocator));

	if (r) {
		free(result);
		return r;
	}

	*memManager = result;

	return VK_SUCCESS;
}

void pmVkDestroyMemoryManager(struct pmVkMemoryManager *memoryManager) {
	vmaDestroyAllocator(memoryManager->allocator);

	free(memoryManager);
}
