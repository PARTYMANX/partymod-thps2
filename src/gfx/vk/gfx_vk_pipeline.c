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

	VkVertexInputAttributeDescription vertexAttributeDesc[5];

	vertexAttributeDesc[0].binding = 0;
	vertexAttributeDesc[0].location = 0;
	vertexAttributeDesc[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	vertexAttributeDesc[0].offset = 0;

	vertexAttributeDesc[1].binding = 0;
	vertexAttributeDesc[1].location = 1;
	vertexAttributeDesc[1].format = VK_FORMAT_R32G32_SFLOAT;
	vertexAttributeDesc[1].offset = offsetof(renderVertex, u);

	vertexAttributeDesc[2].binding = 0;
	vertexAttributeDesc[2].location = 2;
	vertexAttributeDesc[2].format = VK_FORMAT_R8G8B8A8_UNORM;
	vertexAttributeDesc[2].offset = offsetof(renderVertex, color);

	vertexAttributeDesc[3].binding = 0;
	vertexAttributeDesc[3].location = 3;
	vertexAttributeDesc[3].format = VK_FORMAT_R16_SINT;
	vertexAttributeDesc[3].offset = offsetof(renderVertex, texture);

	vertexAttributeDesc[4].binding = 0;
	vertexAttributeDesc[4].location = 4;
	vertexAttributeDesc[4].format = VK_FORMAT_R16_SINT;
	vertexAttributeDesc[4].offset = offsetof(renderVertex, flags);

	VkVertexInputBindingDescription vertexBindingDesc[1];

	vertexBindingDesc[0].binding = 0;
	vertexBindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexBindingDesc[0].stride = sizeof(renderVertex);

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = NULL;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 5;
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDesc;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = vertexBindingDesc;

	// input assembly

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo[2];
	inputAssemblyInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo[0].pNext = NULL;
	inputAssemblyInfo[0].flags = 0;
	inputAssemblyInfo[0].topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo[0].primitiveRestartEnable = VK_FALSE;

	inputAssemblyInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo[1].pNext = NULL;
	inputAssemblyInfo[1].flags = 0;
	inputAssemblyInfo[1].topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	inputAssemblyInfo[1].primitiveRestartEnable = VK_FALSE;

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
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;

	// depth-stencil

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.pNext = NULL;
	depthStencilInfo.flags = 0;
	depthStencilInfo.depthTestEnable = VK_TRUE;
	depthStencilInfo.depthWriteEnable = VK_TRUE;
	depthStencilInfo.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;

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

	VkPipelineColorBlendAttachmentState attachmentBlend[5];

	attachmentBlend[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachmentBlend[0].blendEnable = VK_TRUE;
	attachmentBlend[0].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentBlend[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentBlend[0].colorBlendOp = VK_BLEND_OP_ADD;
	attachmentBlend[0].alphaBlendOp = VK_BLEND_OP_ADD;

	attachmentBlend[1].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachmentBlend[1].blendEnable = VK_TRUE;
	attachmentBlend[1].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	attachmentBlend[1].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	attachmentBlend[1].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	attachmentBlend[1].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	attachmentBlend[1].colorBlendOp = VK_BLEND_OP_ADD;
	attachmentBlend[1].alphaBlendOp = VK_BLEND_OP_ADD;

	attachmentBlend[2].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachmentBlend[2].blendEnable = VK_TRUE;
	attachmentBlend[2].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	attachmentBlend[2].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	attachmentBlend[2].dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[2].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[2].colorBlendOp = VK_BLEND_OP_ADD;
	attachmentBlend[2].alphaBlendOp = VK_BLEND_OP_ADD;

	attachmentBlend[3].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachmentBlend[3].blendEnable = VK_TRUE;
	attachmentBlend[3].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentBlend[3].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentBlend[3].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
	attachmentBlend[3].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
	attachmentBlend[3].colorBlendOp = VK_BLEND_OP_ADD;
	attachmentBlend[3].alphaBlendOp = VK_BLEND_OP_ADD;

	attachmentBlend[4].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachmentBlend[4].blendEnable = VK_TRUE;
	attachmentBlend[4].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[4].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[4].dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[4].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend[4].colorBlendOp = VK_BLEND_OP_ADD;
	attachmentBlend[4].alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo blendInfo[5];
	blendInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendInfo[0].pNext = NULL;
	blendInfo[0].flags = 0;
	blendInfo[0].logicOpEnable = VK_FALSE;
	blendInfo[0].logicOp = VK_LOGIC_OP_COPY;
	blendInfo[0].attachmentCount = 1;
	blendInfo[0].pAttachments = attachmentBlend;
	blendInfo[0].blendConstants[0] = 0.0f;
	blendInfo[0].blendConstants[1] = 0.0f;
	blendInfo[0].blendConstants[2] = 0.0f;
	blendInfo[0].blendConstants[3] = 0.0f;

	blendInfo[1] = blendInfo[0];
	blendInfo[1].pAttachments = attachmentBlend + 1;

	blendInfo[2] = blendInfo[0];
	blendInfo[2].pAttachments = attachmentBlend + 2;

	blendInfo[3] = blendInfo[0];
	blendInfo[3].pAttachments = attachmentBlend + 3;

	blendInfo[4] = blendInfo[0];
	blendInfo[4].pAttachments = attachmentBlend + 4;

	// dynamic state

	VkDynamicState dynamicState[] = { 
		VK_DYNAMIC_STATE_VIEWPORT, 
		VK_DYNAMIC_STATE_SCISSOR, 
		//VK_DYNAMIC_STATE_DEPTH_BIAS, 
		VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE,
		VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
		VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY,
		VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT,
		VK_DYNAMIC_STATE_POLYGON_MODE_EXT, 
	};

	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pNext = NULL;
	dynamicStateInfo.flags = 0;
	dynamicStateInfo.dynamicStateCount = 4;
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

	VkDescriptorSetLayoutBinding descBindings[1];
	descBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descBindings[0].binding = 0;
	descBindings[0].descriptorCount = 2048;
	descBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descBindings[0].pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descInfo;
	descInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descInfo.pNext = NULL;
	descInfo.flags = 0;
	descInfo.bindingCount = 1;
	descInfo.pBindings = descBindings;

	if (vkCreateDescriptorSetLayout(renderer->device->device, &descInfo, NULL, &renderer->renderDescriptorLayout) != VK_SUCCESS) {
		printf("Failed to create descriptor set layout!\n");
		exit(1);
	}

	VkPipelineLayout layout;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = NULL;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &renderer->renderDescriptorLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = NULL;

	if (vkCreatePipelineLayout(renderer->device->device, &pipelineLayoutInfo, NULL, &(layout)) != VK_SUCCESS) {
		printf("Failed to create pipeline layout!\n");
		exit(1);
	}

	renderer->renderPipelineLayout = layout;

	VkPipelineRenderingCreateInfo renderingInfo;
	renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	renderingInfo.pNext = NULL;
	renderingInfo.viewMask = 0;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachmentFormats = &renderer->swapchain->imageFormat;
	renderingInfo.depthAttachmentFormat = renderer->depthImage.pixelFormat;
	renderingInfo.stencilAttachmentFormat = renderer->depthImage.pixelFormat;

	// finally, the pipeline

	VkGraphicsPipelineCreateInfo pipelineInfo[10];
	pipelineInfo[0].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo[0].pNext = &renderingInfo;
	pipelineInfo[0].flags = 0;
	pipelineInfo[0].stageCount = 2;
	pipelineInfo[0].pStages = shaderStageInfo;
	pipelineInfo[0].pVertexInputState = &vertexInputInfo;
	pipelineInfo[0].pInputAssemblyState = inputAssemblyInfo;
	pipelineInfo[0].pViewportState = &viewportInfo;
	pipelineInfo[0].pRasterizationState = &rasterizerInfo;
	pipelineInfo[0].pMultisampleState = &multisampleInfo;
	pipelineInfo[0].pDepthStencilState = &depthStencilInfo;
	pipelineInfo[0].pColorBlendState = blendInfo;
	pipelineInfo[0].pDynamicState = &dynamicStateInfo;
	pipelineInfo[0].layout = layout;
	pipelineInfo[0].renderPass = VK_NULL_HANDLE;
	pipelineInfo[0].subpass = 0;
	pipelineInfo[0].basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo[0].basePipelineIndex = -1;

	pipelineInfo[1] = pipelineInfo[0];
	pipelineInfo[1].pColorBlendState = blendInfo + 1;

	pipelineInfo[2] = pipelineInfo[0];
	pipelineInfo[2].pColorBlendState = blendInfo + 2;

	pipelineInfo[3] = pipelineInfo[0];
	pipelineInfo[3].pColorBlendState = blendInfo + 3;

	pipelineInfo[4] = pipelineInfo[0];
	pipelineInfo[4].pColorBlendState = blendInfo + 4;

	pipelineInfo[5] = pipelineInfo[0];
	pipelineInfo[5].pInputAssemblyState = inputAssemblyInfo + 1;

	pipelineInfo[6] = pipelineInfo[1];
	pipelineInfo[6].pInputAssemblyState = inputAssemblyInfo + 1;

	pipelineInfo[7] = pipelineInfo[2];
	pipelineInfo[7].pInputAssemblyState = inputAssemblyInfo + 1;

	pipelineInfo[8] = pipelineInfo[3];
	pipelineInfo[8].pInputAssemblyState = inputAssemblyInfo + 1;

	pipelineInfo[9] = pipelineInfo[4];
	pipelineInfo[9].pInputAssemblyState = inputAssemblyInfo + 1;

	uint8_t shaderResult = 0;

	shaderResult = createShaderFromFile(renderer, "shaders/base-texture.vert.spv", &(shaderStageInfo[0].module));
	if (!shaderResult) {
		printf("failed to create vertex shader!\n");
	}

	shaderResult = createShaderFromFile(renderer, "shaders/base-texture.frag.spv", &(shaderStageInfo[1].module));
	if (!shaderResult) {
		printf("failed to create fragment shader!\n");
	}

	// to avoid having to use a dynamic state extension (even though probably everyone has it), make one pipeline per blend mode

	VkResult result = vkCreateGraphicsPipelines(renderer->device->device, VK_NULL_HANDLE, 10, pipelineInfo, NULL, renderer->renderPipelines);

	free(vertexAttributeDesc);
	free(vertexBindingDesc);
	//free(attachmentBlend);

	return result;
}

/*void destroyPipelineLayout(vid2_renderer_t *renderer, pipelineLayout_t layout) {
	vkDestroyPipelineLayout(renderer->device->device, layout.pipelineLayout, NULL);
}*/

void destroyPipeline(partyRenderer *renderer) {
	for (int i = 0; i < 10; i++) {
		vkDestroyPipeline(renderer->device->device, renderer->renderPipelines[i], NULL);
	}
	
	//destroyPipelineLayout(renderer, pipeline.pipelineLayout);
}

VkResult createScalerPipeline(partyRenderer *renderer) {
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

	VkVertexInputAttributeDescription vertexAttributeDesc[2];

	vertexAttributeDesc[0].binding = 0;
	vertexAttributeDesc[0].location = 0;
	vertexAttributeDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
	vertexAttributeDesc[0].offset = 0;

	vertexAttributeDesc[1].binding = 0;
	vertexAttributeDesc[1].location = 1;
	vertexAttributeDesc[1].format = VK_FORMAT_R32G32_SFLOAT;
	vertexAttributeDesc[1].offset = (sizeof(float) * 2);

	VkVertexInputBindingDescription vertexBindingDesc[1];

	vertexBindingDesc[0].binding = 0;
	vertexBindingDesc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexBindingDesc[0].stride = (sizeof(float) * 2) + (sizeof(float) * 2);

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = NULL;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 2;
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
	multisampleInfo.alphaToCoverageEnable = VK_FALSE;
	multisampleInfo.alphaToOneEnable = VK_FALSE;

	// depth-stencil

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilInfo.pNext = NULL;
	depthStencilInfo.flags = 0;
	depthStencilInfo.depthTestEnable = VK_FALSE;
	depthStencilInfo.depthWriteEnable = VK_FALSE;
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

	VkPipelineColorBlendAttachmentState attachmentBlend;

	attachmentBlend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachmentBlend.blendEnable = VK_FALSE;
	attachmentBlend.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	attachmentBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	attachmentBlend.colorBlendOp = VK_BLEND_OP_ADD;
	attachmentBlend.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo blendInfo;
	blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendInfo.pNext = NULL;
	blendInfo.flags = 0;
	blendInfo.logicOpEnable = VK_FALSE;
	blendInfo.logicOp = VK_LOGIC_OP_COPY;
	blendInfo.attachmentCount = 1;
	blendInfo.pAttachments = &attachmentBlend;
	blendInfo.blendConstants[0] = 0.0f;
	blendInfo.blendConstants[1] = 0.0f;
	blendInfo.blendConstants[2] = 0.0f;
	blendInfo.blendConstants[3] = 0.0f;

	// dynamic state

	VkDynamicState dynamicState[] = { 
		VK_DYNAMIC_STATE_VIEWPORT, 
		VK_DYNAMIC_STATE_SCISSOR, 
	};

	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pNext = NULL;
	dynamicStateInfo.flags = 0;
	dynamicStateInfo.dynamicStateCount = 2;
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

	VkDescriptorSetLayoutBinding descBindings[2];
	descBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descBindings[0].binding = 0;
	descBindings[0].descriptorCount = 1;
	descBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descBindings[0].pImmutableSamplers = NULL;

	descBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descBindings[1].binding = 1;
	descBindings[1].descriptorCount = 1;
	descBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descBindings[1].pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descInfo;
	descInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descInfo.pNext = NULL;
	descInfo.flags = 0;
	descInfo.bindingCount = 2;
	descInfo.pBindings = descBindings;

	if (vkCreateDescriptorSetLayout(renderer->device->device, &descInfo, NULL, &renderer->scalerLayout) != VK_SUCCESS) {
		printf("Failed to create descriptor set layout!\n");
		exit(1);
	}

	VkPipelineLayout layout;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = NULL;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &renderer->scalerLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = NULL;

	if (vkCreatePipelineLayout(renderer->device->device, &pipelineLayoutInfo, NULL, &layout) != VK_SUCCESS) {
		printf("Failed to create pipeline layout!\n");
		exit(1);
	}

	VkPipelineRenderingCreateInfo renderingInfo;
	renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	renderingInfo.pNext = NULL;
	renderingInfo.viewMask = 0;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachmentFormats = &renderer->swapchain->imageFormat;
	renderingInfo.depthAttachmentFormat = renderer->depthImage.pixelFormat;
	renderingInfo.stencilAttachmentFormat = renderer->depthImage.pixelFormat;

	// finally, the pipeline

	VkGraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.pNext = &renderingInfo;
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

	shaderResult = createShaderFromFile(renderer, "shaders/framebuffer.vert.spv", &(shaderStageInfo[0].module));
	if (!shaderResult) {
		printf("failed to create vertex shader!\n");
	}

	shaderResult = createShaderFromFile(renderer, "shaders/framebuffer-sharp.frag.spv", &(shaderStageInfo[1].module));
	if (!shaderResult) {
		printf("failed to create fragment shader!\n");
	}

	VkResult result = vkCreateGraphicsPipelines(renderer->device->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &renderer->scalerPipeline);
	renderer->scalerPipelineLayout = layout;

	free(vertexAttributeDesc);
	free(vertexBindingDesc);
	//free(attachmentBlend);

	return result;
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