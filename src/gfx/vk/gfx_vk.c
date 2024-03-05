#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <vulkan/vulkan.h>

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

	printf("VALIDATION LAYER: %s\n", pCallbackData->pMessage);

	return VK_FALSE;
}

void createDebugMessenger() {
	VkDebugUtilsMessengerCreateInfoEXT messengerInfo;
	messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerInfo.pNext = NULL;
	messengerInfo.flags = 0;
	messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
	messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	messengerInfo.pfnUserCallback = debugMessageCallback;
	messengerInfo.pUserData = NULL;

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
		printf("Failed to enumerate instance layer properties\n");
		return 0;
	}

	VkLayerProperties *supportedLayers = malloc(sizeof(VkLayerProperties) * supportedLayerCount);
	r = vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers);
	if (r) {
		printf("Failed to enumerate instance layer properties\n");
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
			printf("Layer not supported: %s\n", *i);
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
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = "PARTYMOD for THPS2";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "M3D";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	// get extension count
	uint32_t extCount = 0;
	char **extNames = NULL;

	appendWindowExtensions(&extCount, &extNames);

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = NULL;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;

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
		printf("Failed to get validation layer!\n");

		instanceInfo.enabledLayerCount = 0;
		instanceInfo.ppEnabledLayerNames = NULL;
	}
#endif

	appendExtensions(&extCount, &extNames, reqExtensionCount, reqExtensionNames);

#ifdef DEBUG
	printf("Creating Vulkan instance with %u extensions:\n", extCount);
	for (int i = 0; i < extCount; i++) {
		printf("%s\n", extNames[i]);
	}
#endif

	instanceInfo.enabledExtensionCount = extCount;
	instanceInfo.ppEnabledExtensionNames = extNames;

	VkResult result = vkCreateInstance(&instanceInfo, NULL, &instance);

	if (result != VK_SUCCESS) {
		// error
		printf("ERROR: Failed to create Vulkan instance!\n");
		return result;
	}

#ifdef VK_VALIDATION
	vkpfn_vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	vkpfn_vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (!vkpfn_vkCreateDebugUtilsMessengerEXT) {
		printf("ERROR: Failed to load vkCreateDebugUtilsMessengerEXT function!\n");
		//return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	if (!vkpfn_vkDestroyDebugUtilsMessengerEXT) {
		printf("ERROR: Failed to load vkDestroyDebugUtilsMessengerEXT function!\n");
		//return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	if (vkpfn_vkCreateDebugUtilsMessengerEXT)
		createDebugMessenger();
#endif

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

	result.vertexCapacity = 10000;
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

	renderer->polyBuffer.currentVertex += vertex_count;
}

void resetPolyBuffer(partyRenderer *renderer) {
	renderer->polyBuffer.currentVertex = 0;
}

void updatePolyBuffer(partyRenderer *renderer) {
	printf("Updating poly buffer with %d vertices\n", renderer->polyBuffer.currentVertex);

	renderVertex *buffer = mapBuffer(renderer, &(renderer->polyBuffer.buffer));

	memcpy(buffer, renderer->polyBuffer.vertices, sizeof(renderVertex) * renderer->polyBuffer.currentVertex);

	unmapBuffer(renderer, &(renderer->polyBuffer.buffer));
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

	//result->parent.driver = &rbVkDriver;

	r = rbVkCreateWindow(windowHandle, &(result->window));
	if (r) {
		goto error_free;
	}

	r = createVulkanDevice(result->window, &(result->device));
	if (r) {
		goto error_free;
	}

	r = rbVkCreateSwapchain(result->device, &(result->swapchain));
	if (r) {
		goto error_free;
	}

	r = rbVkCreateCommandQueue(result->device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, &(result->queue));
	if (r) {
		goto error_free;
	}

	r = rbVkCreateCommandQueue(result->device, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, &(result->memQueue));
	if (r) {
		goto error_free;
	}

	r = createRenderCommandBuffer(result);
	if (r) {
		goto error_free;
	}

	r = createRenderPipelines(result);
	if (r) {
		goto error_free;
	}

	r = rbVkInitMemoryManager(result, &(result->memoryManager));
	if (r) {
		goto error_free;
	}

	r = createPolyBuffer(result);
	if (r) {
		goto error_free;
	}

	/*result->renderPassCache = initRenderPassCache(result->device);
	result->framebufferCache = initFramebufferCache(result);
	result->pipelineCache = initPipelineCache(result);
	result->descriptorSetCache = initDescriptorSetCache(result);

	result->commandBuffers = createHandleList(sizeof(commandBuffer_t), 1);*/

	*renderer = result;

	return 1;

error_free:
	free(result);
error:
	printf("failed to create renderer\n");
	// todo: translate error
	return 0;
}

void rbVkDestroyRenderer(partyRenderer *renderer) {
	/*destroyFramebufferCache(renderer->framebufferCache);
	destroyPipelineCache(renderer->pipelineCache);
	destroyRenderPassCache(renderer->renderPassCache);
	destroyDescriptorSetCache(renderer->descriptorSetCache);*/
	
	//rbVkDestroyMemoryManager(renderer->memoryManager);
	destroyRenderCommandBuffer(renderer);
	rbVkDestroyCommandQueue(renderer->memQueue);
	rbVkDestroyCommandQueue(renderer->queue);
	rbVkDestroySwapchain(renderer->swapchain);
	rbVkDestroyDevice(renderer->device);
	rbVkDestroyWindow(renderer->window);

	//destroyPHandleList(renderer->pipelines);
	// TODO: add renderpass and framebuffer cache destruction ?
	//destroyPHandleList(renderer->renderPasses);
	//destroyPHandleList(renderer->framebuffers);

	free(renderer);

	destroyInstance();
}

uint8_t rbVkWaitIdle(partyRenderer *renderer) {
	VkResult result = vkDeviceWaitIdle(renderer->device->device);
	
	if (result != VK_SUCCESS) {
		// TODO: better errors
		return 0;
	}

	return 1;
}

/*void invalidateCommandBuffers(vid2_renderer_t *renderer) {
	for (uint64_t i = 0; i < renderer->commandBuffers->len; i++) {
		commandBuffer_t *data = renderer->commandBuffers->data;
		data[i].dirty = 1;
	}
}*/

void rbVkUpdateRenderer(partyRenderer *renderer) {
	rbVkWaitIdle(renderer);

	//clearFramebufferCache(renderer->framebufferCache);	// avoid reusing old framebuffers
	//clearRenderPassCache(renderer->renderPassCache);

	rbVkDestroySwapchain(renderer->swapchain);
	rbVkCreateSwapchain(renderer->device, &(renderer->swapchain));	// FIXME: check errors
	
	//invalidateCommandBuffers(renderer);
}

// TODO: put images into list to have handles for them, then only return the handles
uint8_t rbVkGetNextImage(partyRenderer *renderer, uint32_t *idx) {
	VkResult result;
	result = vkWaitForFences(renderer->device->device, 1, &(renderer->swapchain->fence), VK_TRUE, 1000000000);	// FIXME: is there a better way to deal with an unsignaled fence?
	if (result == VK_TIMEOUT) {
		printf("TIME OUT!!!\n");	
	} else if (result != VK_SUCCESS) {
		printf("Failed to wait for swapchain fence: %d\n", result);
		return 0;
	}

	result = vkResetFences(renderer->device->device, 1, &(renderer->swapchain->fence));
	if (result != VK_SUCCESS) {
		printf("Failed to reset swapchain fence: %d\n", result);
		return 0;
	}

	result = vkAcquireNextImageKHR(renderer->device->device, renderer->swapchain->swapchain, 1000000000, renderer->swapchain->imageReadySemaphore, VK_NULL_HANDLE, &(renderer->swapchain->imageIdx));

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		rbVkUpdateRenderer(renderer);

		vkAcquireNextImageKHR(renderer->device->device, renderer->swapchain->swapchain, UINT64_MAX, renderer->swapchain->imageReadySemaphore, VK_NULL_HANDLE, &(renderer->swapchain->imageIdx));
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		printf("Failed to get next image: %d\n", result);
		return 0;
	}

	*idx = renderer->swapchain->imageIdx;

	return 1;
}

uint8_t rbVkPresent(partyRenderer *renderer) {
	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = NULL;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &(renderer->swapchain->imageFinishedSemaphore);

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &(renderer->swapchain->swapchain);
	presentInfo.pImageIndices = &(renderer->swapchain->imageIdx);

	VkResult result;
	presentInfo.pResults = &result;

	vkQueuePresentKHR(renderer->queue->presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		rbVkUpdateRenderer(renderer);
	} else if (result != VK_SUCCESS) {
		return 0;
	}

	return 1;
}

void startRender(partyRenderer *renderer) {
	uint32_t currentImage = 0;
	if (!rbVkGetNextImage(renderer, &currentImage)) {
		printf("ERROR: failed to get next image\n");
		return;
	}

	// begin command buffer
	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = NULL;

	if (vkBeginCommandBuffer(renderer->renderCommandBuffer, &beginInfo) != VK_SUCCESS) {
		printf("ERROR: Failed to begin command buffers!\n");
		exit(1);
	}

	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier = {
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
		&image_memory_barrier // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION

	// begin rendering
	VkClearColorValue clearColor;
	clearColor.float32[0] = 0.0f;
	clearColor.float32[1] = 0.0f;
	clearColor.float32[2] = 0.0f;
	clearColor.float32[3] = 1.0f;

	VkClearValue clearVal;
	clearVal.color = clearColor;

	VkRenderingAttachmentInfo colorAttachment;
	colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
	colorAttachment.pNext = NULL;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.imageView = renderer->swapchain->imageViews[currentImage];
	colorAttachment.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
	colorAttachment.resolveImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	colorAttachment.resolveImageView = renderer->swapchain->imageViews[currentImage];
	colorAttachment.resolveMode = VK_RESOLVE_MODE_NONE;
	colorAttachment.clearValue = clearVal;

	VkRect2D renderArea;
	renderArea.offset = (VkOffset2D) { 0.0f, 0.0f };
	renderArea.extent = renderer->swapchain->extent;

	VkRenderingInfo renderInfo;
	renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
	renderInfo.pNext = NULL;
	renderInfo.flags = 0;
	renderInfo.colorAttachmentCount = 1;
	renderInfo.pColorAttachments = &colorAttachment;
	renderInfo.pDepthAttachment = NULL;
	renderInfo.pStencilAttachment = NULL;
	renderInfo.layerCount = 1;
	renderInfo.renderArea = renderArea;
	renderInfo.viewMask = 0;

	vkCmdBeginRendering(renderer->renderCommandBuffer, &renderInfo);

	vkCmdBindPipeline(renderer->renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->renderPipeline);

	VkViewport viewport;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = renderer->swapchain->extent.width;
	viewport.height = renderer->swapchain->extent.height;

	vkCmdSetViewport(renderer->renderCommandBuffer, 0, 1, &viewport);

	vkCmdSetScissor(renderer->renderCommandBuffer, 0, 1, &renderArea);

	uint64_t zero = 0;
	vkCmdBindVertexBuffers(renderer->renderCommandBuffer, 0, 1, &(renderer->polyBuffer.buffer.buffer), &zero);
}

void drawTriangleFan(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count) {
	// convenience function for reordering triangle fans (DXPoly is stored like this)
	vkCmdDraw(renderer->renderCommandBuffer, (vertex_count - 2) * 3, 1, renderer->polyBuffer.currentVertex, 0);

	for (int i = 1; i < vertex_count - 2 + 1; i++) {
		appendPolyBuffer(renderer, vertices, 1);
		appendPolyBuffer(renderer, vertices + i, 2);
	}
}

void drawVertices(partyRenderer *renderer, renderVertex *vertices, uint32_t vertex_count) {
	vkCmdDraw(renderer->renderCommandBuffer, vertex_count, 1, renderer->polyBuffer.currentVertex, 0);

	appendPolyBuffer(renderer, vertices, vertex_count);
}

void setViewport(partyRenderer *renderer, float x, float y, float width, float height) {
	VkViewport viewport;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.x = x;
	viewport.y = y;
	viewport.width = width;
	viewport.height = height;

	vkCmdSetViewport(renderer->renderCommandBuffer, 0, 1, &viewport);
}

void setScissor(partyRenderer *renderer, float x, float y, float width, float height) {
	VkRect2D renderArea;
	renderArea.offset = (VkOffset2D) { x, y };
	renderArea.extent = (VkExtent2D) { width, height };

	vkCmdSetScissor(renderer->renderCommandBuffer, 0, 1, &renderArea);
}

void finishRender(partyRenderer *renderer) {
	vkCmdEndRendering(renderer->renderCommandBuffer);

	// START IMAGE TRANSITION

	const VkImageMemoryBarrier image_memory_barrier = {
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
		&image_memory_barrier // pImageMemoryBarriers
	);

	// END IMAGE TRANSITION

	vkEndCommandBuffer(renderer->renderCommandBuffer);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;

	// TODO: store all command buffers in array?
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &(renderer->renderCommandBuffer);

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &(renderer->swapchain->imageReadySemaphore);

	VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submitInfo.pWaitDstStageMask = &waitStages;

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &(renderer->swapchain->imageFinishedSemaphore);

	updatePolyBuffer(renderer);
	resetPolyBuffer(renderer);

	if (vkQueueSubmit(renderer->queue->queue, 1, &submitInfo, renderer->swapchain->fence) != VK_SUCCESS) {
		printf("ERROR: Failed to submit command queue!");
		exit(1);
	}

	if (!rbVkPresent(renderer)) {
		printf("Present failed!\n");	
	}
}