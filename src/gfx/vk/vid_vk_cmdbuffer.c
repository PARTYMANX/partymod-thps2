#include <stdlib.h>
#include <stdint.h>

#include <vulkan/vulkan.h>

//#include "hash.h"
//#include "handle.h"
//#include "vid.h"
//#include "vid_vk.h"

#include <gfx/vk/vk.h>

/*
	QUEUE
*/

VkResult rbVkCreateCommandQueue(struct rbVkDevice *device, VkCommandPoolCreateFlags flags, struct rbVkCommandQueue **queue) {
	struct rbVkCommandQueue *result = malloc(sizeof(struct rbVkCommandQueue));

	if (!result) {
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	*queue = NULL;

	result->device = device;

	vkGetDeviceQueue(device->device, device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_GRAPHICS], 0, &(result->queue));
	vkGetDeviceQueue(device->device, device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_PRESENT], 0, &(result->presentQueue));

	VkCommandPoolCreateInfo commandPoolInfo;
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.pNext = NULL;
	commandPoolInfo.flags = flags;
	commandPoolInfo.queueFamilyIndex = device->physicalDevice.queueFamilyIdxs[QUEUE_FAMILY_GRAPHICS];

	VkResult r = vkCreateCommandPool(device->device, &commandPoolInfo, NULL, &(result->commandPool));
	if (r) {
		printf("ERROR: Failed to create device command pool!");
		return r;
	}

	*queue = result;

	return r;
}

void rbVkDestroyCommandQueue(struct rbVkCommandQueue *queue) {
	//vkFreeCommandBuffers(queue->device->device, queue->commandPool, VID2_VULKAN_COMMAND_BUFFER_COUNT, queue->commandBuffers);
	vkDestroyCommandPool(queue->device->device, queue->commandPool, NULL);
	free(queue);
}

/*
	COMMAND BUFFERS
*/

VkResult createRenderCommandBuffer(partyRenderer *renderer) {
	VkCommandBufferAllocateInfo bufferAllocateInfo;
	bufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	bufferAllocateInfo.pNext = NULL;
	bufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	bufferAllocateInfo.commandPool = renderer->queue->commandPool;
	bufferAllocateInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(renderer->device->device, &bufferAllocateInfo, &(renderer->renderCommandBuffer)) != VK_SUCCESS) {
		printf("ERROR: Failed to create render command buffer!");
		exit(1);
	}
}

void destroyRenderCommandBuffer(partyRenderer *renderer) {
	vkFreeCommandBuffers(renderer->device->device, renderer->queue->commandPool, 1, &(renderer->renderCommandBuffer));
}

VkCommandBuffer startStagingCommandBuffer(partyRenderer *renderer) {
	VkCommandBufferAllocateInfo cmdbufAllocInfo;
	cmdbufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdbufAllocInfo.pNext = NULL;
	cmdbufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdbufAllocInfo.commandPool = renderer->memQueue->commandPool;
	cmdbufAllocInfo.commandBufferCount = 1;

	VkCommandBuffer cmdbuf;
	vkAllocateCommandBuffers(renderer->device->device, &cmdbufAllocInfo, &cmdbuf);

	VkCommandBufferBeginInfo cmdbufBeginInfo;
	cmdbufBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdbufBeginInfo.pNext = NULL;
	cmdbufBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	cmdbufBeginInfo.pInheritanceInfo = NULL;

	vkBeginCommandBuffer(cmdbuf, &cmdbufBeginInfo);

	return cmdbuf;
}

void endStagingCommandBuffer(partyRenderer *renderer, VkCommandBuffer cmdbuf) {
	vkEndCommandBuffer(cmdbuf);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdbuf;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = NULL;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = NULL;
	submitInfo.pWaitDstStageMask = NULL;

	vkQueueSubmit(renderer->memQueue->queue, 1, &submitInfo, VK_NULL_HANDLE);
	//vkQueueWaitIdle(renderer->memQueue->queue);	// TODO: maybe make a queue of sorts of the command buffers to free?  lets us load faster

	//vkFreeCommandBuffers(renderer->device->device, renderer->memQueue->commandPool, 1, &cmdbuf);
}

/*commandBuffer_t createCommandBuffer(vid2_renderer_t *renderer) {
	commandBuffer_t buffer;

	//buffer.bufferCount = VID2_VULKAN_COMMAND_BUFFER_COUNT;

	//buffer.commandBuffers = malloc(sizeof(VkCommandBuffer) * VID2_VULKAN_COMMAND_BUFFER_COUNT);

	VkCommandBufferAllocateInfo bufferAllocateInfo;
	bufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	bufferAllocateInfo.pNext = NULL;
	bufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	bufferAllocateInfo.commandPool = renderer->queue->commandPool;
	bufferAllocateInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(renderer->device->device, &bufferAllocateInfo, &(buffer.commandBuffer)) != VK_SUCCESS) {
		printf("ERROR: Failed to create command buffers!");
		exit(1);
	}

	buffer.dirty = 1;	// TODO: is this still used?

	return buffer;
}

vid2_commandBuffer_t *vid2_createCommandBuffer(vid2_renderer_t *renderer) {
	vid2_commandBuffer_t *result = malloc(sizeof(vid2_commandBuffer_t));

	result->renderer = renderer;

	commandBuffer_t buffer = createCommandBuffer(renderer);

	result->hdl = handleListAdd(renderer->commandBuffers, &buffer);

	VkPhysicalDeviceProperties props;

	return result;
}

void vid2_destroyCommandBuffer(vid2_commandBuffer_t *buffer) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);
	vkFreeCommandBuffers(buffer->renderer->device->device, buffer->renderer->queue->commandPool, 1, &(inBuffer->commandBuffer));
	handleListRemove(buffer->renderer->commandBuffers, buffer->hdl);
	free(buffer);
}

void vid2_beginCommandBuffer(vid2_commandBuffer_t *buffer) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = NULL;

	if (vkBeginCommandBuffer(inBuffer->commandBuffer, &beginInfo) != VK_SUCCESS) {
		printf("ERROR: Failed to begin command buffers!");
		exit(1);
	}

	inBuffer->bindState.head = NULL;
}

void vid2_endCommandBuffer(vid2_commandBuffer_t *buffer) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	if (vkEndCommandBuffer(inBuffer->commandBuffer) != VK_SUCCESS) {
		printf("ERROR: Failed to end command buffers!");
		exit(1);
	}
}

void vid2_submitCommandBuffer(vid2_renderer_t *renderer, vid2_commandBuffer_t *buffer) {
	// IDEA: use a "present" command to move swapchain data to queue for execution time?

	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = NULL;

	// TODO: store all command buffers in array?
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &(inBuffer->commandBuffer);

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &(renderer->swapchain->imageReadySemaphore);

	VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submitInfo.pWaitDstStageMask = &waitStages;

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &(renderer->swapchain->imageFinishedSemaphore);

	if (vkQueueSubmit(renderer->queue->queue, 1, &submitInfo, renderer->swapchain->fence) != VK_SUCCESS) {
		printf("ERROR: Failed to submit command queue!");
		exit(1);
	}

	//renderer->swapchain->fenceUsed = 1;

	freeBindState(&(inBuffer->bindState));
}*/

/*
	COMMANDS
*/

// FIXME: should probably not have side effects.  explore vulkan 1.3 support
/*void vid2_cmdBeginRenderPass(vid2_commandBuffer_t *buffer, vid2_renderPassBeginInfo_t info) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);
	//framebuffer_t *inFramebuffer = pHandleListGet(info.framebuffer->renderer->framebuffers, info.framebuffer->hdl, 0);

	VkRenderPassBeginInfo rpInfo;
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpInfo.pNext = NULL;
	rpInfo.renderPass = vid2_getRenderPass(buffer->renderer->renderPassCache, &info);

	framebufferInfo_t fbInfo;
	fbInfo.renderPass = rpInfo.renderPass;
	fbInfo.attachmentCount = (info.pDepthStencilAttachment != NULL) ? info.colorAttachmentCount + 1 : info.colorAttachmentCount;
	if (info.pResolveAttachments) {
		fbInfo.attachmentCount += info.colorAttachmentCount;
	}
	fbInfo.width = info.width;
	fbInfo.height = info.height;
	fbInfo.layers = info.layers;
	
	vid2_attachment_t *attachments = malloc(sizeof(vid2_attachment_t) * (fbInfo.attachmentCount));
	for (int i = 0; i < info.colorAttachmentCount; i++) {
		attachments[i] = info.pColorAttachments[i];
	}
	if (info.pResolveAttachments) {
		for (int i = info.colorAttachmentCount; i < info.colorAttachmentCount * 2; i++) {
			attachments[i] = info.pResolveAttachments[i - info.colorAttachmentCount];
		}
	}
	if (info.pDepthStencilAttachment != NULL) {
		attachments[fbInfo.attachmentCount - 1] = *(info.pDepthStencilAttachment);
	}

	fbInfo.attachments = attachments;

	framebuffer_t fb = vid2_getFramebuffer(buffer->renderer->framebufferCache, &fbInfo);

	rpInfo.renderArea.offset = (VkOffset2D) { 0, 0 };
	rpInfo.renderArea.extent = (VkExtent2D) { fb.width, fb.height };

	rpInfo.clearValueCount = (info.pDepthStencilAttachment != NULL) ? info.colorAttachmentCount + 1 : info.colorAttachmentCount;	// FIXME: why wouldn't this be the same as color attachment count?
	if (info.pResolveAttachments) {
		rpInfo.clearValueCount += info.colorAttachmentCount;
	}

	VkClearValue *clearValues = malloc(sizeof(VkClearValue) * (rpInfo.clearValueCount));
	for (int i = 0; i < info.colorAttachmentCount; i++) {
		clearValues[i] = (VkClearValue) {.color.float32 = {
			info.pColorAttachments[i].clearValue.color.red, 
			info.pColorAttachments[i].clearValue.color.green, 
			info.pColorAttachments[i].clearValue.color.blue, 
			info.pColorAttachments[i].clearValue.color.alpha
		}};
	}
	if (info.pResolveAttachments) {
		for (int i = info.colorAttachmentCount; i < info.colorAttachmentCount * 2; i++) {
			clearValues[i] = (VkClearValue) {.color.float32 = {
				info.pResolveAttachments[i - info.colorAttachmentCount].clearValue.color.red, 
				info.pResolveAttachments[i - info.colorAttachmentCount].clearValue.color.green, 
				info.pResolveAttachments[i - info.colorAttachmentCount].clearValue.color.blue, 
				info.pResolveAttachments[i - info.colorAttachmentCount].clearValue.color.alpha
			}};
		}
	}
	if (info.pDepthStencilAttachment != NULL) {
		clearValues[rpInfo.clearValueCount - 1] = (VkClearValue) {.color.float32 = {
			info.pDepthStencilAttachment->clearValue.color.red, 
			info.pDepthStencilAttachment->clearValue.color.green, 
			info.pDepthStencilAttachment->clearValue.color.blue, 
			info.pDepthStencilAttachment->clearValue.color.alpha
		}};
	}

	rpInfo.pClearValues = clearValues;

	// viewport is flipped to correct for y coordinate in NDC space
	VkViewport viewport;
	viewport.x = 0;
	viewport.y = fb.height;
	viewport.width = ((float)fb.width);
	viewport.height = -((float)fb.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = fb.width;
	scissor.extent.height = fb.height;

	rpInfo.framebuffer = fb.framebuffer;

	vkCmdBeginRenderPass(inBuffer->commandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdSetViewport(inBuffer->commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(inBuffer->commandBuffer, 0, 1, &scissor);

	free(attachments);
	free(clearValues);
}

void vid2_cmdEndRenderPass(vid2_commandBuffer_t *buffer) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	vkCmdEndRenderPass(inBuffer->commandBuffer);
}

void vid2_cmdSetViewport(vid2_commandBuffer_t *buffer, vid2_viewport_t *viewport) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	// viewport is flipped to correct for y coordinate in NDC space
	VkViewport cmdviewport;
	cmdviewport.x = viewport->x;
	cmdviewport.y = viewport->y;
	cmdviewport.width = viewport->width;
	cmdviewport.height = -viewport->height;
	cmdviewport.minDepth = viewport->minDepth;
	cmdviewport.maxDepth = viewport->maxDepth;

	vkCmdSetViewport(inBuffer->commandBuffer, 0, 1, &cmdviewport);
}

void vid2_cmdSetScissor(vid2_commandBuffer_t *buffer, vid2_rect2D_t *scissor) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	VkRect2D cmdscissor;
	cmdscissor.offset.x = scissor->offset.x;
	cmdscissor.offset.y = scissor->offset.y;
	cmdscissor.extent.width = scissor->extent.width;
	cmdscissor.extent.height = scissor->extent.height;

	vkCmdSetScissor(inBuffer->commandBuffer, 0, 1, &cmdscissor);
}

void vid2_cmdBindPipeline(vid2_commandBuffer_t *buffer, vid2_pipeline_t *pipeline) {
	commandBuffer_t *inBuffer = handleListGet(buffer->renderer->commandBuffers, buffer->hdl);
	pipeline_t *inPipeline = handleListGet(pipeline->cache->pipelines, pipeline->hdl);

	inBuffer->boundPipeline = *pipeline;

	vkCmdBindPipeline(inBuffer->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, inPipeline->pipeline);
}

void vid2_cmdBindVertexBuffer(vid2_commandBuffer_t *buffer, vid2_buffer_t *vbuffer, size_t offset, uint32_t idx) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	VkBuffer bufferArr[1] = {vbuffer->buffer};
	VkDeviceSize offsetArr[1] = {offset};

	vkCmdBindVertexBuffers(inBuffer->commandBuffer, idx, 1, bufferArr, offsetArr);
}

void vid2_cmdBindIndexBuffer(vid2_commandBuffer_t *buffer, vid2_buffer_t *idxbuffer, size_t offset, uint32_t idx) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	vkCmdBindIndexBuffer(inBuffer->commandBuffer, idxbuffer->buffer, offset, VK_INDEX_TYPE_UINT32);
}

void vid2_cmdBindUniformBuffer(vid2_commandBuffer_t *buffer, vid2_buffer_t *unibuffer, uint32_t binding) {
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	setUniformBindState(&(inBuffer->bindState), binding, unibuffer);

	//for (int i = 0; i < inBuffer->bufferCount; i++) {
	//	vkCmdBindIndexBuffer(inBuffer->commandBuffers[i], idxbuffer->buffer, offset, VK_INDEX_TYPE_UINT32);
	//}
}*/

//void vid2_cmdBindTexture(vid2_commandBuffer_t *buffer, vid2_texture_t *texture, vid2_sampler_t *sampler, uint32_t binding) {
	//commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	/*VkImageMemoryBarrier imgMemBarrier;
	imgMemBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imgMemBarrier.pNext = NULL;
	imgMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imgMemBarrier.image = texture->image;
	imgMemBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imgMemBarrier.subresourceRange.baseArrayLayer = 0;
	imgMemBarrier.subresourceRange.layerCount = 1;	// TODO: get this

	imgMemBarrier.subresourceRange.baseMipLevel = 0;
	imgMemBarrier.subresourceRange.levelCount = texture->mipmapCount;

	imgMemBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imgMemBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imgMemBarrier.srcAccessMask = 0;
	imgMemBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

	vkCmdPipelineBarrier(inBuffer->commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imgMemBarrier);*/

	//setSamplerBindState(&(inBuffer->bindState), binding, texture, sampler);

	//for (int i = 0; i < inBuffer->bufferCount; i++) {
	//	vkCmdBindIndexBuffer(inBuffer->commandBuffers[i], idxbuffer->buffer, offset, VK_INDEX_TYPE_UINT32);
	//}
//}

/*void preDraw(vid2_commandBuffer_t *buffer) {
	bindDescriptorSetState(buffer);
}

void vid2_cmdDraw(vid2_commandBuffer_t *buffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t offset, uint32_t instanceOffset) {
	preDraw(buffer);

	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	vkCmdDraw(inBuffer->commandBuffer, vertexCount, instanceCount, offset, instanceOffset);
}

void vid2_cmdDrawIndexed(vid2_commandBuffer_t *buffer, uint32_t indexCount, uint32_t instanceCount, uint32_t offset, int32_t vertexOffset, uint32_t instanceOffset) {
	preDraw(buffer);
	
	commandBuffer_t *inBuffer = (commandBuffer_t *)handleListGet(buffer->renderer->commandBuffers, buffer->hdl);

	vkCmdDrawIndexed(inBuffer->commandBuffer, indexCount, instanceCount, offset, vertexOffset, instanceOffset);
}*/