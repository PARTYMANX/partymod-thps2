#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <vulkan/vulkan.h>

#include <gfx/vk/vk.h>

uint8_t createShaderFromFile(partyRenderer *renderer, char *filename, VkShaderModule *dst) {
	uint8_t result = 0;
	FILE *f = fopen(filename, "rb");

	if (f) {
		// get file length
		fseek(f, 0, SEEK_END);
		size_t filesize = ftell(f);
		fseek(f, 0, SEEK_SET);

		uint8_t *buffer = malloc(filesize);

		if (buffer) {
			fread(buffer, 1, filesize, f);

			VkShaderModuleCreateInfo shaderInfo;
			shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shaderInfo.pNext = NULL;
			shaderInfo.flags = 0;
			shaderInfo.codeSize = filesize;
			shaderInfo.pCode = buffer;

			if (vkCreateShaderModule(renderer->device->device, &shaderInfo, NULL, dst) != VK_SUCCESS) {
				printf("Failed to create shader!\n");
			} else {
				result = 1;
			}

			free(buffer);
		} else {
			printf("to allocate buffer for file!\n");
		}

		fclose(f);
	} else {
		printf("Could not open file: %s\n", filename);
	}

	return result;
}

/*
	SHADERS
*/

VkShaderModule *createShader(partyRenderer *renderer, uint32_t codeSz, uint8_t *code) {
	VkShaderModule result;

	VkShaderModuleCreateInfo shaderInfo;
	shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.pNext = NULL;
	shaderInfo.flags = 0;
	shaderInfo.codeSize = codeSz;
	shaderInfo.pCode = code;

	if (vkCreateShaderModule(renderer->device->device, &shaderInfo, NULL, &result) != VK_SUCCESS) {
		printf("Failed to create shader!\n");
		exit(1);
	}

	return result;
}

void vid2_destroyShader(partyRenderer *renderer, VkShaderModule *shader) {
	vkDestroyShaderModule(renderer->device->device, shader, NULL);
}