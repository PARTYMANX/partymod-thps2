#include <stdlib.h>
#include <stdint.h>

#include <vulkan/vulkan.h>

#include <gfx/vk/vk.h>

// TODO: strip out all of the old handle stuff; we have the same problems on metal so we can probably surface it safely
// TODO: figure out what that meant

/*pipelineLayout_t createPipelineLayout(vid2_renderer_t *renderer, descriptorSetLayoutInfo_t *setLayout) {
	pipelineLayout_t result;

	result.setLayout = getDescriptorSetLayout(renderer, renderer->descriptorSetCache, setLayout);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = NULL;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &(result.setLayout.layout);
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = NULL;

	if (vkCreatePipelineLayout(renderer->device->device, &pipelineLayoutInfo, NULL, &(result.pipelineLayout)) != VK_SUCCESS) {
		printf("Failed to create pipeline layout!\n");
		exit(1);
	}

	

	return result;
}*/

// TODO: return handle to pipeline
VkResult createRenderPipelines(partyRenderer *renderer) {
	//pipeline_t pipeline;

	// shaders

	VkPipelineShaderStageCreateInfo shaderStageInfo[2];

	shaderStageInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo[0].pNext = NULL;
	shaderStageInfo[0].flags = 0;
	shaderStageInfo[0].pName = "main";
	shaderStageInfo[0].pSpecializationInfo = NULL;
	shaderStageInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfo[0].module = VK_NULL_HANDLE;
	
	shaderStageInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo[1].pNext = NULL;
	shaderStageInfo[1].flags = 0;
	shaderStageInfo[1].pName = "main";
	shaderStageInfo[1].pSpecializationInfo = NULL;
	shaderStageInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStageInfo[1].module = VK_NULL_HANDLE;

	// vertex input state

	VkVertexInputAttributeDescription vertexAttributeDesc[3];

	vertexAttributeDesc[0].binding = 0;
	vertexAttributeDesc[0].location = 0;
	vertexAttributeDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexAttributeDesc[0].offset = 0;

	vertexAttributeDesc[1].binding = 0;
	vertexAttributeDesc[1].location = 1;
	vertexAttributeDesc[1].format = VK_FORMAT_R32G32_SFLOAT;
	vertexAttributeDesc[1].offset = (sizeof(float) * 3);

	vertexAttributeDesc[2].binding = 0;
	vertexAttributeDesc[2].location = 2;
	vertexAttributeDesc[2].format = VK_FORMAT_R8G8B8A8_UNORM;
	vertexAttributeDesc[2].offset = (sizeof(float) * 3) + (sizeof(float) * 2);

	VkVertexInputBindingDescription vertexBindingDesc[1];

	vertexBindingDesc[0].binding = 0;
	vertexBindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexBindingDesc[0].stride = (sizeof(float) * 3) + (sizeof(float) * 2) + (sizeof(uint32_t));

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = NULL;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 3;
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDesc;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = vertexBindingDesc;

	// input assembly

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.pNext = NULL;
	inputAssemblyInfo.flags = 0;
	inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// viewport/scissor

	VkViewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = 0.0f;
	viewport.height = 0.0f;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = 0.0f;
	scissor.extent.height = 0.0f;

	VkPipelineViewportStateCreateInfo viewportInfo;
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.pNext = NULL;
	viewportInfo.flags = 0;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = &viewport;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = &scissor;

	// rasterizer

	VkPipelineRasterizationStateCreateInfo rasterizerInfo;
	rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerInfo.pNext = NULL;
	rasterizerInfo.flags = 0;
	rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerInfo.cullMode = VK_CULL_MODE_NONE;
	rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizerInfo.lineWidth = 1.0f;
	rasterizerInfo.depthBiasEnable = VK_FALSE;
	rasterizerInfo.depthBiasConstantFactor = 0.0f;
	rasterizerInfo.depthBiasSlopeFactor = 0.0f;
	rasterizerInfo.depthBiasClamp = 0.0f;
	rasterizerInfo.depthClampEnable = VK_FALSE;

	// multisampling

	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleInfo.pNext = NULL;
	multisampleInfo.flags = 0;
	multisampleInfo.sampleShadingEnable = VK_FALSE;
	multisampleInfo.rasterizationSamples = 1;
	multisampleInfo.minSampleShading = 1.0f;
	multisampleInfo.pSampleMask = NULL;
	multisampleInfo.alphaToCoverageEnable = VK_TRUE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;

	// depth-stencil

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.pNext = NULL;
	depthStencilInfo.flags = 0;
	depthStencilInfo.depthTestEnable = VK_FALSE;	// disable if compare op is ALWAYS?
	depthStencilInfo.depthWriteEnable = VK_TRUE;
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;

	depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	depthStencilInfo.minDepthBounds = 0.0f;
	depthStencilInfo.maxDepthBounds = 1.0f;

	depthStencilInfo.stencilTestEnable = VK_FALSE;

	depthStencilInfo.back.failOp = VK_STENCIL_OP_ZERO; 
	depthStencilInfo.back.passOp = VK_STENCIL_OP_ZERO;
	depthStencilInfo.back.depthFailOp = VK_STENCIL_OP_ZERO;
	depthStencilInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
	depthStencilInfo.back.compareMask = 0;
	depthStencilInfo.back.writeMask = 0;
	depthStencilInfo.back.reference = 0;

	depthStencilInfo.front.failOp = VK_STENCIL_OP_ZERO; 
	depthStencilInfo.front.passOp = VK_STENCIL_OP_ZERO;
	depthStencilInfo.front.depthFailOp = VK_STENCIL_OP_ZERO;
	depthStencilInfo.front.compareOp = VK_COMPARE_OP_ALWAYS;
	depthStencilInfo.front.compareMask = 0;
	depthStencilInfo.front.writeMask = 0;
	depthStencilInfo.front.reference = 0;

	// blending

	VkPipelineColorBlendAttachmentState attachmentBlend[1];

	attachmentBlend[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachmentBlend[0].blendEnable = VK_FALSE;
	attachmentBlend[0].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[0].colorBlendOp = VK_BLEND_OP_ADD;
	attachmentBlend[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[0].alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo blendInfo;
	blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendInfo.pNext = NULL;
	blendInfo.flags = 0;
	blendInfo.logicOpEnable = VK_FALSE;
	blendInfo.logicOp = VK_LOGIC_OP_COPY;
	blendInfo.attachmentCount = 1;
	blendInfo.pAttachments = attachmentBlend;
	blendInfo.blendConstants[0] = 0.0f;
	blendInfo.blendConstants[1] = 0.0f;
	blendInfo.blendConstants[2] = 0.0f;
	blendInfo.blendConstants[3] = 0.0f;

	// dynamic state

	VkDynamicState dynamicState[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_DEPTH_BIAS };

	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pNext = NULL;
	dynamicStateInfo.flags = 0;
	dynamicStateInfo.dynamicStateCount = 3;
	dynamicStateInfo.pDynamicStates = &dynamicState;

	// pipeline layout

	/*descriptorSetLayoutInfo_t layout;
	layout.numBindings = 0;
	layout.bindings = NULL;

	copyDescriptorSetLayout(&layout, &descriptor->vertexShader->descriptorSetLayout);
	copyDescriptorSetLayout(&layout, &descriptor->fragmentShader->descriptorSetLayout);
	pipeline.pipelineLayout = createPipelineLayout(renderer, &layout);

	//pipeline.pipelineLayout 

	free(layout.bindings);*/

	VkPipelineLayout layout;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = NULL;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = NULL;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = NULL;

	if (vkCreatePipelineLayout(renderer->device->device, &pipelineLayoutInfo, NULL, &(layout)) != VK_SUCCESS) {
		printf("Failed to create pipeline layout!\n");
		exit(1);
	}

	// finally, the pipeline

	VkGraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = NULL;
	pipelineInfo.flags = 0;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStageInfo;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &rasterizerInfo;
	pipelineInfo.pMultisampleState = &multisampleInfo;
	pipelineInfo.pDepthStencilState = &depthStencilInfo;
	pipelineInfo.pColorBlendState = &blendInfo;
	pipelineInfo.pDynamicState = &dynamicStateInfo;
	pipelineInfo.layout = layout;
	pipelineInfo.renderPass = VK_NULL_HANDLE;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	uint8_t shaderResult = 0;

	shaderResult = createShaderFromFile(renderer, "shaders/base.vert.spv", &(shaderStageInfo[0].module));
	if (!shaderResult) {
		printf("failed to create vertex shader!\n");
	}

	shaderResult = createShaderFromFile(renderer, "shaders/base.frag.spv", &(shaderStageInfo[1].module));
	if (!shaderResult) {
		printf("failed to create fragment shader!\n");
	}

	VkResult result = vkCreateGraphicsPipelines(renderer->device->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &(renderer->renderPipeline));

	free(vertexAttributeDesc);
	free(vertexBindingDesc);
	free(attachmentBlend);

	return result;
}

/*void destroyPipelineLayout(vid2_renderer_t *renderer, pipelineLayout_t layout) {
	vkDestroyPipelineLayout(renderer->device->device, layout.pipelineLayout, NULL);
}*/

void destroyPipeline(partyRenderer *renderer) {
	vkDestroyPipeline(renderer->device->device, renderer->renderPipeline, NULL);
	//destroyPipelineLayout(renderer, pipeline.pipelineLayout);
}

/*void vid2_destroyPipeline(vid2_pipeline_t *pipeline) {
	pipeline_t *inPipeline = (pipeline_t *)handleListGet(pipeline->cache->pipelines, pipeline->hdl);

	destroyPipeline(pipeline->cache->renderer, *inPipeline);

	handleListRemove(pipeline->cache->pipelines, pipeline->hdl);

	free(pipeline);
}

pipelineCache_t *initPipelineCache(vid2_renderer_t *renderer) {
	pipelineCache_t *result = malloc(sizeof(pipelineCache_t));

	result->renderer = renderer;
	result->pipelines = createHandleList(sizeof(pipeline_t), 1);

	return result;
}

void destroyPipelineCache(pipelineCache_t *cache) {
	pipeline_t *pipelines = cache->pipelines->data;

	for(int i = 0; i < cache->pipelines->allocCount; i++) {
		destroyPipeline(cache->renderer, pipelines[i]);
	}

	destroyHandleList(cache->pipelines);
}*/