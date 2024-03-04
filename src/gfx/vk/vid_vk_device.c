#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <vulkan/vulkan.h>
#include <gfx/vk/vk.h>

// REFACTOR STATUS: needs validation layers and surface

/*
	DEVICE
*/

const uint32_t deviceExtCount = 3;
const char *deviceExtNames[] = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_MAINTENANCE1_EXTENSION_NAME,
	VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
};

const VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature = {
    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
	.pNext = NULL,
    .dynamicRendering = VK_TRUE,
};

uint8_t getPresentQueueFamily(VkPhysicalDevice phDevice, uint32_t queueFamilyCount, VkQueueFamilyProperties *queueFamilyProperties, VkSurfaceKHR surface, uint32_t *pQueueFamilyIdx) {
	*pQueueFamilyIdx = 0;
	for (VkQueueFamilyProperties *i = queueFamilyProperties; i < queueFamilyProperties + queueFamilyCount; i++) {
		VkBool32 presentSupported = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(phDevice, *pQueueFamilyIdx, surface, &presentSupported);

		if (i->queueCount > 0 && presentSupported) {
			return 1;
		}

		(*pQueueFamilyIdx)++;
	}

	return 0;
}

uint8_t getGraphicsQueueFamily(VkPhysicalDevice phDevice, uint32_t queueFamilyCount, VkQueueFamilyProperties *queueFamilyProperties, uint32_t *pQueueFamilyIdx) {
	*pQueueFamilyIdx = 0;
	for (VkQueueFamilyProperties *i = queueFamilyProperties; i < queueFamilyProperties + queueFamilyCount; i++) {
		if (i->queueCount > 0 && i->queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			return 1;
		}

		(*pQueueFamilyIdx)++;
	}

	return 0;
}

uint8_t getQueueFamilies(VkPhysicalDevice phDevice, uint32_t *pQueueFamilyIdxs, VkSurfaceKHR surface) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(phDevice, &queueFamilyCount, NULL);

#ifdef DEBUG
	printf("Queue Family Count: %d\n", queueFamilyCount);
#endif

	VkQueueFamilyProperties *queueFamilyProperties = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(phDevice, &queueFamilyCount, queueFamilyProperties);

	uint8_t result = 1;
	result = result && getGraphicsQueueFamily(phDevice, queueFamilyCount, queueFamilyProperties, pQueueFamilyIdxs + QUEUE_FAMILY_GRAPHICS);
	result = result && getPresentQueueFamily(phDevice, queueFamilyCount, queueFamilyProperties, surface, pQueueFamilyIdxs + QUEUE_FAMILY_PRESENT);

	free(queueFamilyProperties);

	return result;
}

uint8_t deviceSupportsExt(VkPhysicalDevice phDevice, uint32_t extCount, char **extNames) {
	VkResult r;

	uint32_t supportedExtCount = 0;
	r = vkEnumerateDeviceExtensionProperties(phDevice, NULL, &supportedExtCount, NULL);
	if (r) {
		printf("Error enumerating devices\n");
		return 0;
	}

	VkExtensionProperties *supportedExts = malloc(sizeof(VkExtensionProperties) * supportedExtCount);
	r = vkEnumerateDeviceExtensionProperties(phDevice, NULL, &supportedExtCount, supportedExts);
	if (r) {
		printf("Error enumerating devices\n");
		return 0;
	}

	uint32_t supportCount = 0;
	for (char **i = extNames; i < extNames + extCount; i++) {
		printf("TESTING SUPPORT FOR: %s\n", *i);
		for (VkExtensionProperties *j = supportedExts; j < supportedExts + supportedExtCount; j++) {
			if (strcmp(*i, j->extensionName) == 0) {
				printf("%s supported!\n", j->extensionName);
				supportCount++;
				break;
			}
		}
	}

	free(supportedExts);

	return supportCount == extCount;
}

uint8_t deviceSupportsSwapchain(VkPhysicalDevice phDevice, VkSurfaceKHR surface) {
	VkResult r;

	uint32_t formatCount = 0;
	uint32_t presentModeCount = 0;

	r = vkGetPhysicalDeviceSurfaceFormatsKHR(phDevice, surface, &formatCount, NULL);
	if (r) {
		printf("Error getting device surface formats\n");
		return 0;
	}

	r = vkGetPhysicalDeviceSurfacePresentModesKHR(phDevice, surface, &presentModeCount, NULL);
	if (r) {
		printf("Error getting device present modes\n");
		return 0;
	}

	return formatCount > 0 && presentModeCount > 0;
}

VkResult getPhysicalDevice(struct rbVkWindow *window, struct rbVkPhysicalDevice *physicalDevice) {
	VkResult r;

	VkSurfaceKHR surface = getWindowSurface(window);

	struct rbVkPhysicalDevice result;
	result.device = VK_NULL_HANDLE;
	for (uint32_t *i = result.queueFamilyIdxs; i < result.queueFamilyIdxs + QUEUE_FAMILY_COUNT; i++) 
		*i = 0;

	uint8_t isDiscrete = 0;

	uint32_t phDeviceCount = 0;
	r = vkEnumeratePhysicalDevices(instance, &phDeviceCount, NULL);

	if (r) {
		printf("Failed to enumerate devices\n");
		return r;
	}

#ifdef DEBUG
	printf("Physical Device Count: %d\n", phDeviceCount);
#endif

	if (phDeviceCount == 0) {
		printf("ERROR: Cannot find GPU supporting Vulkan!\n");
		return VK_SUCCESS;
	}

	// pick the optimal device
	VkPhysicalDevice *phDevices = malloc(sizeof(VkPhysicalDevice) * phDeviceCount);
	r = vkEnumeratePhysicalDevices(instance, &phDeviceCount, phDevices);

	if (r) {
		printf("Failed to enumerate devices\n");
		return r;
	}

	for (VkPhysicalDevice *i = phDevices; i < phDevices + phDeviceCount; i++) {
		// TODO: better heuristics for device picking, ability to choose lower power devices
		VkPhysicalDeviceProperties phDeviceProperties;
		vkGetPhysicalDeviceProperties(*i, &phDeviceProperties);

		VkPhysicalDeviceFeatures phDeviceFeatures;
		vkGetPhysicalDeviceFeatures(*i, &phDeviceFeatures);

		uint32_t queueFamilyIdxs[QUEUE_FAMILY_COUNT];
		if (!isDiscrete && 
			phDeviceProperties.apiVersion >= VK_MAKE_API_VERSION(0, 1, 3, 0) && 
			getQueueFamilies(*i, result.queueFamilyIdxs, surface) && 
			deviceSupportsExt(*i, deviceExtCount, deviceExtNames) && 
			deviceSupportsSwapchain(*i, surface) && phDeviceFeatures.samplerAnisotropy) {	// FIXME: error checking
			result.device = *i;

			printf("DEVICE: %s\n", phDeviceProperties.deviceName);

			isDiscrete = phDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		}

		if (isDiscrete)
			break;
	}

	free(phDevices);

	if (!result.device) {
		printf("ERROR: Cannot find suitable GPU supporting Vulkan!\n");
		return VK_SUCCESS;
	}

	*physicalDevice = result;

	return VK_SUCCESS;
}

VkResult createVulkanDevice(struct rbVkWindow *window, struct rbVkDevice **device) {
	struct rbVkDevice *result;
	VkResult r;

	*device = NULL;

	result = malloc(sizeof(struct rbVkDevice));

	if (!result) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	r = getPhysicalDevice(window, &(result->physicalDevice));
	if (r) {
		printf("Failed to get physical device!\n");
		free(result);
		return r;
	}

	result->window = window;

	float priority = 1.0f;

	uint32_t deviceQueueCount = 0;
	VkDeviceQueueCreateInfo deviceQueueInfo[QUEUE_FAMILY_COUNT];
	for (int i = 0; i < QUEUE_FAMILY_COUNT; i++) {
		// check if it's a unique queue family
		uint8_t isUnique = 1;

		for (int j = 0; j < i; j++) {
			if (result->physicalDevice.queueFamilyIdxs[i] == deviceQueueInfo[j].queueFamilyIndex)
				isUnique = 0;
		}

		// if the queue family is unique, create the queue
		if (isUnique) {
			deviceQueueInfo[deviceQueueCount].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			deviceQueueInfo[deviceQueueCount].pNext = NULL;
			deviceQueueInfo[deviceQueueCount].flags = 0;
			deviceQueueInfo[deviceQueueCount].queueFamilyIndex = result->physicalDevice.queueFamilyIdxs[i];
			deviceQueueInfo[deviceQueueCount].queueCount = 1;
			deviceQueueInfo[deviceQueueCount].pQueuePriorities = &priority;

			deviceQueueCount++;
		}
	}

	VkPhysicalDeviceFeatures phDeviceFeatures;
	memset(&phDeviceFeatures, 0, sizeof(VkPhysicalDeviceFeatures));

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = &dynamic_rendering_feature;
	deviceInfo.flags = 0;
	deviceInfo.queueCreateInfoCount = deviceQueueCount;
	deviceInfo.pQueueCreateInfos = deviceQueueInfo;
	deviceInfo.pEnabledFeatures = &phDeviceFeatures;
	deviceInfo.enabledExtensionCount = deviceExtCount;
	deviceInfo.ppEnabledExtensionNames = deviceExtNames;
	deviceInfo.enabledLayerCount = validationLayerCount;
	deviceInfo.ppEnabledLayerNames = validationLayerNames;

	r = vkCreateDevice(result->physicalDevice.device, &deviceInfo, NULL, &(result->device));
	if (r) {
		printf("ERROR: Failed to create Vulkan device!\n");
		free(result);
		return r;
	}

	*device = result;

	return VK_SUCCESS;
}

void rbVkDestroyDevice(struct rbVkDevice *device) {
	vkDestroyDevice(device->device, 0);

	free(device);
}