#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <gfx/vk/vk.h>
#include <log.h>

VkDescriptorPool createPool(partyRenderer *renderer, pmVkDescriptorAllocator *allocator) {
	VkDescriptorPoolSize *sizes = malloc(sizeof(VkDescriptorPoolSize) * allocator->ratios->count);
	for (int i = 0; i < allocator->ratios->count; i++) {
		struct PoolSizeRatio *ratios = ((struct PoolSizeRatio *)allocator->ratios->data);
		sizes[i].type = ratios[i].type;
		sizes[i].descriptorCount = ratios[i].ratio * allocator->sets_per_pool;
	}
	
	VkDescriptorPoolCreateInfo poolInfo;
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.pNext = NULL;
	poolInfo.flags = 0;
	poolInfo.maxSets = allocator->sets_per_pool;
	poolInfo.poolSizeCount = allocator->ratios->count;
	poolInfo.pPoolSizes = sizes;

	VkDescriptorPool result;

	if (vkCreateDescriptorPool(renderer->device->device, &poolInfo, NULL, &result) != VK_SUCCESS) {
		// error, do something
	}
	
	free(sizes);

	return result;
}

VkDescriptorPool getPool(partyRenderer *renderer, pmVkDescriptorAllocator *allocator) {
	VkDescriptorPool result;
	if (allocator->ready_pools->count > 0) {
		sb_pop(allocator->ready_pools, &result);
	} else {
		result = createPool(renderer, allocator);

		allocator->sets_per_pool = allocator->sets_per_pool * 1.5;
		if (allocator->sets_per_pool > 4092) {
			allocator->sets_per_pool = 4092;
		}
	}

	return result;
}

pmVkDescriptorAllocator *init_descriptors(partyRenderer *renderer, uint32_t max_sets, struct PoolSizeRatio *poolRatios, size_t poolRatioCount) {
	pmVkDescriptorAllocator *result = malloc(sizeof(pmVkDescriptorAllocator));

	result->ratios = sb_alloc(sizeof(struct PoolSizeRatio), 8);
	result->full_pools = sb_alloc(sizeof(VkDescriptorPool), 8);
	result->ready_pools = sb_alloc(sizeof(VkDescriptorPool), 8);
	result->writes = sb_alloc(sizeof(VkWriteDescriptorSet), 8);

	for (int i = 0; i < poolRatioCount; i++) {
		sb_push_back(result->ratios, poolRatios + i);
	}

	result->sets_per_pool = max_sets;
	
	VkDescriptorPool pool = createPool(renderer, result);

	result->sets_per_pool = max_sets * 1.5;

	sb_push_back(result->ready_pools, &pool);

	log_printf(LL_DEBUG, "Descriptor pool ready!\n");

	return result;
}

void clear_descriptor_pools(partyRenderer *renderer, pmVkDescriptorAllocator *allocator) {
	VkDescriptorPool *ready_pools = allocator->ready_pools->data;
	for (int i = 0; i < allocator->ready_pools->count; i++) {
		vkResetDescriptorPool(renderer->device->device, ready_pools[i], 0);
	}

	while (allocator->full_pools->count) {
		VkDescriptorPool pool;
		sb_pop(allocator->full_pools, &pool);
		vkResetDescriptorPool(renderer->device->device, pool, 0);
		sb_push_back(allocator->ready_pools, &pool);
	}
}

void destroy_descriptor_pools(partyRenderer *renderer, pmVkDescriptorAllocator *allocator) {
	VkDescriptorPool *ready_pools = allocator->ready_pools->data;
	for (int i = 0; i < allocator->ready_pools->count; i++) {
		VkDescriptorPool pool;
		sb_pop(allocator->full_pools, &pool);
		vkDestroyDescriptorPool(renderer->device->device, pool, NULL);
	}

	while (allocator->full_pools->count) {
		VkDescriptorPool pool;
		sb_pop(allocator->full_pools, &pool);
		vkDestroyDescriptorPool(renderer->device->device, pool, 0);
	}
}

VkDescriptorSet allocate_descriptor_set(partyRenderer *renderer, pmVkDescriptorAllocator *allocator, VkDescriptorSetLayout layout) {
	VkDescriptorPool pool = getPool(renderer, allocator);

	VkDescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkDescriptorSet result;
	VkResult r = vkAllocateDescriptorSets(renderer->device->device, &allocInfo, &result);

	if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL) {
		sb_push_back(allocator->full_pools, &pool);

		pool = getPool(renderer, allocator);
		allocInfo.descriptorPool = pool;

		if (vkAllocateDescriptorSets(renderer->device->device, &allocInfo, &result) != VK_SUCCESS) {
			// error; do something
		}
	}

	sb_push_back(allocator->ready_pools, &pool);
	return result;
}

void write_descriptor_buffer(pmVkDescriptorAllocator *allocator, int binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type) {
	VkDescriptorBufferInfo *bufferInfo = malloc(sizeof(VkDescriptorBufferInfo));
	bufferInfo->buffer = buffer;
	bufferInfo->offset = offset;
	bufferInfo->range = size;

	VkWriteDescriptorSet write;
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = NULL;
	write.dstBinding = binding;
	write.dstSet = VK_NULL_HANDLE;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = type;
	write.pBufferInfo = bufferInfo;
	write.pImageInfo = NULL;
	write.pTexelBufferView = NULL;
	
	sb_push_back(allocator->writes, &write);
}

void write_descriptor_image(pmVkDescriptorAllocator *allocator, int binding, VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout, VkDescriptorType type) {
	VkDescriptorImageInfo *imageInfo = malloc(sizeof(VkDescriptorImageInfo));
	imageInfo->sampler = sampler;
	imageInfo->imageView = imageView;
	imageInfo->imageLayout = imageLayout;

	VkWriteDescriptorSet write;
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = NULL;
	write.dstBinding = binding;
	write.dstSet = VK_NULL_HANDLE;
	write.dstArrayElement = 0;
	write.descriptorCount = 1;
	write.descriptorType = type;
	write.pBufferInfo = NULL;
	write.pImageInfo = imageInfo;
	write.pTexelBufferView = NULL;
	
	sb_push_back(allocator->writes, &write);
}

void write_descriptor_image_array(pmVkDescriptorAllocator *allocator, int binding, int idx, VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout, VkDescriptorType type) {
	VkDescriptorImageInfo *imageInfo = malloc(sizeof(VkDescriptorImageInfo));
	imageInfo->sampler = sampler;
	imageInfo->imageView = imageView;
	imageInfo->imageLayout = imageLayout;

	VkWriteDescriptorSet write;
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.pNext = NULL;
	write.dstBinding = binding;
	write.dstSet = VK_NULL_HANDLE;
	write.dstArrayElement = idx;
	write.descriptorCount = 1;
	write.descriptorType = type;
	write.pBufferInfo = NULL;
	write.pImageInfo = imageInfo;
	write.pTexelBufferView = NULL;
	
	sb_push_back(allocator->writes, &write);
}

void update_set(partyRenderer *renderer, pmVkDescriptorAllocator *allocator, VkDescriptorSet set) {
	for (int i = 0; i < allocator->writes->count; i++) {
		((VkWriteDescriptorSet *)allocator->writes->data)[i].dstSet = set;
	}

	vkUpdateDescriptorSets(renderer->device->device, allocator->writes->count, ((VkWriteDescriptorSet *)allocator->writes->data), 0, NULL);
}

void clear_writes(pmVkDescriptorAllocator *allocator) {
	for (int i = 0; i < allocator->writes->count; i++) {
		VkWriteDescriptorSet write = ((VkWriteDescriptorSet *)allocator->writes->data)[i];

		if (write.pBufferInfo) {
			free(write.pBufferInfo);
		}

		if (write.pImageInfo) {
			free(write.pImageInfo);
		}
	}

	allocator->writes->count = 0;
}