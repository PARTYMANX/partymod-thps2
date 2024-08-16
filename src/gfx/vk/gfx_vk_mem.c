#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <gfx/vk/vk.h>
#include <log.h>

VkResult pmVkInitMemoryManager(partyRenderer *renderer, struct pmVkMemoryManager **memManager) {
	struct pmVkMemoryManager *result = malloc(sizeof(struct pmVkMemoryManager));

	if (!result) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	*memManager = NULL;

	result->renderer = renderer;

	VmaAllocatorCreateInfo info = {
		.flags = 0,
		.physicalDevice = renderer->device->physicalDevice.device,
		.device = renderer->device->device,
		.preferredLargeHeapBlockSize = 0,
		.pAllocationCallbacks = NULL,
		.pDeviceMemoryCallbacks = NULL,
		.pHeapSizeLimit = NULL,
		.pVulkanFunctions = NULL,
		.instance = instance,
		.vulkanApiVersion = VK_API_VERSION_1_3,
		.pTypeExternalMemoryHandleTypes = NULL,
	};

	VkResult r = vmaCreateAllocator(&info, &(result->allocator));

	if (r) {
		free(result);
		return r;
	}

	*memManager = result;

	log_printf(LL_DEBUG, "Vulkan Memory Allocator initialized!\n");

	return VK_SUCCESS;
}

void pmVkDestroyMemoryManager(struct pmVkMemoryManager *memoryManager) {
	vmaDestroyAllocator(memoryManager->allocator);

	free(memoryManager);
}
