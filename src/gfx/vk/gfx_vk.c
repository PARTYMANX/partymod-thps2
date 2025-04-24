#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <global.h>
#include <log.h>

#include <volk.h>
//#include <vulkan/vulkan.h>

#include <gfx/vk/gfx_vk.h>
#include <gfx/vk/vk.h>

/*
	VALIDATION LAYER
*/

VkDebugUtilsMessengerEXT debugMessenger;

PFN_vkCreateDebugUtilsMessengerEXT vkpfn_vkCreateDebugUtilsMessengerEXT = NULL;
PFN_vkDestroyDebugUtilsMessengerEXT vkpfn_vkDestroyDebugUtilsMessengerEXT = NULL;

const uint32_t validationLayerCount = 1;
const char * const validationLayerNames[] = {
	"VK_LAYER_KHRONOS_validation"
};

const uint32_t validationExtCount = 1;
const char * const validationExtNames[] = {
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	log_printf(LL_INFO, "VALIDATION LAYER: %s\n", pCallbackData->pMessage);

	return VK_FALSE;
}

void createDebugMessenger() {
	VkDebugUtilsMessengerCreateInfoEXT messengerInfo = {
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.pNext = NULL,
		.flags = 0,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debugMessageCallback,
		.pUserData = NULL,
	};

	vkpfn_vkCreateDebugUtilsMessengerEXT(instance, &messengerInfo, NULL, &debugMessenger);
}

/*
	REQUIRED EXTENSIONS
*/

const uint32_t reqExtensionCount = 0;
const char * const *reqExtensionNames;

/*
	INSTANCE
*/

// TODO: maybe return an array of only the supported layers

uint8_t checkLayerSupport(char **layers, uint32_t layerCount) {
	VkResult r;

	uint32_t supportedLayerCount = 0;
	r = vkEnumerateInstanceLayerProperties(&supportedLayerCount, NULL);
	if (r) {
		log_printf(LL_ERROR, "Failed to enumerate instance layer properties\n");
		return 0;
	}

	VkLayerProperties *supportedLayers = malloc(sizeof(VkLayerProperties) * supportedLayerCount);
	r = vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers);
	if (r) {
		log_printf(LL_ERROR, "Failed to enumerate instance layer properties\n");
		return 0;
	}

	for (char **i = layers; i < layers + layerCount; i++) {
		uint8_t found = 0;

		for (VkLayerProperties *j = supportedLayers; j < supportedLayers + supportedLayerCount; j++) {
			if (strcmp(*i, j->layerName) == 0) {
				found = 1;
				break;
			}
		}

		if (!found) {
			log_printf(LL_ERROR, "Layer not supported: %s\n", *i);
			return 0;
		}
	}

	free(supportedLayers);

	return 1;
}

void appendExtensions(uint32_t *dstExtCount, char ***dstExtNames, uint32_t srcExtCount, char **srcExtNames) {
	uint32_t oldExtCount = *dstExtCount;
	*dstExtCount = *dstExtCount + srcExtCount;

	*dstExtNames = realloc(*dstExtNames, sizeof(char *) * *dstExtCount);
	for (int i = 0; i < srcExtCount; i++) {
		(*dstExtNames)[oldExtCount + i] = srcExtNames[i];
	}
}

VkResult initInstance() {
	VkResult result = volkInitialize();
	if (result != VK_SUCCESS) {
		// error
		log_printf(LL_ERROR, "ERROR: Failed to load Vulkan!\n");
		return result;
	}

	VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = NULL,
		.pApplicationName = "PARTYMOD for THPS2",
		.applicationVersion = VK_MAKE_VERSION(VERSION_NUMBER_MAJOR, VERSION_NUMBER_MINOR, VERSION_NUMBER_PATCH),
		.pEngineName = "M3D",
		.engineVersion = VK_MAKE_VERSION(VERSION_NUMBER_MAJOR, VERSION_NUMBER_MINOR, VERSION_NUMBER_PATCH),
		.apiVersion = VK_API_VERSION_1_3,
	};

	// get extension count
	uint32_t extCount = 0;
	char **extNames = NULL;

	appendWindowExtensions(&extCount, &extNames);

	VkInstanceCreateInfo instanceInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = &appInfo,
	};

#ifndef VK_VALIDATION
	instanceInfo.enabledExtensionCount = extCount;
	instanceInfo.ppEnabledExtensionNames = extNames;
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = NULL;
#else
	if (checkLayerSupport(validationLayerNames, validationLayerCount)) {
		instanceInfo.enabledLayerCount = validationLayerCount;
		instanceInfo.ppEnabledLayerNames = validationLayerNames;

		appendExtensions(&extCount, &extNames, validationExtCount, validationExtNames);
	} else {
		log_printf(LL_ERROR, "Failed to get validation layer!\n");

		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = NULL;
	}
#endif

	appendExtensions(&extCount, &extNames, reqExtensionCount, reqExtensionNames);

	log_printf(LL_INFO, "Creating Vulkan instance with %u extensions:\n", extCount);
	for (int i = 0; i < extCount; i++) {
		log_printf(LL_INFO, "%s\n", extNames[i]);
	}

	instanceInfo.enabledExtensionCount = extCount;
	instanceInfo.ppEnabledExtensionNames = extNames;

	result = vkCreateInstance(&instanceInfo, NULL, &instance);

	if (result != VK_SUCCESS) {
		// error
		log_printf(LL_ERROR, "ERROR: Failed to create Vulkan instance!\n");
		return result;
	}

#ifdef VK_VALIDATION
	vkpfn_vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	vkpfn_vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (!vkpfn_vkCreateDebugUtilsMessengerEXT) {
		log_printf(LL_ERROR, "ERROR: Failed to load vkCreateDebugUtilsMessengerEXT function!\n");
		//return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	if (!vkpfn_vkDestroyDebugUtilsMessengerEXT) {
		log_printf(LL_ERROR, "ERROR: Failed to load vkDestroyDebugUtilsMessengerEXT function!\n");
		//return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	if (vkpfn_vkCreateDebugUtilsMessengerEXT)
		createDebugMessenger();
#endif

	volkLoadInstance(instance);

	return VK_SUCCESS;
}

void destroyInstance() {
#ifdef VK_VALIDATION
	if (vkpfn_vkDestroyDebugUtilsMessengerEXT)
		vkpfn_vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
#endif

	vkDestroyInstance(instance, 0);
}

/*
	POLY BUFFER
*/

VkResult createPolyBuffer(partyRenderer *renderer) {
	polyBuffer result;

	result.vertexCapacity = 32768;
	result.vertices = malloc(sizeof(renderVertex) * result.vertexCapacity);
	result.currentVertex = 0;

	createBuffer(renderer, sizeof(renderVertex) * result.vertexCapacity, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, &(result.buffer));

	renderer->polyBuffer = result;

	return VK_SUCCESS;
}

void appendPolyBuffer(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count) {
	for (int i = 0; i < vertex_count; i++) {
		renderer->polyBuffer.vertices[renderer->polyBuffer.currentVertex + i] = vertices[i];
	}

	if (vertices[0].texture > 0) {
		int16_t targetTexture = vertices[0].texture;
		if (targetTexture >= renderer->textureManager.capacity) {
			targetTexture -= renderer->textureManager.capacity;
		}

		if (!renderer->textureManager.occupied[targetTexture]) {
			log_printf(LL_ERROR, "Referencing freed texture %d!!!\n", targetTexture);
		}
	}

	renderer->polyBuffer.currentVertex += vertex_count;
}

void resetPolyBuffer(partyRenderer *renderer) {
	renderer->polyBuffer.currentVertex = 0;
}

void updatePolyBuffer(partyRenderer *renderer) {
	//printf("Updating poly buffer with %d vertices\n", renderer->polyBuffer.currentVertex);

	renderVertex *buffer = mapBuffer(renderer, &(renderer->polyBuffer.buffer));

	memcpy(buffer, renderer->polyBuffer.vertices, sizeof(renderVertex) * renderer->polyBuffer.currentVertex);

	unmapBuffer(renderer, &(renderer->polyBuffer.buffer));
}

/*
	TEXTURE MANAGER
*/

void createTextureManager(partyRenderer *renderer) {
	textureManager result;

	result.capacity = 1024;
	result.count = 0;
	result.samplers[0] = createSampler(renderer, VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT);
	result.samplers[1] = createSampler(renderer, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT);
	result.samplers[2] = createSampler(renderer, VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	result.samplers[3] = createSampler(renderer, VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
	result.images = malloc(sizeof(pmVkImage) * result.capacity);
	result.occupied = calloc(result.capacity, sizeof(uint8_t));

	renderer->textureManager = result;
}

uint32_t createTextureEntry(partyRenderer *renderer, uint32_t width, uint32_t height) {
	pmVkImage result;
	createTexture(renderer, width, height, &result);

	int i = 0;
	while(renderer->textureManager.occupied[i] && i < renderer->textureManager.capacity) {
		i++;
	}

	renderer->textureManager.occupied[i] = 1;
	renderer->textureManager.images[i] = result;

	renderer->textureManager.count++;

	return i + 1;
}

void updateTextureEntry(partyRenderer *renderer, uint32_t idx, uint32_t width, uint32_t height, void *data) {
	updateTexture(renderer, renderer->textureManager.images + idx - 1, width, height, data);
}

void destroyTextureEntry(partyRenderer *renderer, uint32_t idx) {
	//printf("deleting texture %d\n", idx);

	pendingImageDelete del;
	del.img = renderer->textureManager.images[idx - 1];
	// i'll be honest, this is a bit of a hack for a validation error i was seeing when switching to certain screens.  
	// sometimes we were deleting images too soon that i'm pretty sure weren't in use but we were getting validation errors anyway.  whoops!
	del.remainingFrames = 2;	

	sb_push_back(renderer->pendingImageDeletes, &del);
	renderer->textureManager.occupied[idx - 1] = 0;
	renderer->textureManager.count--;

	if (renderer->pendingImageDeletes->count > (renderer->textureManager.capacity / 2)) {
		// emergency flush last half of pending image deletes.  something went horribly wrong 
		// this can actually happen if images don't get cleared during a long-held soft reset

		log_printf(LL_WARN, "DOING EMERGENCY TEXTURE DELETION FLUSH\n");
		if (renderer->pendingImageWrites->count > 0) {
			vkQueueWaitIdle(renderer->memQueue->queue);

			for (int i = 0; i < renderer->pendingImageWrites->count; i++) {
				pendingImageWrite *imgWrite = ((pendingImageWrite *)renderer->pendingImageWrites->data) + i;
				vkFreeCommandBuffers(renderer->device->device, renderer->memQueue->commandPool, 1, &imgWrite->cmdbuf);
				destroyBuffer(renderer, &imgWrite->transferbuf);
			}
			renderer->pendingImageWrites->count = 0;
		}

		while (renderer->pendingImageDeletes->count > 512) {
			pendingImageDelete del;
			sb_pop_front(renderer->pendingImageDeletes, &del);
			destroyTexture(renderer, del.img);
		}
		
	}

	/*if (renderer->pendingImageWrites->count > 0) {
		vkQueueWaitIdle(renderer->memQueue->queue);
	}

	destroyTexture(renderer, renderer->textureManager.images[idx - 1]);*/
}

int getTextureCount(partyRenderer *renderer) {
	return renderer->textureManager.count;
}

// DANGER: this will cause dangling pointers!!!
void clearAllTextures(partyRenderer *renderer) {
	// flush writes
	if (renderer->pendingImageWrites->count > 0) {
		vkQueueWaitIdle(renderer->memQueue->queue);

		for (int i = 0; i < renderer->pendingImageWrites->count; i++) {
			pendingImageWrite *imgWrite = ((pendingImageWrite *)renderer->pendingImageWrites->data) + i;
			vkFreeCommandBuffers(renderer->device->device, renderer->memQueue->commandPool, 1, &imgWrite->cmdbuf);
			destroyBuffer(renderer, &imgWrite->transferbuf);
		}
		renderer->pendingImageWrites->count = 0;
	}

	renderer->pendingImageDeletes->count = 0;

	for (int i = 0; i < renderer->textureManager.capacity; i++) {
		if (renderer->textureManager.occupied[i]) {
			destroyTexture(renderer, renderer->textureManager.images[i]);
			renderer->textureManager.occupied[i] = 0;
		}
	}

	renderer->textureManager.count = 0;
}

void writeTextureDescriptors(partyRenderer *renderer) {
	//printf("writing %d descriptors...\n", renderer->textureManager.count);
	int i = 0;
	int count = 0;
	while(count < renderer->textureManager.count) {
		if (renderer->textureManager.occupied[i]) {
			write_descriptor_image_array(renderer->descriptorAllocator, 0, i, renderer->textureManager.samplers[renderer->textureFilter], renderer->textureManager.images[i].imageView, VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
			write_descriptor_image_array(renderer->descriptorAllocator, 0, i + renderer->textureManager.capacity, renderer->textureManager.samplers[renderer->textureFilter + 2], renderer->textureManager.images[i].imageView, VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
			count++;
		}

		i++;
	}
}

void flushTextureDeletes(partyRenderer *renderer) {
	for (int i = 0; i < renderer->pendingImageDeletes->count; i++){
		pendingImageDelete del;
		sb_pop_front(renderer->pendingImageDeletes, &del);
		if (del.remainingFrames > 0) {
			del.remainingFrames--;
			sb_push_back(renderer->pendingImageDeletes, &del);
			//printf("delaying delete for %d frames\n", del.remainingFrames);
		} else {
			//printf("TEST\n");
			destroyTexture(renderer, del.img);
		}
	}
}

/*
	SCALER
*/

VkResult createScalerVertexBuffer(partyRenderer *renderer) {
	pmVkBuffer result;

	createBuffer(renderer, sizeof(scalerVertex) * 6, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, &result);

	scalerVertex vertices[6] = {
		{ -1.0, 1.0, 0.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ -1.0, -1.0, 0.0, 0.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 1.0, -1.0, 1.0, 0.0 },
		{ -1.0, -1.0, 0.0, 0.0 },
	};
	scalerVertex *buffer = mapBuffer(renderer, &result);

	memcpy(buffer, vertices, sizeof(scalerVertex) * 6);

	unmapBuffer(renderer, &result);

	renderer->scalerBuffer = result;

	return VK_SUCCESS;
}

VkResult createScalerImageInfoBuffer(partyRenderer *renderer) {
	pmVkBuffer result;

	createBuffer(renderer, sizeof(imageInfo), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, &result);

	imageInfo info = { renderer->renderImage.width, renderer->renderImage.height };
	imageInfo *buffer = mapBuffer(renderer, &result);

	memcpy(buffer, &info, sizeof(imageInfo));

	unmapBuffer(renderer, &result);

	renderer->renderImageInfoBuffer = result;

	return VK_SUCCESS;
}

/*
	RENDERER
*/

uint8_t CreateVKRenderer(void *windowHandle, partyRenderer **renderer) {
	VkResult r;

	r = initInstance();
	if (r) {
		goto error;
	}

	partyRenderer *result = malloc(sizeof(partyRenderer));
	if (!result) {
		goto error;
	}

	//result->parent.driver = &pmVkDriver;

	r = pmVkCreateWindow(windowHandle, &(result->window));
	if (r) {
		fatalError("Failed to create Vulkan surface!");
		goto error_free;
	}

	r = createVulkanDevice(result->window, &(result->device));
	if (r) {
		fatalError("Failed to create Vulkan 1.3 device!  Make sure that you have the latest graphics drivers installed.");
		goto error_free;
	}

	r = pmVkCreateSwapchain(result->device, &(result->swapchain));
	if (r) {
		fatalError("Failed to create Vulkan swapchain!");
		goto error_free;
	}

	r = pmVkCreateCommandQueue(result->device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, &(result->queue));
	if (r) {
		goto error_free;
	}

	r = pmVkCreateCommandQueue(result->device, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, &(result->memQueue));
	if (r) {
		goto error_free;
	}

	r = pmVkInitMemoryManager(result, &(result->memoryManager));
	if (r) {
		goto error_free;
	}

	r = createRenderCommandBuffer(result);
	if (r) {
		goto error_free;
	}

	struct PoolSizeRatio descriptor_ratios[4] = {
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3 }, 
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 }, 
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 }, 
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4 }, 
	};
	result->descriptorAllocator = init_descriptors(result, 1000, descriptor_ratios, 4);

	log_printf(LL_DEBUG, "Creating render targets...\n");

	// create default render targets at 640x480, 4:3
	createRenderTargets(result, 640, 480, GFX_COLOR_FORMAT, GFX_DEPTH_FORMAT);
	result->aspectRatio = 4.0f / 3.0f;
	result->renderWidth = 640;
	result->renderHeight = 480;
	result->textureFilter = 0;

	log_printf(LL_DEBUG, "Successful!\n");
	
	log_printf(LL_DEBUG, "Creating pipelines...\n");

	r = createRenderPipelines(result);
	if (r) {
		goto error_free;
	}

	log_printf(LL_DEBUG, "Created render pipelines\n");

	r = createScalerPipeline(result);
	if (r) {
		goto error_free;
	}

	log_printf(LL_DEBUG, "Scaler pipeline created!\n");

	r = createPolyBuffer(result);
	if (r) {
		goto error_free;
	}

	createTextureManager(result);

	result->pendingImageWrites = sb_alloc(sizeof(pendingImageWrite), 64);
	result->pendingImageDeletes = sb_alloc(sizeof(pendingImageDelete), 64);

	r = createScalerVertexBuffer(result);
	if (r) {
		goto error_free;
	}

	r = createScalerImageInfoBuffer(result);
	if (r) {
		goto error_free;
	}

	*renderer = result;

	return 1;

error_free:
	free(result);
error:
	log_printf(LL_ERROR, "failed to create renderer\n");
	fatalError("Failed to create Vulkan renderer!");
	// todo: translate error
	return 0;
}

void pmVkDestroyRenderer(partyRenderer *renderer) {
	//pmVkDestroyMemoryManager(renderer->memoryManager);
	destroyRenderCommandBuffer(renderer);
	pmVkDestroyCommandQueue(renderer->memQueue);
	pmVkDestroyCommandQueue(renderer->queue);
	pmVkDestroySwapchain(renderer->swapchain);
	pmVkDestroyDevice(renderer->device);
	pmVkDestroyWindow(renderer->window);

	free(renderer);

	destroyInstance();
}

uint8_t pmVkWaitIdle(partyRenderer *renderer) {
	VkResult result = vkDeviceWaitIdle(renderer->device->device);
	
	if (result != VK_SUCCESS) {
		// TODO: better errors
		return 0;
	}

	return 1;
}

void updateRenderer(partyRenderer *renderer) {
	pmVkWaitIdle(renderer);

	pmVkDestroySwapchain(renderer->swapchain);
	pmVkCreateSwapchain(renderer->device, &(renderer->swapchain));	// FIXME: check errors
}

// TODO: put images into list to have handles for them, then only return the handles
uint8_t pmVkGetNextImage(partyRenderer *renderer, uint32_t *idx) {
	VkResult result;
	result = vkWaitForFences(renderer->device->device, 1, &(renderer->swapchain->fence), VK_TRUE, 1000000000);	// FIXME: is there a better way to deal with an unsignaled fence?
	if (result == VK_TIMEOUT) {
		//printf("TIME OUT!!!\n");	
	} else if (result != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to wait for swapchain fence: %d\n", result);
		return 0;
	}

	result = vkResetFences(renderer->device->device, 1, &(renderer->swapchain->fence));
	if (result != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to reset swapchain fence: %d\n", result);
		return 0;
	}

	result = vkAcquireNextImageKHR(renderer->device->device, renderer->swapchain->swapchain, 1000000000, renderer->swapchain->imageReadySemaphore, VK_NULL_HANDLE, &(renderer->swapchain->imageIdx));

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		updateRenderer(renderer);

		vkAcquireNextImageKHR(renderer->device->device, renderer->swapchain->swapchain, UINT64_MAX, renderer->swapchain->imageReadySemaphore, VK_NULL_HANDLE, &(renderer->swapchain->imageIdx));
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		log_printf(LL_ERROR, "Failed to get next image: %d\n", result);
		return 0;
	}

	*idx = renderer->swapchain->imageIdx;

	return 1;
}

uint8_t pmVkPresent(partyRenderer *renderer) {
	VkResult result;

	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext = NULL,

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &(renderer->swapchain->imageFinishedSemaphore),

		.swapchainCount = 1,
		.pSwapchains = &(renderer->swapchain->swapchain),
		.pImageIndices = &(renderer->swapchain->imageIdx),

		.pResults = &result,
	};

	vkQueuePresentKHR(renderer->queue->presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		updateRenderer(renderer);
	} else if (result != VK_SUCCESS) {
		return 0;
	}

	return 1;
}

void startRender(partyRenderer *renderer, uint32_t clearCol) {
	uint32_t currentImage = 0;
	if (!pmVkGetNextImage(renderer, &currentImage)) {
		log_printf(LL_ERROR, "ERROR: failed to get next image\n");
		return;
	}

	vkResetCommandBuffer(renderer->renderCommandBuffer, 0);

	// kind of disorganized here: we've waited for the relevant fance in pmVkGetNextImage, so we can reset descriptor sets safely
	clear_descriptor_pools(renderer, renderer->descriptorAllocator);
	flushTextureDeletes(renderer);

	if (renderer->renderWidth != renderer->renderImage.width || renderer->renderHeight != renderer->renderImage.height) {
		log_printf(LL_INFO, "Adjusting internal render resolution from %dx%d to %dx%d\n", renderer->renderImage.width, renderer->renderImage.height, renderer->renderWidth, renderer->renderHeight);
		destroyRenderTargets(renderer);
		createRenderTargets(renderer, renderer->renderWidth, renderer->renderHeight, GFX_COLOR_FORMAT, GFX_DEPTH_FORMAT);

		imageInfo info = { renderer->renderImage.width, renderer->renderImage.height };
		imageInfo *buffer = mapBuffer(renderer, &renderer->renderImageInfoBuffer);

		memcpy(buffer, &info, sizeof(imageInfo));

		unmapBuffer(renderer, &renderer->renderImageInfoBuffer);
	}

	// begin command buffer
	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = NULL,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = NULL,
	};

	if (vkBeginCommandBuffer(renderer->renderCommandBuffer, &beginInfo) != VK_SUCCESS) {
		log_printf(LL_ERROR, "ERROR: Failed to begin command buffers!\n");
		exit(1);
	}

	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.image = renderer->renderImage.image,
		.subresourceRange = {
		  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		  .baseMipLevel = 0,
		  .levelCount = 1,
		  .baseArrayLayer = 0,
		  .layerCount = 1,
		}
	};

	vkCmdPipelineBarrier(
		renderer->renderCommandBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // dstStageMask
		0,
		0,
		NULL,
		0,
		NULL,
		1, // imageMemoryBarrierCount
		&image_memory_barrier // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION

	// begin rendering
	VkClearColorValue clearColor = {
		.float32[0] = (float)(clearCol & 0xFF) / 255.0f,
		.float32[1] = (float)((clearCol >> 8) & 0xFF) / 255.0f,
		.float32[2] = (float)((clearCol >> 16) & 0xFF) / 255.0f,
		.float32[3] = 1.0f,
	};

	VkClearValue clearVal = {
		.color = clearColor,
	};

	VkRenderingAttachmentInfo colorAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.pNext = NULL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.imageView = renderer->renderImage.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.resolveImageView = renderer->renderImage.imageView,
		.resolveMode = VK_RESOLVE_MODE_NONE,
		.clearValue = clearVal,
	};

	if (clearCol == 0) {
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
	}

	VkClearValue clearDepth = {
		.depthStencil.depth = 0.0f,
		.depthStencil.stencil = 0,
	};

	VkRenderingAttachmentInfo depthAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.pNext = NULL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.imageView = renderer->depthImage.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.resolveImageView = renderer->depthImage.imageView,
		.resolveMode = VK_RESOLVE_MODE_NONE,
		.clearValue = clearDepth,
	};

	VkRect2D renderArea = {
		.offset = { 0.0f, 0.0f },
		.extent = { renderer->renderImage.width, renderer->renderImage.height },
	};

	VkRenderingInfo renderInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.pNext = NULL,
		.flags = 0,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment,
		.pDepthAttachment = &depthAttachment,
		.pStencilAttachment = NULL,
		.layerCount = 1,
		.renderArea = renderArea,
		.viewMask = 0,
	};

	vkCmdBeginRendering(renderer->renderCommandBuffer, &renderInfo);

	vkCmdBindPipeline(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->renderPipelines[0]);

	VkViewport viewport = {
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
		.x = 0.0f,
		.y = 0.0f,
		.width = renderer->renderImage.width,
		.height = renderer->renderImage.height,
	};

	vkCmdSetViewport(renderer->renderCommandBuffer, 0, 1, &viewport);

	vkCmdSetScissor(renderer->renderCommandBuffer, 0, 1, &renderArea);

	vkCmdSetDepthTestEnable(renderer->renderCommandBuffer, VK_FALSE);
	vkCmdSetDepthWriteEnable(renderer->renderCommandBuffer, VK_FALSE);

	VkDescriptorSet descSet = allocate_descriptor_set(renderer, renderer->descriptorAllocator, renderer->renderDescriptorLayout);
	writeTextureDescriptors(renderer);
	update_set(renderer, renderer->descriptorAllocator, descSet);
	clear_writes(renderer->descriptorAllocator);

	vkCmdBindDescriptorSets(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->renderPipelineLayout, 0, 1, &descSet, 0, NULL);

	uint64_t zero = 0;
	vkCmdBindVertexBuffers(renderer->renderCommandBuffer, 0, 1, &(renderer->polyBuffer.buffer.buffer), &zero);

	renderer->currentViewport = viewport;
	renderer->currentScissor = renderArea;
	renderer->currentDepthTestState = 0;
	renderer->currentDepthWriteState = 0;
	renderer->currentBlendState = 0;
	renderer->currentLineState = 0;

	renderer->lastDraw = 0;
	renderer->processedVerts = 0;
}

void flushVerts(partyRenderer *renderer) {
	if (renderer->processedVerts > 0) {
		vkCmdDraw(renderer->renderCommandBuffer, renderer->processedVerts, 1, renderer->lastDraw, 0);
		renderer->lastDraw += renderer->processedVerts;
		renderer->processedVerts = 0;
	}
}

void setPipeline(partyRenderer *renderer, uint32_t pipeline) {
	flushVerts(renderer);
	vkCmdBindPipeline(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->renderPipelines[pipeline]);
}

int getBlendPipeline(uint32_t mode) {
	switch(mode) {
		case 1:
			return 1;
		case 2:
			return 2;
		case 4:
			return 3;
		case 6:
			return 4;
		default:
			log_printf(LL_WARN, "UNKNOWN BLEND MODE %d\n", mode);
		case 0:
		case 3:
			return 0;
		}
}

void setBlendState(partyRenderer *renderer, uint32_t mode) {
	uint32_t state = getBlendPipeline(mode);

	if (renderer->currentBlendState != state) {
		renderer->currentBlendState = state;

		uint32_t lineState = (renderer->currentLineState) ? 5 : 0;

		setPipeline(renderer, state + lineState);
	}

	//vkCmdSetColorBlendEquationEXT(renderer->renderCommandBuffer, 0, 1, &equation);
}

/*void drawTriangleFan(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count) {
	// convenience function for reordering triangle fans (DXPoly is stored like this)
	//vkCmdDraw(renderer->renderCommandBuffer, (vertex_count - 2) * 3, 1, renderer->polyBuffer.currentVertex, 0);
	renderer->processedVerts += vertex_count;

	for (int i = 1; i < vertex_count - 2 + 1; i++) {
		appendPolyBuffer(renderer, vertices, 1);
		appendPolyBuffer(renderer, vertices + i, 2);
	}
}*/

void drawVertices(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count, uint8_t clamp_texture, float zbias) {
	//vkCmdDraw(renderer->renderCommandBuffer, vertex_count, 1, renderer->polyBuffer.currentVertex, 0);
	// fix texture idx for each vertex
	for (int i = 0; i < vertex_count; i++) {
		vertices[i].texture -= 1;

		if (vertices[i].texture > ((int16_t)renderer->textureManager.capacity)) {
			log_printf(LL_ERROR, "invalid texture: 0x%04x\n", vertices[i].texture);
			vertices[i].texture = -1;
		}

		if (vertices[i].texture >= 0 && clamp_texture) {
			vertices[i].texture += renderer->textureManager.capacity;
		}
	}

	if (renderer->currentLineState) {
		setPipeline(renderer, renderer->currentBlendState);
		renderer->currentLineState = 0;
	}
	renderer->processedVerts += vertex_count;

	appendPolyBuffer(renderer, vertices, vertex_count);

	vkCmdSetDepthBiasEnable(renderer->renderCommandBuffer, VK_TRUE);
	vkCmdSetDepthBias(renderer->renderCommandBuffer, zbias, 0.0f, 0.0f);

#ifdef FLUSH_ALL
	flushVerts(renderer);
#endif
}

void drawLines(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count) {
	//vkCmdDraw(renderer->renderCommandBuffer, vertex_count, 1, renderer->polyBuffer.currentVertex, 0);
	if (!renderer->currentLineState) {
		setPipeline(renderer, renderer->currentBlendState + 5);
		renderer->currentLineState = 1;
	}
	renderer->processedVerts += vertex_count;

	appendPolyBuffer(renderer, vertices, vertex_count);

	vkCmdSetDepthBiasEnable(renderer->renderCommandBuffer, VK_FALSE);
	vkCmdSetDepthBias(renderer->renderCommandBuffer, 0.0f, 0.0f, 0.0f);

#ifdef FLUSH_ALL
	flushVerts(renderer);
#endif
}

void setViewport(partyRenderer *renderer, float x, float y, float width, float height) {
	flushVerts(renderer);

	VkViewport viewport = {
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
		.x = x,
		.y = y,
		.width = width,
		.height = height,
	};

	renderer->currentViewport = viewport;

	vkCmdSetViewport(renderer->renderCommandBuffer, 0, 1, &viewport);
}

void setScissor(partyRenderer *renderer, float x, float y, float width, float height) {
	flushVerts(renderer);

	VkRect2D renderArea = {
		.offset = { x, y },
		.extent = { width, height },
	};

	renderer->currentScissor = renderArea;

	vkCmdSetScissor(renderer->renderCommandBuffer, 0, 1, &renderArea);
}

void setDepthState(partyRenderer *renderer, uint8_t test, uint8_t write) {
	if (renderer->currentDepthTestState != test || renderer->currentDepthWriteState != write) {
		flushVerts(renderer);
	}

	if (renderer->currentDepthTestState != test) {
		renderer->currentDepthTestState = test;
		vkCmdSetDepthTestEnable(renderer->renderCommandBuffer, (test) ? VK_TRUE : VK_FALSE);
	}
	
	if (renderer->currentDepthWriteState != write) {
		renderer->currentDepthWriteState = write;
		vkCmdSetDepthWriteEnable(renderer->renderCommandBuffer, (write) ? VK_TRUE : VK_FALSE);
	}
}

void setTextureFilter(partyRenderer *renderer, uint8_t filter) {
	renderer->textureFilter = filter;
}

void drawRenderTexture(partyRenderer *renderer) {
	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier_start = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.image = renderer->swapchain->images[renderer->swapchain->imageIdx],
		.subresourceRange = {
		  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		  .baseMipLevel = 0,
		  .levelCount = 1,
		  .baseArrayLayer = 0,
		  .layerCount = 1,
		}
	};

	vkCmdPipelineBarrier(
		renderer->renderCommandBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // dstStageMask
		0,
		0,
		NULL,
		0,
		NULL,
		1, // imageMemoryBarrierCount
		&image_memory_barrier_start // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION

	// begin rendering
	VkClearColorValue clearColor = {
		.float32[0] = 0.0f,
		.float32[1] = 0.0f,
		.float32[2] = 0.0f,
		.float32[3] = 1.0f,
	};

	VkClearValue clearVal = {
		clearVal.color = clearColor,
	};

	VkRenderingAttachmentInfo colorAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.pNext = NULL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.imageView = renderer->swapchain->imageViews[renderer->swapchain->imageIdx],
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.resolveImageView = renderer->swapchain->imageViews[renderer->swapchain->imageIdx],
		.resolveMode = VK_RESOLVE_MODE_NONE,
		.clearValue = clearVal,
	};

	VkClearValue clearDepth = {
		.depthStencil.depth = 1.0f,
		.depthStencil.stencil = 0,
	};

	VkRenderingAttachmentInfo depthAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.pNext = NULL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.imageView = renderer->depthImage.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.resolveImageView = renderer->depthImage.imageView,
		.resolveMode = VK_RESOLVE_MODE_NONE,
		.clearValue = clearDepth,
	};

	VkRect2D renderArea = {
		.offset = { 0.0f, 0.0f },
		.extent = renderer->swapchain->extent,
	};

	VkRenderingInfo renderInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.pNext = NULL,
		.flags = 0,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment,
		.pDepthAttachment = NULL,
		.pStencilAttachment = NULL,
		.layerCount = 1,
		.renderArea = renderArea,
		.viewMask = 0,
	};

	vkCmdBeginRendering(renderer->renderCommandBuffer, &renderInfo);

	vkCmdBindPipeline(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->scalerPipeline);

	VkViewport viewport = {
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
		.x = 0.0f,
		.y = 0.0f,
		.width = renderer->swapchain->extent.width,
		.height = renderer->swapchain->extent.height,
	};

	// modify viewport for letterboxing/pillarboxing
	float windowAspectRatio = viewport.width / viewport.height;
	if (windowAspectRatio > renderer->aspectRatio) {
		// pillarbox
		float newWidth = viewport.width * (renderer->aspectRatio / windowAspectRatio);
		viewport.x = (viewport.width - newWidth) * 0.5f;
		viewport.width = newWidth;
	} else if (windowAspectRatio < renderer->aspectRatio) {
		// letterbox
		float newHeight = viewport.height * (windowAspectRatio / renderer->aspectRatio);
		viewport.y = (viewport.height - newHeight) * 0.5f;
		viewport.height = newHeight;
	}

	vkCmdSetViewport(renderer->renderCommandBuffer, 0, 1, &viewport);

	vkCmdSetScissor(renderer->renderCommandBuffer, 0, 1, &renderArea);

	uint64_t zero = 0;
	vkCmdBindVertexBuffers(renderer->renderCommandBuffer, 0, 1, &(renderer->scalerBuffer.buffer), &zero);

	VkDescriptorSet descSet = allocate_descriptor_set(renderer, renderer->descriptorAllocator, renderer->scalerLayout);
	write_descriptor_image(renderer->descriptorAllocator, 0, renderer->renderSampler, renderer->renderImage.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
	write_descriptor_buffer(renderer->descriptorAllocator, 1, renderer->renderImageInfoBuffer.buffer, sizeof(imageInfo), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	update_set(renderer, renderer->descriptorAllocator, descSet);
	clear_writes(renderer->descriptorAllocator);

	vkCmdBindDescriptorSets(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->scalerPipelineLayout, 0, 1, &descSet, 0, NULL);

	vkCmdDraw(renderer->renderCommandBuffer, 6, 1, 0, 0);

	vkCmdEndRendering(renderer->renderCommandBuffer);

	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier_end = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.image = renderer->swapchain->images[renderer->swapchain->imageIdx],
		.subresourceRange = {
		  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		  .baseMipLevel = 0,
		  .levelCount = 1,
		  .baseArrayLayer = 0,
		  .layerCount = 1,
		}
	};

	vkCmdPipelineBarrier(
		renderer->renderCommandBuffer,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // dstStageMask
		0,
		0,
		NULL,
		0,
		NULL,
		1, // imageMemoryBarrierCount
		&image_memory_barrier_end // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION
}

void finishRender(partyRenderer *renderer) {
	flushVerts(renderer);

	vkCmdEndRendering(renderer->renderCommandBuffer);

	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		.image = renderer->renderImage.image,
		.subresourceRange = {
		  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		  .baseMipLevel = 0,
		  .levelCount = 1,
		  .baseArrayLayer = 0,
		  .layerCount = 1,
		}
	};

	vkCmdPipelineBarrier(
		renderer->renderCommandBuffer,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // dstStageMask
		0,
		0,
		NULL,
		0,
		NULL,
		1, // imageMemoryBarrierCount
		&image_memory_barrier // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION

	drawRenderTexture(renderer);

	vkEndCommandBuffer(renderer->renderCommandBuffer);

	VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = NULL,

		// TODO: store all command buffers in array?
		.commandBufferCount = 1,
		.pCommandBuffers = &(renderer->renderCommandBuffer),

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &(renderer->swapchain->imageReadySemaphore),

		.pWaitDstStageMask = &waitStages,

		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &(renderer->swapchain->imageFinishedSemaphore),
	};

	updatePolyBuffer(renderer);
	resetPolyBuffer(renderer);

	if (renderer->pendingImageWrites->count > 0) {
		vkQueueWaitIdle(renderer->memQueue->queue);
	}

	if (vkQueueSubmit(renderer->queue->queue, 1, &submitInfo, renderer->swapchain->fence) != VK_SUCCESS) {
		log_printf(LL_ERROR, "ERROR: Failed to submit command queue!\n");
		exit(1);
	}

	if (!pmVkPresent(renderer)) {
		log_printf(LL_ERROR, "Present failed!\n");	
	}

	if (renderer->pendingImageWrites->count > 0) {
		for (int i = 0; i < renderer->pendingImageWrites->count; i++) {
			pendingImageWrite *imgWrite = ((pendingImageWrite *)renderer->pendingImageWrites->data) + i;
			vkFreeCommandBuffers(renderer->device->device, renderer->memQueue->commandPool, 1, &imgWrite->cmdbuf);
			destroyBuffer(renderer, &imgWrite->transferbuf);
		}
		renderer->pendingImageWrites->count = 0;
	}
	vkResetCommandPool(renderer->device->device, renderer->memQueue->commandPool, 0);
}

void setRenderResolution(partyRenderer *renderer, uint32_t width, uint32_t height, float aspectRatio) {
	renderer->aspectRatio = aspectRatio;
	renderer->renderWidth = width;
	renderer->renderHeight = height;
}

void renderImageFrame(partyRenderer *renderer, uint32_t texIdx) {
	texIdx--;

	uint32_t currentImage = 0;
	if (!pmVkGetNextImage(renderer, &currentImage)) {
		log_printf(LL_ERROR, "ERROR: failed to get next image\n");
		return;
	}

	vkResetCommandBuffer(renderer->renderCommandBuffer, 0);

	// kind of disorganized here: we've waited for the relevant fance in pmVkGetNextImage, so we can reset descriptor sets safely
	clear_descriptor_pools(renderer, renderer->descriptorAllocator);
	flushTextureDeletes(renderer);

	if (renderer->renderWidth != renderer->textureManager.images[texIdx].width || renderer->renderHeight != renderer->textureManager.images[texIdx].height) {
		imageInfo info = { renderer->textureManager.images[texIdx].width, renderer->textureManager.images[texIdx].height };
		imageInfo *buffer = mapBuffer(renderer, &renderer->renderImageInfoBuffer);

		memcpy(buffer, &info, sizeof(imageInfo));

		unmapBuffer(renderer, &renderer->renderImageInfoBuffer);
	}

	// begin command buffer
	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = NULL,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = NULL,
	};

	if (vkBeginCommandBuffer(renderer->renderCommandBuffer, &beginInfo) != VK_SUCCESS) {
		log_printf(LL_ERROR, "ERROR: Failed to begin command buffers!\n");
		exit(1);
	}

	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier_start = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.image = renderer->swapchain->images[renderer->swapchain->imageIdx],
		.subresourceRange = {
		  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		  .baseMipLevel = 0,
		  .levelCount = 1,
		  .baseArrayLayer = 0,
		  .layerCount = 1,
		}
	};

	vkCmdPipelineBarrier(
		renderer->renderCommandBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // dstStageMask
		0,
		0,
		NULL,
		0,
		NULL,
		1, // imageMemoryBarrierCount
		&image_memory_barrier_start // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION

	// begin rendering
	VkClearColorValue clearColor = {
		.float32[0] = 0.0f,
		.float32[1] = 0.0f,
		.float32[2] = 0.0f,
		.float32[3] = 1.0f,
	};

	VkClearValue clearVal = {
		.color = clearColor,
	};

	VkRenderingAttachmentInfo colorAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.pNext = NULL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.imageView = renderer->swapchain->imageViews[renderer->swapchain->imageIdx],
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.resolveImageView = renderer->swapchain->imageViews[renderer->swapchain->imageIdx],
		.resolveMode = VK_RESOLVE_MODE_NONE,
		.clearValue = clearVal,
	};

	VkClearValue clearDepth = {
		.depthStencil.depth = 1.0f,
		.depthStencil.stencil = 0,
	};

	VkRenderingAttachmentInfo depthAttachment = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
		.pNext = NULL,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.imageView = renderer->depthImage.imageView,
		.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
		.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.resolveImageView = renderer->depthImage.imageView,
		.resolveMode = VK_RESOLVE_MODE_NONE,
		.clearValue = clearDepth,
	};

	VkRect2D renderArea = {
		.offset = { 0.0f, 0.0f },
		.extent = renderer->swapchain->extent,
	};

	VkRenderingInfo renderInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
		.pNext = NULL,
		.flags = 0,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment,
		.pDepthAttachment = NULL,
		.pStencilAttachment = NULL,
		.layerCount = 1,
		.renderArea = renderArea,
		.viewMask = 0,
	};

	vkCmdBeginRendering(renderer->renderCommandBuffer, &renderInfo);

	vkCmdBindPipeline(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->scalerPipeline);

	VkViewport viewport = {
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
		.x = 0.0f,
		.y = 0.0f,
		.width = renderer->swapchain->extent.width,
		.height = renderer->swapchain->extent.height,
	};

	// modify viewport for letterboxing/pillarboxing
	float windowAspectRatio = viewport.width / viewport.height;
	if (windowAspectRatio > renderer->aspectRatio) {
		// pillarbox
		float newWidth = viewport.width * (renderer->aspectRatio / windowAspectRatio);
		viewport.x = (viewport.width - newWidth) * 0.5f;
		viewport.width = newWidth;
	} else if (windowAspectRatio < renderer->aspectRatio) {
		// letterbox
		float newHeight = viewport.height * (windowAspectRatio / renderer->aspectRatio);
		viewport.y = (viewport.height - newHeight) * 0.5f;
		viewport.height = newHeight;
	}

	vkCmdSetViewport(renderer->renderCommandBuffer, 0, 1, &viewport);

	vkCmdSetScissor(renderer->renderCommandBuffer, 0, 1, &renderArea);

	uint64_t zero = 0;
	vkCmdBindVertexBuffers(renderer->renderCommandBuffer, 0, 1, &(renderer->scalerBuffer.buffer), &zero);

	VkDescriptorSet descSet = allocate_descriptor_set(renderer, renderer->descriptorAllocator, renderer->scalerLayout);
	write_descriptor_image(renderer->descriptorAllocator, 0, renderer->renderSampler, renderer->textureManager.images[texIdx].imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
	write_descriptor_buffer(renderer->descriptorAllocator, 1, renderer->renderImageInfoBuffer.buffer, sizeof(imageInfo), 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	update_set(renderer, renderer->descriptorAllocator, descSet);
	clear_writes(renderer->descriptorAllocator);

	vkCmdBindDescriptorSets(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->scalerPipelineLayout, 0, 1, &descSet, 0, NULL);

	vkCmdDraw(renderer->renderCommandBuffer, 6, 1, 0, 0);

	vkCmdEndRendering(renderer->renderCommandBuffer);

	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier_end = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		.image = renderer->swapchain->images[renderer->swapchain->imageIdx],
		.subresourceRange = {
		  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		  .baseMipLevel = 0,
		  .levelCount = 1,
		  .baseArrayLayer = 0,
		  .layerCount = 1,
		}
	};

	vkCmdPipelineBarrier(
		renderer->renderCommandBuffer,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  // srcStageMask
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // dstStageMask
		0,
		0,
		NULL,
		0,
		NULL,
		1, // imageMemoryBarrierCount
		&image_memory_barrier_end // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION

	//drawRenderTexture(renderer);

	vkEndCommandBuffer(renderer->renderCommandBuffer);

	VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = NULL,

		// TODO: store all command buffers in array?
		.commandBufferCount = 1,
		.pCommandBuffers = &(renderer->renderCommandBuffer),

		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &(renderer->swapchain->imageReadySemaphore),

		.pWaitDstStageMask = &waitStages,

		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &(renderer->swapchain->imageFinishedSemaphore),
	};

	if (renderer->pendingImageWrites->count > 0) {
		vkQueueWaitIdle(renderer->memQueue->queue);
	}

	if (vkQueueSubmit(renderer->queue->queue, 1, &submitInfo, renderer->swapchain->fence) != VK_SUCCESS) {
		log_printf(LL_ERROR, "ERROR: Failed to submit command queue!");
		exit(1);
	}

	if (!pmVkPresent(renderer)) {
		log_printf(LL_ERROR, "Present failed!\n");	
	}

	if (renderer->pendingImageWrites->count > 0) {
		for (int i = 0; i < renderer->pendingImageWrites->count; i++) {
			pendingImageWrite *imgWrite = ((pendingImageWrite *)renderer->pendingImageWrites->data) + i;
			vkFreeCommandBuffers(renderer->device->device, renderer->memQueue->commandPool, 1, &imgWrite->cmdbuf);
			destroyBuffer(renderer, &imgWrite->transferbuf);
		}
		renderer->pendingImageWrites->count = 0;
	}
	vkResetCommandPool(renderer->device->device, renderer->memQueue->commandPool, 0);

	if (renderer->renderWidth != renderer->textureManager.images[texIdx].width || renderer->renderHeight != renderer->textureManager.images[texIdx].height) {
		imageInfo info = { renderer->renderWidth, renderer->renderHeight };
		imageInfo *buffer = mapBuffer(renderer, &renderer->renderImageInfoBuffer);

		memcpy(buffer, &info, sizeof(imageInfo));

		unmapBuffer(renderer, &renderer->renderImageInfoBuffer);
	}
}
