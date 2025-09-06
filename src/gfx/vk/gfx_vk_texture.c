#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <volk.h>
//#include <vulkan/vulkan.h>

#include <gfx/vk/vk.h>
#include <log.h>

uint8_t isDepthFormat(VkFormat format) {
	return (format == VK_FORMAT_D16_UNORM || 
		format == VK_FORMAT_D16_UNORM_S8_UINT || 
		format == VK_FORMAT_D24_UNORM_S8_UINT || 
		format == VK_FORMAT_D32_SFLOAT ||
		format == VK_FORMAT_D32_SFLOAT_S8_UINT);
}

void createRenderTargets(partyRenderer *renderer, uint32_t width, uint32_t height, VkFormat colorFmt, VkFormat depthFmt) {
	renderer->depthImage.type = VK_IMAGE_TYPE_2D;
	renderer->depthImage.pixelFormat = depthFmt;

	renderer->depthImage.width = width;
	renderer->depthImage.height = height;
	renderer->depthImage.depth = 1;

	renderer->depthImage.mipmapCount = 1;
	renderer->depthImage.arrayLength = 1;
	renderer->depthImage.sampleCount = 1;

	renderer->renderImage.type = VK_IMAGE_TYPE_2D;
	renderer->renderImage.pixelFormat = colorFmt;

	renderer->renderImage.width = width;
	renderer->renderImage.height = height;
	renderer->renderImage.depth = 1;

	renderer->renderImage.mipmapCount = 1;
	renderer->renderImage.arrayLength = 1;
	renderer->renderImage.sampleCount = 1;

	//renderer->depthImage.flags = 0;

	VkImageCreateInfo imgCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.imageType = VK_IMAGE_TYPE_2D,
		.extent.width = width,
		.extent.height = height,
		.extent.depth = 1,
		.mipLevels = 1,
		.arrayLayers = 1,

		.format = depthFmt,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,

		.samples = 1,

		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = NULL,
	};

	log_printf(LL_TRACE, "createRenderTargets(): depth image\n");
	if (vkCreateImage(renderer->device->device, &imgCreateInfo, NULL, &(renderer->depthImage.image)) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create depth texture!\n");
		exit(1);
	}

	imgCreateInfo.format = colorFmt;
	imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	log_printf(LL_TRACE, "createRenderTargets(): color image\n");
	if (vkCreateImage(renderer->device->device, &imgCreateInfo, NULL, &(renderer->renderImage.image)) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create render texture!\n");
		exit(1);
	}

	// allocate memory

	VmaAllocationCreateInfo allocInfo = {
		.flags = 0,
		.usage = VMA_MEMORY_USAGE_GPU_ONLY,
		.requiredFlags = 0,
		.preferredFlags = 0,
		.memoryTypeBits = 0,
		.pool = VK_NULL_HANDLE,
		.pUserData = NULL,
		.priority = 0.0f,
	};

	log_printf(LL_TRACE, "createRenderTargets(): allocate memory for depth image\n");
	if (vmaAllocateMemoryForImage(renderer->memoryManager->allocator, renderer->depthImage.image, &allocInfo, &renderer->depthImage.allocation, NULL) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to allocate depth image memory!\n");
		exit(1);
	}

	log_printf(LL_TRACE, "createRenderTargets(): bind memory for depth image\n");
	if (vmaBindImageMemory(renderer->memoryManager->allocator, renderer->depthImage.allocation, renderer->depthImage.image) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to bind depth image memory!\n");
		exit(1);
	}

	log_printf(LL_TRACE, "createRenderTargets(): allocate memory for color image\n");
	if (vmaAllocateMemoryForImage(renderer->memoryManager->allocator, renderer->renderImage.image, &allocInfo, &renderer->renderImage.allocation, NULL) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to allocate color image memory!\n");
		exit(1);
	}

	log_printf(LL_TRACE, "createRenderTargets(): bind memory for color image\n");
	if (vmaBindImageMemory(renderer->memoryManager->allocator, renderer->renderImage.allocation, renderer->renderImage.image) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to bind color image memory!\n");
		exit(1);
	}

	// create image view

	VkImageViewCreateInfo viewCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.image = renderer->depthImage.image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = depthFmt,

		.components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.a = VK_COMPONENT_SWIZZLE_IDENTITY,

		//.subresourceRange.aspectMask = (isDepthFormat(info->pixelFormat)) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT,
		.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
		.subresourceRange.baseMipLevel = 0,
		.subresourceRange.levelCount = 1,
		.subresourceRange.baseArrayLayer = 0,
		.subresourceRange.layerCount = 1,
	};

	log_printf(LL_TRACE, "createRenderTargets(): depth image view\n");
	if(vkCreateImageView(renderer->device->device, &viewCreateInfo, NULL, &(renderer->depthImage.imageView)) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create depth image view");
		exit(1);
	}

	viewCreateInfo.image = renderer->renderImage.image;
	viewCreateInfo.format = colorFmt;
	viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	log_printf(LL_TRACE, "createRenderTargets(): color image view\n");
	if(vkCreateImageView(renderer->device->device, &viewCreateInfo, NULL, &(renderer->renderImage.imageView)) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create render image view");
		exit(1);
	}

	// create render image sampler

	VkSamplerCreateInfo samplerInfo = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,

		.minFilter = VK_FILTER_LINEAR,
		.magFilter = VK_FILTER_LINEAR,

		.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,

		.anisotropyEnable = VK_FALSE,
		.maxAnisotropy = 0,

		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,

		.unnormalizedCoordinates = VK_FALSE,

		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,

		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias = 0,
		.minLod = VK_LOD_CLAMP_NONE,
		.maxLod = VK_LOD_CLAMP_NONE,
	};

	log_printf(LL_TRACE, "createRenderTargets(): sampler\n");
	if(vkCreateSampler(renderer->device->device, &samplerInfo, NULL, &(renderer->renderSampler)) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create sampler");
		exit(1);
	}
}

void destroyRenderTargets(partyRenderer *renderer) {
	vkDestroyImageView(renderer->device->device, renderer->depthImage.imageView, NULL);
	vkDestroyImage(renderer->device->device, renderer->depthImage.image, NULL);
	vmaFreeMemory(renderer->memoryManager->allocator, renderer->depthImage.allocation);

	vkDestroySampler(renderer->device->device, renderer->renderSampler, NULL);
	vkDestroyImageView(renderer->device->device, renderer->renderImage.imageView, NULL);
	vkDestroyImage(renderer->device->device, renderer->renderImage.image, NULL);
	vmaFreeMemory(renderer->memoryManager->allocator, renderer->renderImage.allocation);
}

VkResult createTexture(partyRenderer *renderer, uint32_t width, uint32_t height, pmVkImage *result) {
	result->type = VK_IMAGE_TYPE_2D;
	result->pixelFormat = VK_FORMAT_R8G8B8A8_UNORM;

	result->width = width;
	result->height = height;
	result->depth = 1;
	result->pixelFormat = VK_FORMAT_R8G8B8A8_UNORM;

	result->mipmapCount = 1;
	result->arrayLength = 1;
	result->sampleCount = 1;

	//result->flags = 0;

	VkImageCreateInfo imgCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.imageType = VK_IMAGE_TYPE_2D,
		.extent.width = result->width,
		.extent.height = result->height,
		.extent.depth = 1,
		.mipLevels = 1,
		.arrayLayers = 1,

		.format = result->pixelFormat,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,

		.samples = 1,

		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = NULL,
	};

	VkResult r = vkCreateImage(renderer->device->device, &imgCreateInfo, NULL, &(result->image));
	if (r != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create texture!\n");
		return r;
	}

	// allocate memory

	VmaAllocationCreateInfo allocInfo = {
		.flags = 0,
		.usage = VMA_MEMORY_USAGE_GPU_ONLY,
		.requiredFlags = 0,
		.preferredFlags = 0,
		.memoryTypeBits = 0,
		.pool = VK_NULL_HANDLE,
		.pUserData = NULL,
		.priority = 0.0f,
	};

	vmaAllocateMemoryForImage(renderer->memoryManager->allocator, result->image, &allocInfo, &result->allocation, NULL);

	vmaBindImageMemory(renderer->memoryManager->allocator, result->allocation, result->image);

	// create image view

	VkImageViewCreateInfo viewCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.image = result->image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = result->pixelFormat,

		.components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
		.components.a = VK_COMPONENT_SWIZZLE_IDENTITY,

		.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.subresourceRange.baseMipLevel = 0,
		.subresourceRange.levelCount = 1,
		.subresourceRange.baseArrayLayer = 0,
		.subresourceRange.layerCount = 1,
	};

	r = vkCreateImageView(renderer->device->device, &viewCreateInfo, NULL, &(result->imageView));
	if(r != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create image view");
	}

	return r;
}

void destroyTexture(partyRenderer *renderer, pmVkImage img) {
	vkDestroyImageView(renderer->device->device, img.imageView, NULL);
	vkDestroyImage(renderer->device->device, img.image, NULL);
	vmaFreeMemory(renderer->memoryManager->allocator, img.allocation);
	//free(img);
}

VkSampler createSampler(partyRenderer *renderer, VkFilter minMagFilter) {
	VkSampler result;

	VkSamplerCreateInfo samplerInfo = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,

		.minFilter = minMagFilter,
		.magFilter = minMagFilter,

		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,

		.anisotropyEnable = VK_FALSE,
		.maxAnisotropy = 0,

		.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,

		.unnormalizedCoordinates = VK_FALSE,

		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,

		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias = 0,
		.minLod = 0,
		.maxLod = VK_LOD_CLAMP_NONE,
	};

	if(vkCreateSampler(renderer->device->device, &samplerInfo, NULL, &result) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create sampler");
		exit(1);
	}

	return result;
}

void updateTexture(partyRenderer *renderer, pmVkImage *img, uint32_t width, uint32_t height, void *data) {
	VkDeviceSize sz = width * height * 4;	// assuming rgba8

	pmVkBuffer transferBuffer;
	createBuffer(renderer, sz, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, &transferBuffer);

	void *buf = mapBuffer(renderer, &transferBuffer);
	memcpy(buf, data, sz);
	unmapBuffer(renderer, &transferBuffer);

	VkCommandBuffer cmdbuf = startStagingCommandBuffer(renderer);

	//IMAGE COPY GOES HERE

	// start layout change

	VkImageMemoryBarrier imgMemBarrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.pNext = NULL,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = img->image,
		.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.subresourceRange.baseMipLevel = 0,
		.subresourceRange.levelCount = 1,
		.subresourceRange.baseArrayLayer = 0,
		.subresourceRange.layerCount = 1,

		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.srcAccessMask = 0,
		.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
	};

	vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);

	// end layout change

	// start image copy

	VkBufferImageCopy copyRegion = {
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,

		.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.imageSubresource.mipLevel = 0,
		.imageSubresource.baseArrayLayer = 0,
		.imageSubresource.layerCount = 1,

		.imageOffset.x = 0,
		.imageOffset.y = 0,
		.imageOffset.z = 0,

		.imageExtent.width = width,
		.imageExtent.height = height,
		.imageExtent.depth = 1,
	};

	vkCmdCopyBufferToImage(cmdbuf, transferBuffer.buffer, img->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

	// end image copy
	
	// start layout change

	imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imgMemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imgMemBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);

	// end layout change

	//END IMAGE COPY

	endStagingCommandBuffer(renderer, cmdbuf);

	//destroyBuffer(renderer, &transferBuffer);
	pendingImageWrite imageWrite = {
		.cmdbuf = cmdbuf,
		.transferbuf = transferBuffer,
	};

	sb_push_back(renderer->pendingImageWrites, &imageWrite);
}

inline int32_t i32max(int32_t a, int32_t b) {
	return (a > b) ? a : b;
}
