#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <vulkan/vulkan.h>

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

	VkImageCreateInfo imgCreateInfo;
	imgCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgCreateInfo.pNext = NULL;
	imgCreateInfo.flags = 0;
	imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imgCreateInfo.extent.width = width;
	imgCreateInfo.extent.height = height;
	imgCreateInfo.extent.depth = 1;
	imgCreateInfo.mipLevels = 1;
	imgCreateInfo.arrayLayers = 1;

	imgCreateInfo.format = depthFmt;
	imgCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imgCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	imgCreateInfo.samples = 1;

	imgCreateInfo.queueFamilyIndexCount = 0;
	imgCreateInfo.pQueueFamilyIndices = NULL;

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

	VmaAllocationCreateInfo allocInfo;
	allocInfo.flags = 0;
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocInfo.requiredFlags = 0;
	allocInfo.preferredFlags = 0;
	allocInfo.memoryTypeBits = 0;
	allocInfo.pool = VK_NULL_HANDLE;
	allocInfo.pUserData = NULL;
	allocInfo.priority = 0.0f;

	log_printf(LL_TRACE, "createRenderTargets(): allocate and bind memory\n");
	vmaAllocateMemoryForImage(renderer->memoryManager->allocator, renderer->depthImage.image, &allocInfo, &renderer->depthImage.allocation, NULL);

	vmaBindImageMemory(renderer->memoryManager->allocator, renderer->depthImage.allocation, renderer->depthImage.image);

	vmaAllocateMemoryForImage(renderer->memoryManager->allocator, renderer->renderImage.image, &allocInfo, &renderer->renderImage.allocation, NULL);

	vmaBindImageMemory(renderer->memoryManager->allocator, renderer->renderImage.allocation, renderer->renderImage.image);

	// create image view

	VkImageViewCreateInfo viewCreateInfo;
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.pNext = NULL;
	viewCreateInfo.flags = 0;
	viewCreateInfo.image = renderer->depthImage.image;
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCreateInfo.format = depthFmt;

	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	//viewCreateInfo.subresourceRange.aspectMask = (isDepthFormat(info->pixelFormat)) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
	viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	viewCreateInfo.subresourceRange.baseMipLevel = 0;
	viewCreateInfo.subresourceRange.levelCount = 1;
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
	viewCreateInfo.subresourceRange.layerCount = 1;
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

	VkSamplerCreateInfo samplerInfo;
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.pNext = NULL;
	samplerInfo.flags = 0;

	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.magFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 0;

	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0;
	samplerInfo.minLod = VK_LOD_CLAMP_NONE;
	samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
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

	VkImageCreateInfo imgCreateInfo;
	imgCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgCreateInfo.pNext = NULL;
	imgCreateInfo.flags = 0;
	imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imgCreateInfo.extent.width = result->width;
	imgCreateInfo.extent.height = result->height;
	imgCreateInfo.extent.depth = 1;
	imgCreateInfo.mipLevels = 1;
	imgCreateInfo.arrayLayers = 1;

	imgCreateInfo.format = result->pixelFormat;
	imgCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;	// FIXME: ? causes issue if when trying to display before update + mipmap generation
	imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imgCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	// TODO: (probably) from info??

	imgCreateInfo.samples = 1;

	imgCreateInfo.queueFamilyIndexCount = 0;
	imgCreateInfo.pQueueFamilyIndices = NULL;

	VkResult r = vkCreateImage(renderer->device->device, &imgCreateInfo, NULL, &(result->image));
	if (r != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create texture!\n");
		return r;
	}

	// allocate memory

	VmaAllocationCreateInfo allocInfo;
	allocInfo.flags = 0;
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocInfo.requiredFlags = 0;
	allocInfo.preferredFlags = 0;
	allocInfo.memoryTypeBits = 0;
	allocInfo.pool = VK_NULL_HANDLE;
	allocInfo.pUserData = NULL;
	allocInfo.priority = 0.0f;

	vmaAllocateMemoryForImage(renderer->memoryManager->allocator, result->image, &allocInfo, &result->allocation, NULL);

	vmaBindImageMemory(renderer->memoryManager->allocator, result->allocation, result->image);

	// create image view

	VkImageViewCreateInfo viewCreateInfo;
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.pNext = NULL;
	viewCreateInfo.flags = 0;
	viewCreateInfo.image = result->image;
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCreateInfo.format = result->pixelFormat;

	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewCreateInfo.subresourceRange.baseMipLevel = 0;
	viewCreateInfo.subresourceRange.levelCount = 1;
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
	viewCreateInfo.subresourceRange.layerCount = 1;

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

	VkSamplerCreateInfo samplerInfo;
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.pNext = NULL;
	samplerInfo.flags = 0;

	samplerInfo.minFilter = minMagFilter;
	samplerInfo.magFilter = minMagFilter;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 0;

	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;

	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0;
	samplerInfo.minLod = 0;
	samplerInfo.maxLod = VK_LOD_CLAMP_NONE;

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

	VkImageMemoryBarrier imgMemBarrier;
	imgMemBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imgMemBarrier.pNext = NULL;
	imgMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.image = img->image;
	imgMemBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imgMemBarrier.subresourceRange.baseMipLevel = 0;
	imgMemBarrier.subresourceRange.levelCount = 1;
	imgMemBarrier.subresourceRange.baseArrayLayer = 0;
	imgMemBarrier.subresourceRange.layerCount = 1;

	imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imgMemBarrier.srcAccessMask = 0;
	imgMemBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);

	// end layout change

	// start image copy

	VkBufferImageCopy copyRegion;
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;
	copyRegion.bufferImageHeight = 0;

	copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;

	copyRegion.imageOffset.x = 0;
	copyRegion.imageOffset.y = 0;
	copyRegion.imageOffset.z = 0;

	copyRegion.imageExtent.width = width;
	copyRegion.imageExtent.height = height;
	copyRegion.imageExtent.depth = 1;

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
	pendingImageWrite imageWrite;
	imageWrite.cmdbuf = cmdbuf;
	imageWrite.transferbuf = transferBuffer;

	sb_push_back(renderer->pendingImageWrites, &imageWrite);
}

inline int32_t i32max(int32_t a, int32_t b) {
	return (a > b) ? a : b;
}
