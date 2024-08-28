#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <gfx/vk/vk.h>
#include <log.h>

#include <incbin.h>

INCBIN(shader_base_vert, "shader-built/base-texture.vert.spv");
INCBIN(shader_base_frag, "shader-built/base-texture.frag.spv");
INCBIN(shader_framebuffer_vert, "shader-built/framebuffer.vert.spv");
INCBIN(shader_framebuffer_frag, "shader-built/framebuffer-sharp.frag.spv");

VkResult createRenderPipelines(partyRenderer *renderer) {
	// shaders

	VkPipelineShaderStageCreateInfo shaderStageInfo[2] = {
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.pName = "main",
			.pSpecializationInfo = NULL,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = VK_NULL_HANDLE,
		}, {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.pName = "main",
			.pSpecializationInfo = NULL,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = VK_NULL_HANDLE,
		},
	};

	// vertex input state

	VkVertexInputAttributeDescription vertexAttributeDesc[5] = {
		{
			.binding = 0,
			.location = 0,
			.format = VK_FORMAT_R32G32B32A32_SFLOAT,
			.offset = 0,
		}, {
			.binding = 0,
			.location = 1,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof(renderVertex, u),
		}, {
			.binding = 0,
			.location = 2,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.offset = offsetof(renderVertex, color),
		}, {
			.binding = 0,
			.location = 3,
			.format = VK_FORMAT_R16_SINT,
			.offset = offsetof(renderVertex, texture),
		}, {
			.binding = 0,
			.location = 4,
			.format = VK_FORMAT_R16_SINT,
			.offset = offsetof(renderVertex, flags),
		},
	};

	VkVertexInputBindingDescription vertexBindingDesc[1] = {
		{
			.binding = 0,
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
			.stride = sizeof(renderVertex),
		},
	};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.vertexAttributeDescriptionCount = 5,
		.pVertexAttributeDescriptions = vertexAttributeDesc,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = vertexBindingDesc,
	};

	// input assembly

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo[2] = {
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE,
		}, {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
			.primitiveRestartEnable = VK_FALSE,
		},
	};

	// viewport/scissor

	VkViewport viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = 0.0f,
		.height = 0.0f,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	VkRect2D scissor = {
		.offset = {
			.x = 0,
			.y = 0,
		},
		.extent = {
			.width = 0,
			.height = 0,
		},
	};
	

	VkPipelineViewportStateCreateInfo viewportInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor,
	};

	// rasterizer

	VkPipelineRasterizationStateCreateInfo rasterizerInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_NONE,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.lineWidth = 1.0f,
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthClampEnable = VK_FALSE,
	};

	// multisampling

	VkPipelineMultisampleStateCreateInfo multisampleInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = 1,
		.minSampleShading = 1.0f,
		.pSampleMask = NULL,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE,
	};

	// depth-stencil

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL,

		.depthBoundsTestEnable = VK_FALSE,
		.minDepthBounds = 0.0f,
		.maxDepthBounds = 1.0f,

		.stencilTestEnable = VK_FALSE,

		.back = {
			.failOp = VK_STENCIL_OP_ZERO,
			.passOp = VK_STENCIL_OP_ZERO,
			.depthFailOp = VK_STENCIL_OP_ZERO,
			.compareOp = VK_COMPARE_OP_ALWAYS,
			.compareMask = 0,
			.writeMask = 0,
			.reference = 0,
		},

		.front = {
			.failOp = VK_STENCIL_OP_ZERO,
			.passOp = VK_STENCIL_OP_ZERO,
			.depthFailOp = VK_STENCIL_OP_ZERO,
			.compareOp = VK_COMPARE_OP_ALWAYS,
			.compareMask = 0,
			.writeMask = 0,
			.reference = 0,
		},
	};

	// blending

	VkPipelineColorBlendAttachmentState attachmentBlend[5] = {
		{
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},{
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},{
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},{
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},{
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.alphaBlendOp = VK_BLEND_OP_ADD,
		},
	};

	VkPipelineColorBlendStateCreateInfo blendInfo[5] = {
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = attachmentBlend,
			.blendConstants[0] = 0.0f,
			.blendConstants[1] = 0.0f,
			.blendConstants[2] = 0.0f,
			.blendConstants[3] = 0.0f,
		},{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = attachmentBlend + 1,
			.blendConstants[0] = 0.0f,
			.blendConstants[1] = 0.0f,
			.blendConstants[2] = 0.0f,
			.blendConstants[3] = 0.0f,
		},{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = attachmentBlend + 2,
			.blendConstants[0] = 0.0f,
			.blendConstants[1] = 0.0f,
			.blendConstants[2] = 0.0f,
			.blendConstants[3] = 0.0f,
		},{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = attachmentBlend + 3,
			.blendConstants[0] = 0.0f,
			.blendConstants[1] = 0.0f,
			.blendConstants[2] = 0.0f,
			.blendConstants[3] = 0.0f,
		},{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = attachmentBlend + 4,
			.blendConstants[0] = 0.0f,
			.blendConstants[1] = 0.0f,
			.blendConstants[2] = 0.0f,
			.blendConstants[3] = 0.0f,
		},
	};

	// dynamic state

	const VkDynamicState dynamicState[] = { 
		VK_DYNAMIC_STATE_VIEWPORT, 
		VK_DYNAMIC_STATE_SCISSOR, 
		//VK_DYNAMIC_STATE_DEPTH_BIAS, 
		VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE,
		VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
		//VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY,
		//VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT,
		//VK_DYNAMIC_STATE_POLYGON_MODE_EXT, 
	};

	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.dynamicStateCount = 4,
		.pDynamicStates = &dynamicState,
	};

	// pipeline layout

	VkDescriptorSetLayoutBinding descBindings[1] = {
		{
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.binding = 0,
			.descriptorCount = 2048,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImmutableSamplers = NULL,
		},
	};

	VkDescriptorSetLayoutCreateInfo descInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.bindingCount = 1,
		.pBindings = descBindings,
	};

	log_printf(LL_TRACE, "createRenderPipelines(): descriptor set layout\n");
	if (vkCreateDescriptorSetLayout(renderer->device->device, &descInfo, NULL, &renderer->renderDescriptorLayout) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create descriptor set layout!\n");
		exit(1);
	}

	VkPipelineLayout layout = VK_NULL_HANDLE;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.setLayoutCount = 1,
		.pSetLayouts = &renderer->renderDescriptorLayout,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = NULL,
	};

	log_printf(LL_TRACE, "createRenderPipelines(): pipeline layout\n");
	if (vkCreatePipelineLayout(renderer->device->device, &pipelineLayoutInfo, NULL, &(layout)) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create pipeline layout!\n");
		exit(1);
	}

	renderer->renderPipelineLayout = layout;

	VkPipelineRenderingCreateInfo renderingInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.pNext = NULL,
		.viewMask = 0,
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &renderer->swapchain->imageFormat,
		.depthAttachmentFormat = renderer->depthImage.pixelFormat,
		.stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
	};

	// finally, the pipeline

	VkGraphicsPipelineCreateInfo pipelineInfo[10] = {
		{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 1,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 2,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 3,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 4,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo + 1,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo + 1,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 1,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo + 1,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 2,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo + 1,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 3,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = &renderingInfo,
			.flags = 0,
			.stageCount = 2,
			.pStages = shaderStageInfo,
			.pVertexInputState = &vertexInputInfo,
			.pInputAssemblyState = inputAssemblyInfo + 1,
			.pViewportState = &viewportInfo,
			.pRasterizationState = &rasterizerInfo,
			.pMultisampleState = &multisampleInfo,
			.pDepthStencilState = &depthStencilInfo,
			.pColorBlendState = blendInfo + 4,
			.pDynamicState = &dynamicStateInfo,
			.layout = layout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1,
		},
	};

	uint8_t shaderResult = 0;

	/*shaderResult = createShaderFromFile(renderer, "shaders/base-texture.vert.spv", &(shaderStageInfo[0].module));
	if (!shaderResult) {
		printf("failed to create vertex shader!\n");
	}

	shaderResult = createShaderFromFile(renderer, "shaders/base-texture.frag.spv", &(shaderStageInfo[1].module));
	if (!shaderResult) {
		printf("failed to create fragment shader!\n");
	}*/

	log_printf(LL_TRACE, "createRenderPipelines(): vertex shader\n");
	shaderResult = createShader(renderer, gshader_base_vertData, gshader_base_vertSize, &(shaderStageInfo[0].module));
	if (!shaderResult) {
		log_printf(LL_ERROR, "failed to create vertex shader for render pipeline!\n");
	}

	log_printf(LL_TRACE, "createRenderPipelines(): fragment shader\n");
	shaderResult = createShader(renderer, gshader_base_fragData, gshader_base_fragSize, &(shaderStageInfo[1].module));
	if (!shaderResult) {
		log_printf(LL_ERROR, "failed to create fragment shader for render pipeline!\n");
	}

	// to avoid having to use a dynamic state extension (even though probably everyone has it), make one pipeline per blend mode
	log_printf(LL_TRACE, "createRenderPipelines(): pipelines\n");
	VkResult result = vkCreateGraphicsPipelines(renderer->device->device, VK_NULL_HANDLE, 10, pipelineInfo, NULL, renderer->renderPipelines);

	//free(vertexAttributeDesc);
	//free(vertexBindingDesc);
	//free(attachmentBlend);

	return result;
}

void destroyPipeline(partyRenderer *renderer) {
	for (int i = 0; i < 10; i++) {
		vkDestroyPipeline(renderer->device->device, renderer->renderPipelines[i], NULL);
	}
	
	//destroyPipelineLayout(renderer, pipeline.pipelineLayout);
}

VkResult createScalerPipeline(partyRenderer *renderer) {
	VkPipelineShaderStageCreateInfo shaderStageInfo[2] = {
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.pName = "main",
			.pSpecializationInfo = NULL,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = VK_NULL_HANDLE,
		},{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.pName = "main",
			.pSpecializationInfo = NULL,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = VK_NULL_HANDLE,
		},
	};

	// vertex input state

	VkVertexInputAttributeDescription vertexAttributeDesc[2] = {
		{
			.binding = 0,
			.location = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = 0,
		},{
			.binding = 0,
			.location = 1,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = (sizeof(float) * 2),
		},
	};

	VkVertexInputBindingDescription vertexBindingDesc[1] = {
		{
			.binding = 0,
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
			.stride = (sizeof(float) * 2) + (sizeof(float) * 2),
		},
	};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.vertexAttributeDescriptionCount = 2,
		.pVertexAttributeDescriptions = vertexAttributeDesc,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = vertexBindingDesc,
	};

	// input assembly

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	// viewport/scissor

	VkViewport viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = 0.0f,
		.height = 0.0f,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	VkRect2D scissor = {
		.offset = {
			.x = 0,
			.y = 0,
		},
		.extent = {
			.width = 0,
			.height = 0,
		},
	};

	VkPipelineViewportStateCreateInfo viewportInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor,
	};

	// rasterizer

	VkPipelineRasterizationStateCreateInfo rasterizerInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_NONE,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
		.lineWidth = 1.0f,
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthClampEnable = VK_FALSE,
	};

	// multisampling

	VkPipelineMultisampleStateCreateInfo multisampleInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = 1,
		.minSampleShading = 1.0f,
		.pSampleMask = NULL,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE,
	};

	// depth-stencil

	VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.depthTestEnable = VK_FALSE,
		.depthWriteEnable = VK_FALSE,
		.depthCompareOp = VK_COMPARE_OP_LESS,

		.depthBoundsTestEnable = VK_FALSE,
		.minDepthBounds = 0.0f,
		.maxDepthBounds = 1.0f,

		.stencilTestEnable = VK_FALSE,

		.back = {
			.failOp = VK_STENCIL_OP_ZERO,
			.passOp = VK_STENCIL_OP_ZERO,
			.depthFailOp = VK_STENCIL_OP_ZERO,
			.compareOp = VK_COMPARE_OP_ALWAYS,
			.compareMask = 0,
			.writeMask = 0,
			.reference = 0,
		},

		.front = {
			.failOp = VK_STENCIL_OP_ZERO,
			.passOp = VK_STENCIL_OP_ZERO,
			.depthFailOp = VK_STENCIL_OP_ZERO,
			.compareOp = VK_COMPARE_OP_ALWAYS,
			.compareMask = 0,
			.writeMask = 0,
			.reference = 0,
		},
	};

	// blending

	VkPipelineColorBlendAttachmentState attachmentBlend = {
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		.blendEnable = VK_FALSE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.alphaBlendOp = VK_BLEND_OP_ADD,
	};

	VkPipelineColorBlendStateCreateInfo blendInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &attachmentBlend,
		.blendConstants[0] = 0.0f,
		.blendConstants[1] = 0.0f,
		.blendConstants[2] = 0.0f,
		.blendConstants[3] = 0.0f,
	};

	// dynamic state

	const VkDynamicState dynamicState[] = { 
		VK_DYNAMIC_STATE_VIEWPORT, 
		VK_DYNAMIC_STATE_SCISSOR, 
	};

	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.dynamicStateCount = 2,
		.pDynamicStates = &dynamicState,
	};

	// pipeline layout

	VkDescriptorSetLayoutBinding descBindings[2] = {
		{
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.binding = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.pImmutableSamplers = NULL,
		},{
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
			.binding = 1,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pImmutableSamplers = NULL,
		},
	};

	VkDescriptorSetLayoutCreateInfo descInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.bindingCount = 2,
		.pBindings = descBindings,
	};

	log_printf(LL_TRACE, "createScalerPipeline(): descriptor set layout\n");
	if (vkCreateDescriptorSetLayout(renderer->device->device, &descInfo, NULL, &renderer->scalerLayout) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create descriptor set layout!\n");
		exit(1);
	}

	VkPipelineLayout layout = VK_NULL_HANDLE;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.setLayoutCount = 1,
		.pSetLayouts = &renderer->scalerLayout,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = NULL,
	};

	log_printf(LL_TRACE, "createScalerPipeline(): pipeline layout\n");
	if (vkCreatePipelineLayout(renderer->device->device, &pipelineLayoutInfo, NULL, &layout) != VK_SUCCESS) {
		log_printf(LL_ERROR, "Failed to create pipeline layout!\n");
		exit(1);
	}

	VkPipelineRenderingCreateInfo renderingInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.pNext = NULL,
		.viewMask = 0,
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &renderer->swapchain->imageFormat,
		.depthAttachmentFormat = VK_FORMAT_UNDEFINED,
		.stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
	};

	// finally, the pipeline

	VkGraphicsPipelineCreateInfo pipelineInfo = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &renderingInfo,
		.flags = 0,
		.stageCount = 2,
		.pStages = shaderStageInfo,
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssemblyInfo,
		.pViewportState = &viewportInfo,
		.pRasterizationState = &rasterizerInfo,
		.pMultisampleState = &multisampleInfo,
		.pDepthStencilState = &depthStencilInfo,
		.pColorBlendState = &blendInfo,
		.pDynamicState = &dynamicStateInfo,
		.layout = layout,
		.renderPass = VK_NULL_HANDLE,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1,
	};

	uint8_t shaderResult = 0;

	/*shaderResult = createShaderFromFile(renderer, "shaders/framebuffer.vert.spv", &(shaderStageInfo[0].module));
	if (!shaderResult) {
		printf("failed to create vertex shader!\n");
	}

	shaderResult = createShaderFromFile(renderer, "shaders/framebuffer-sharp.frag.spv", &(shaderStageInfo[1].module));
	if (!shaderResult) {
		printf("failed to create fragment shader!\n");
	}*/

	log_printf(LL_TRACE, "createScalerPipeline(): vertex shader\n");
	shaderResult = createShader(renderer, gshader_framebuffer_vertData, gshader_framebuffer_vertSize, &(shaderStageInfo[0].module));
	if (!shaderResult) {
		log_printf(LL_ERROR, "failed to create vertex shader!\n");
	}

	log_printf(LL_TRACE, "createScalerPipeline(): fragment shader\n");
	shaderResult = createShader(renderer, gshader_framebuffer_fragData, gshader_framebuffer_fragSize, &(shaderStageInfo[1].module));
	if (!shaderResult) {
		log_printf(LL_ERROR, "failed to create vertex shader!\n");
	}

	log_printf(LL_TRACE, "createScalerPipeline(): pipeline\n");
	VkResult result = vkCreateGraphicsPipelines(renderer->device->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &renderer->scalerPipeline);
	renderer->scalerPipelineLayout = layout;

	return result;
}
