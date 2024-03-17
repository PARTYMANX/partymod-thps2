#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <gfx/vk/vk.h>

typedef struct textureRenderer_s {
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMem;
} textureCache_t;

textureCache_t *initTextures(partyRenderer *renderer) {
	textureCache_t *result = malloc(sizeof(textureCache_t));

	// is this used?  TODO: remove me
}

uint8_t isDepthFormat(VkFormat format) {
	return (format == VK_FORMAT_D16_UNORM || 
		format == VK_FORMAT_D16_UNORM_S8_UINT || 
		format == VK_FORMAT_D24_UNORM_S8_UINT || 
		format == VK_FORMAT_D32_SFLOAT ||
		format == VK_FORMAT_D32_SFLOAT_S8_UINT);
}

/*VkImageUsageFlags decodeUsage(vid2_textureUsage_t usage, uint8_t isDepth) {
	VkImageUsageFlags result = 0;

	if (usage & VID2_TEXTURE_USAGE_SHADER_READ)
		result |= VK_IMAGE_USAGE_SAMPLED_BIT;
	if (usage & VID2_TEXTURE_USAGE_SHADER_WRITE)
		result |= VK_IMAGE_USAGE_STORAGE_BIT;
	if (usage & VID2_TEXTURE_USAGE_RENDER_TARGET) {
		result |= (isDepth) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}

	return result;
}*/

void createRenderTargets(partyRenderer *renderer, uint32_t width, uint32_t height, VkFormat colorFmt, VkFormat depthFmt) {
	printf("CREATING RENDER TARGETS\n");

	renderer->depthImage.type = VK_IMAGE_TYPE_2D;
	renderer->depthImage.pixelFormat = depthFmt;

	renderer->depthImage.width = width;
	renderer->depthImage.height = height;
	renderer->depthImage.depth = 1;

	renderer->depthImage.mipmapCount = 0;
	renderer->depthImage.arrayLength = 0;
	renderer->depthImage.sampleCount = 1;

	renderer->renderImage.type = VK_IMAGE_TYPE_2D;
	renderer->renderImage.pixelFormat = depthFmt;

	renderer->renderImage.width = width;
	renderer->renderImage.height = height;
	renderer->renderImage.depth = 1;

	renderer->renderImage.mipmapCount = 0;
	renderer->renderImage.arrayLength = 0;
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

	if (vkCreateImage(renderer->device->device, &imgCreateInfo, NULL, &(renderer->depthImage.image)) != VK_SUCCESS) {
		printf("Failed to create depth texture!\n");
		exit(1);
	}

	imgCreateInfo.format = colorFmt;
	imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	if (vkCreateImage(renderer->device->device, &imgCreateInfo, NULL, &(renderer->renderImage.image)) != VK_SUCCESS) {
		printf("Failed to create render texture!\n");
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

	if(vkCreateImageView(renderer->device->device, &viewCreateInfo, NULL, &(renderer->depthImage.imageView)) != VK_SUCCESS) {
		printf("Failed to create depth image view");
		exit(1);
	}

	viewCreateInfo.image = renderer->renderImage.image;
	viewCreateInfo.format = colorFmt;
	viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	if(vkCreateImageView(renderer->device->device, &viewCreateInfo, NULL, &(renderer->renderImage.imageView)) != VK_SUCCESS) {
		printf("Failed to create render image view");
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

	if(vkCreateSampler(renderer->device->device, &samplerInfo, NULL, &(renderer->renderSampler)) != VK_SUCCESS) {
		printf("Failed to create sampler");
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

/*vid2_texture_t *vid2_createTexture(vid2_renderer_t *renderer, vid2_textureInfo_t *info) {
	rbVkImage *result = malloc(sizeof(rbVkImage));

	result->type = info->type;
	result->pixelFormat = info->pixelFormat;

	result->width = info->width;
	result->height = info->height;
	result->depth = info->depth;

	result->mipmapCount = info->mipmapCount;
	result->arrayLength = info->arrayLength;
	result->sampleCount = info->sampleCount;

	result->flags = 0;

	VkImageCreateInfo imgCreateInfo;
	imgCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgCreateInfo.pNext = NULL;
	imgCreateInfo.flags = 0;
	imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imgCreateInfo.extent.width = info->width;
	imgCreateInfo.extent.height = info->height;
	imgCreateInfo.extent.depth = info->depth;
	imgCreateInfo.mipLevels = info->mipmapCount;
	imgCreateInfo.arrayLayers = info->arrayLength;

	imgCreateInfo.format = pixelFormatLUT[info->pixelFormat];
	imgCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;	// FIXME: ? causes issue if when trying to display before update + mipmap generation
	imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | decodeUsage(info->usage, isDepthFormat(info->pixelFormat));
	imgCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	// TODO: (probably) from info??

	imgCreateInfo.samples = info->sampleCount;

	imgCreateInfo.queueFamilyIndexCount = 0;
	imgCreateInfo.pQueueFamilyIndices = NULL;

	if (vkCreateImage(renderer->device->device, &imgCreateInfo, NULL, &(result->image)) != VK_SUCCESS) {
		printf("Failed to create texture!\n");
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

	vmaAllocateMemoryForImage(renderer->memoryManager->allocator, result->image, &allocInfo, &result->allocation, NULL);

	vmaBindImageMemory(renderer->memoryManager->allocator, result->allocation, result->image);

	// create image view

	VkImageViewCreateInfo viewCreateInfo;
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.pNext = NULL;
	viewCreateInfo.flags = 0;
	viewCreateInfo.image = result->image;
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCreateInfo.format = pixelFormatLUT[info->pixelFormat];

	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	viewCreateInfo.subresourceRange.aspectMask = (isDepthFormat(info->pixelFormat)) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
	viewCreateInfo.subresourceRange.baseMipLevel = 0;
	viewCreateInfo.subresourceRange.levelCount = info->mipmapCount;
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;
	viewCreateInfo.subresourceRange.layerCount = info->arrayLength;

	if(vkCreateImageView(renderer->device->device, &viewCreateInfo, NULL, &(result->imageView)) != VK_SUCCESS) {
		printf("Failed to create image view");
		exit(1);
	}

	return result;
}*/

void vid2_destroyTexture(partyRenderer *renderer, rbVkImage *img) {
	vkDestroyImageView(renderer->device->device, img->imageView, NULL);
	vkDestroyImage(renderer->device->device, img->image, NULL);
	vmaFreeMemory(renderer->memoryManager->allocator, img->allocation);
	free(img);
}

/*vid2_sampler_t *vid2_createSampler(vid2_renderer_t *renderer, vid2_samplerInfo_t *info) {
	vid2_sampler_t *result = malloc(sizeof(vid2_sampler_t));

	result->renderer = renderer;

	VkSamplerCreateInfo samplerInfo;
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.pNext = NULL;
	samplerInfo.flags = 0;

	samplerInfo.minFilter = filterLUT[info->minFilter];
	samplerInfo.magFilter = filterLUT[info->magFilter];

	samplerInfo.addressModeU = addressModeLUT[info->uAddressMode];
	samplerInfo.addressModeV = addressModeLUT[info->vAddressMode];
	samplerInfo.addressModeW = addressModeLUT[info->wAddressMode];

	samplerInfo.anisotropyEnable = (info->maxAnisotropy != 1.0f) ? VK_TRUE : VK_FALSE;
	samplerInfo.maxAnisotropy = info->maxAnisotropy;

	samplerInfo.borderColor = borderColorLUT[info->borderColor];

	samplerInfo.unnormalizedCoordinates = (info->unnormalizedCoordinates) ? VK_TRUE : VK_FALSE;

	samplerInfo.compareEnable = info->compareEnable;
	samplerInfo.compareOp = compareOpLUT[info->compareOp];

	samplerInfo.mipmapMode = mipmapModeLUT[info->mipmapMode];
	samplerInfo.mipLodBias = info->mipmapLodBias;
	samplerInfo.minLod = info->minLod;
	samplerInfo.maxLod = (info->maxLod == VID2_LOD_CLAMP_NONE) ? VK_LOD_CLAMP_NONE : info->maxLod;

	if(vkCreateSampler(renderer->device->device, &samplerInfo, NULL, &(result->sampler)) != VK_SUCCESS) {
		printf("Failed to create sampler");
		exit(1);
	}

	return result;
}*/

/*void vid2_destroySampler(vid2_sampler_t *sampler) {
	vkDestroySampler(sampler->renderer->device->device, sampler->sampler, NULL);
	free(sampler);
}

void transitionImageLayout(VkCommandBuffer cmdbuf, vid2_texture_t *img, vid2_textureCopyInfo *info) {
	
}*/

/*void vid2_updateTexture(vid2_texture_t *img, vid2_textureCopyInfo *info) {

	VkDeviceSize sz = info->bytesPerRow * info->region.to[1] * info->region.to[2];

	vid2_buffer_t *buffer = createStagingBuffer(img->renderer, sz);

	void *buf = mapMemory((buffer->memory));
	memcpy(buf, info->data, sz);
	unmapMemory((buffer->memory));

	VkCommandBuffer cmdbuf = startStagingCommandBuffer(img->renderer);

	//IMAGE COPY GOES HERE

	// start layout change

	VkImageMemoryBarrier imgMemBarrier;
	imgMemBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imgMemBarrier.pNext = NULL;
	imgMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.image = img->image;
	imgMemBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imgMemBarrier.subresourceRange.baseMipLevel = info->mipLevel;
	imgMemBarrier.subresourceRange.levelCount = 1;
	imgMemBarrier.subresourceRange.baseArrayLayer = info->baseArrayLayer;
	imgMemBarrier.subresourceRange.layerCount = info->layerCount;

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
	copyRegion.imageSubresource.mipLevel = info->mipLevel;
	copyRegion.imageSubresource.baseArrayLayer = info->baseArrayLayer;
	copyRegion.imageSubresource.layerCount = info->layerCount;

	copyRegion.imageOffset.x = info->region.from[0];
	copyRegion.imageOffset.y = info->region.from[1];
	copyRegion.imageOffset.z = info->region.from[2];

	copyRegion.imageExtent.width = info->region.to[0];
	copyRegion.imageExtent.height = info->region.to[1];
	copyRegion.imageExtent.depth = info->region.to[2];

	vkCmdCopyBufferToImage(cmdbuf, buffer->buffer, img->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

	// end image copy
	
	// start layout change

	imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imgMemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imgMemBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);

	// end layout change

	//END IMAGE COPY

	endStagingCommandBuffer(img->renderer, cmdbuf);

	vid2_destroyBuffer(buffer);
}*/

inline int32_t i32max(int32_t a, int32_t b) {
	return (a > b) ? a : b;
}

/*void vid2_generateMipmaps(vid2_texture_t *tex) {
	//vid2_buffer_t *buffer = createStagingBuffer(tex->renderer, sz);

	VkCommandBuffer cmdbuf = startStagingCommandBuffer(tex->renderer);

	VkImageMemoryBarrier imgMemBarrier;
	imgMemBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imgMemBarrier.pNext = NULL;
	imgMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.image = tex->image;
	imgMemBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imgMemBarrier.subresourceRange.baseArrayLayer = 0;
	imgMemBarrier.subresourceRange.layerCount = 1;

	imgMemBarrier.subresourceRange.baseMipLevel = 0;
	imgMemBarrier.subresourceRange.levelCount = tex->mipmapCount;

	imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imgMemBarrier.srcAccessMask = 0;
	imgMemBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

	vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);
	
	for (int i = 1; i < tex->mipmapCount; i++) {
		// start layout change

		imgMemBarrier.subresourceRange.baseMipLevel = i - 1;
		imgMemBarrier.subresourceRange.levelCount = 1;

		imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		imgMemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imgMemBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);

		// end layout change

		// start blit

		int32_t srcWidth = i32max(tex->width >> (i - 1), 1);
		int32_t srcHeight = i32max(tex->height >> (i - 1), 1);

		int32_t dstWidth = i32max(tex->width >> i, 1);
		int32_t dstHeight = i32max(tex->height >> i, 1);

		VkImageBlit blit;
		blit.srcOffsets[0] = (VkOffset3D){ 0, 0, 0 };
		blit.srcOffsets[1] = (VkOffset3D){ srcWidth, srcHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = (VkOffset3D){ 0, 0, 0 };
		blit.dstOffsets[1] = (VkOffset3D){ dstWidth, dstHeight, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(cmdbuf, tex->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, tex->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

		// end blit

		// start layout change

		imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imgMemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		imgMemBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);

		// end layout change
	}

	imgMemBarrier.subresourceRange.baseMipLevel = tex->mipmapCount - 1;
	imgMemBarrier.subresourceRange.levelCount = 1;

	imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imgMemBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imgMemBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(cmdbuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);

	endStagingCommandBuffer(tex->renderer, cmdbuf);

	//vid2_destroyBuffer(buffer);
}*/

/*uint32_t vid2_getTextureWidth(vid2_texture_t *tex) {
	return tex->width;
}

uint32_t vid2_getTextureHeight(vid2_texture_t *tex) {
	return tex->height;
}*/
