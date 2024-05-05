#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <gfx/vk/vk.h>
#include <log.h>

// REFACTOR STATUS: needs window surface
// REFACTOR STATUS: needs texture refactor

/*
	SWAPCHAIN
*/

VkResult getSwapchainFormat(struct pmVkDevice *device, VkSurfaceFormatKHR *fmt) {
	VkSurfaceKHR surface = getWindowSurface(device->window);

	VkResult r;

	VkSurfaceFormatKHR preferredFormat;
	preferredFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	//preferredFormat.format = VK_FORMAT_B8G8R8A8_SRGB;
	preferredFormat.format = VK_FORMAT_B8G8R8A8_UNORM;

	uint32_t formatCount = 0;
	r = vkGetPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice.device, surface, &formatCount, NULL);
	if (r) {
		return r;
	}

	VkSurfaceFormatKHR *formats = malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
	if (!formats) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	r = vkGetPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice.device, surface, &formatCount, formats);
	if (r) {
		free(formats);
		return r;
	}

	// if the device will give us anything, return preferred
	if (formatCount == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
		free(formats);
		*fmt = preferredFormat;
		return VK_SUCCESS;
	}

	// otherwise, find our preferred format and when we do, return it
	for (VkSurfaceFormatKHR *i = formats; i < formats + formatCount; i++) {
		if (i->colorSpace == preferredFormat.colorSpace && i->format == preferredFormat.format) {
			free(formats);
			*fmt = preferredFormat;
			return VK_SUCCESS;
		}
	}

	// if we cannot find our preferred format, just return the first available
	preferredFormat = formats[0];

	free(formats);
	*fmt = preferredFormat;
	return VK_SUCCESS;
}

VkResult getSwapchainPresentMode(struct pmVkDevice *device, VkPresentModeKHR *mode) {
	// TODO: think of some way for the user to select their preferred mode (for example, keep a ranked lut of present modes)
	VkSurfaceKHR surface = getWindowSurface(device->window);
	VkResult r;
	
	VkPresentModeKHR result = VK_PRESENT_MODE_FIFO_KHR;	// fifo is required, thus is our fallback

	uint32_t presentModeCount = 0;
	r = vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice.device, surface, &presentModeCount, NULL);
	if (r) {
		return r;
	}

	VkPresentModeKHR *presentModes = malloc(sizeof(VkPresentModeKHR) * presentModeCount);
	r = vkGetPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice.device, surface, &presentModeCount, presentModes);
	if (r) {
		free(presentModes);
		return r;
	}

	for (VkPresentModeKHR *i = presentModes; i < presentModes + presentModeCount; i++) {
		if (*i == VK_PRESENT_MODE_MAILBOX_KHR)
			result = *i;
		else if (*i == VK_PRESENT_MODE_IMMEDIATE_KHR && result != VK_PRESENT_MODE_MAILBOX_KHR)
			result = *i;
	}

	free(presentModes);

	*mode = result;

	return VK_SUCCESS;
}

VkResult pmVkCreateSwapchain(struct pmVkDevice *device, struct pmVkSwapchain **swapchain) {
	*swapchain = NULL;

	VkSurfaceKHR surface = getWindowSurface(device->window);

	struct pmVkSwapchain *result = malloc(sizeof(struct pmVkSwapchain));
	result->device = device;

	VkResult r;

	VkSurfaceCapabilitiesKHR capabilities;
	r = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->physicalDevice.device, surface, &capabilities);
	if (r) {
		log_printf(LL_ERROR, "Failed to get physical device surface capabilities! %d\n", r);
		free(result);
		return r;
	}

	VkExtent2D extent;

	if (capabilities.currentExtent.width != UINT32_MAX)
		extent = capabilities.currentExtent;
	else {
		pmVkGetDrawableSize(device->window, &(extent.width), &(extent.height));

		if (extent.width < capabilities.minImageExtent.width)
			extent.width = capabilities.minImageExtent.width;
		else if (extent.width > capabilities.maxImageExtent.width)
			extent.width = capabilities.maxImageExtent.width;

		if (extent.height < capabilities.minImageExtent.height)
			extent.height = capabilities.minImageExtent.height;
		else if (extent.height > capabilities.maxImageExtent.height)
			extent.height = capabilities.maxImageExtent.height;
	}

	// TODO: actually specify the number of images
	uint32_t imageCount = capabilities.minImageCount + 1;

	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		imageCount = capabilities.maxImageCount;

	VkSurfaceFormatKHR surfaceFormat;
	r = getSwapchainFormat(device, &surfaceFormat);
	if (r) {
		log_printf(LL_ERROR, "Failed to get swapchain format!\n");
		free(result);
		return r;
	}

	VkPresentModeKHR presentMode;
	r = getSwapchainPresentMode(device, &presentMode);
	if (r) {
		log_printf(LL_ERROR, "Failed to get swapchain present mode!\n");
		free(result);
		return r;
	}

	VkSwapchainCreateInfoKHR swapchainInfo;
	memset(&swapchainInfo, 0, sizeof(VkSwapchainCreateInfoKHR));

	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.pNext = NULL;
	swapchainInfo.surface = surface;
	swapchainInfo.minImageCount = imageCount;
	swapchainInfo.imageFormat = surfaceFormat.format;
	swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_GRAPHICS] != device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_PRESENT]) {
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainInfo.queueFamilyIndexCount = 2;
		swapchainInfo.pQueueFamilyIndices = device->physicalDevice.queueFamilyIdxs;
	} else {
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = NULL;
	}
	swapchainInfo.preTransform = capabilities.currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	r = vkCreateSwapchainKHR(device->device, &swapchainInfo, NULL, &(result->swapchain));
	if (r) {
		log_printf(LL_ERROR, "Failed to create swapchain!\n");
		free(result);
		return r;
	}

	r = vkGetSwapchainImagesKHR(device->device, result->swapchain, &(result->imageCount), NULL);
	if (r) {
		log_printf(LL_ERROR, "Failed to get swapchain image count!\n");
		free(result);
		return r;
	}

	result->imageFormat = surfaceFormat.format;
	result->extent = extent;
	result->images = malloc(sizeof(VkImage) * result->imageCount);
	r = vkGetSwapchainImagesKHR(device->device, result->swapchain, &(result->imageCount), result->images);
	if (r) {
		log_printf(LL_ERROR, "Failed to get swapchain images!\n");
		free(result->images);
		free(result);
		return r;
	}

	result->imageViews = malloc(sizeof(VkImageView) * result->imageCount);

	if (!result->images) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	for (uint32_t i = 0; i < result->imageCount; i++) {
		VkImageViewCreateInfo imageViewInfo;
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.pNext = NULL;
		imageViewInfo.flags = 0;
		imageViewInfo.image = result->images[i];
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = result->imageFormat;
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		r = vkCreateImageView(device->device, &imageViewInfo, NULL, result->imageViews + i);
		if (r) {
			log_printf(LL_ERROR, "Failed to create swapchain image view %d\n", i);
			free(result->imageViews);
			free(result->images);
			free(result);
			return r;
		}
	}

	// setup semaphores
	result->imageIdx = 0;

	VkSemaphoreCreateInfo semaphoreInfo;
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreInfo.pNext = NULL;
	semaphoreInfo.flags = 0;

	r = vkCreateSemaphore(device->device, &semaphoreInfo, NULL, &(result->imageReadySemaphore));
	if (r) {
		log_printf(LL_ERROR, "Failed to create image ready semaphore!\n");
		free(result->imageViews);
		free(result->images);
		free(result);
		return r;
	}

	r = vkCreateSemaphore(device->device, &semaphoreInfo, NULL, &(result->imageFinishedSemaphore));
	if (r) {
		log_printf(LL_ERROR, "Failed to create image finished semaphore\n");
		free(result->imageViews);
		free(result->images);
		free(result);
		return r;
	}

	// setup fences

	VkFenceCreateInfo fenceInfo;
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = NULL;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	r = vkCreateFence(device->device, &fenceInfo, NULL, &(result->fence));
	if (r) {
		log_printf(LL_ERROR, "Failed to create swapchain fence\n");
		free(result->imageViews);
		free(result->images);
		free(result);
		return r;
	}

	result->fenceUsed = 0;

	*swapchain = result;

	log_printf(LL_DEBUG, "Successfully created swapchain!\n");

	return VK_SUCCESS;
}

void pmVkDestroySwapchain(struct pmVkSwapchain *swapchain) {
	vkDestroyFence(swapchain->device->device, swapchain->fence, NULL);
	vkDestroySemaphore(swapchain->device->device, swapchain->imageFinishedSemaphore, NULL);
	vkDestroySemaphore(swapchain->device->device, swapchain->imageReadySemaphore, NULL);

	for (VkImageView *i = swapchain->imageViews; i < swapchain->imageViews + swapchain->imageCount; i++) {
		vkDestroyImageView(swapchain->device->device, i, NULL);
	}

	vkDestroySwapchainKHR(swapchain->device->device, swapchain->swapchain, NULL);
	free(swapchain->images);
	free(swapchain->imageViews);
	free(swapchain);
}