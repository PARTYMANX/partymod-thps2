#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_vulkan.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#define VK_USE_PLATFORM_WIN32_KHR 1
#include <vulkan/vulkan.h>

#include <gfx/vk/gfx_vk.h>
#include <gfx/vk/vk.h>

struct rbVkWindow {
	HWND hwnd;
	VkSurfaceKHR surface;
};

const uint32_t windowExtCount = 2;
const char * const windowExtNames[] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME
};

/*
	WINDOW
*/

VkResult rbVkCreateWindow(void *windowHandle, struct rbVkWindow **window) {
	struct rbVkWindow *result = malloc(sizeof(struct rbVkWindow));

	if (!result) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	*window = NULL;

	result->hwnd = *(HWND *)windowHandle;

	PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR) vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");

	if (!vkCreateWin32SurfaceKHR) {
		printf("failed to create window surface!\n");
		free(result);
		return VK_ERROR_UNKNOWN;
	}

	VkWin32SurfaceCreateInfoKHR createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.hinstance = GetModuleHandle(NULL);
	createInfo.hwnd = *(HWND *)windowHandle;

	VkResult r = vkCreateWin32SurfaceKHR(instance, &createInfo, NULL, &(result->surface));
	if (r) {
		printf("failed to create window surface!\n");
		free(result);
		return r;
	}

	*window = result;

	return VK_SUCCESS;
}

void rbVkDestroyWindow(struct rbVkWindow *window) {
	vkDestroySurfaceKHR(instance, window->surface, NULL);
	
	free(window);
}

VkSurfaceKHR getWindowSurface(struct rbVkWindow *window) {
	return window->surface;
}

void rbVkGetDrawableSize(struct rbVkWindow *window, int *pWidth, int *pHeight) {
	RECT rect;

	GetClientRect(window->hwnd, &rect);

	*pWidth = rect.right;
	*pHeight = rect.bottom;
}

void appendWindowExtensions(uint32_t *dstExtCount, char ***dstExtNames) {
	// append
	uint32_t oldExtCount = *dstExtCount;
	*dstExtCount = *dstExtCount + windowExtCount;

	*dstExtNames = realloc(*dstExtNames, sizeof(char *) * *dstExtCount);
	for (int i = 0; i < windowExtCount; i++) {
		(*dstExtNames)[oldExtCount + i] = windowExtNames[i];
	}
}
