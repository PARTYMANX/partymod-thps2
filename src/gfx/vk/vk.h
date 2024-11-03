#ifndef _VK_H_
#define _VK_H_

#include <util/sb.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <gfx/vk/gfx_vk.h>

#define RB_VK_QUEUE_FAMILY_COUNT 2
#define RB_VK_QUEUE_FAMILY_GRAPHICS 0
#define RB_VK_QUEUE_FAMILY_PRESENT 1

#define QUEUE_FAMILY_COUNT 2
#define QUEUE_FAMILY_GRAPHICS 0
#define QUEUE_FAMILY_PRESENT 1

//#define VK_VALIDATION

/*
	GLOBALS
*/

VkInstance instance;

/*
	CONST
*/

const uint32_t validationLayerCount;
const char * const validationLayerNames[];

const uint32_t validationExtCount;
const char * const validationExtNames[];

typedef struct pmVkRenderer pmVkRenderer;

// os-specific
struct pmVkWindow;

struct pmVkPhysicalDevice {
	VkPhysicalDevice device;
	uint32_t queueFamilyIdxs[RB_VK_QUEUE_FAMILY_COUNT];
};

// stores info relating to a GPU
struct pmVkDevice {
	struct pmVkPhysicalDevice physicalDevice;
	VkDevice device;

	struct pmVkWindow *window;
};

// stores info relating to a swapchain: a set of images that are drawn to by the gpu
struct pmVkSwapchain {
	VkSwapchainKHR swapchain;
	uint32_t imageCount;
	VkImage *images;	// TODO: make new texture struct
	VkImageView *imageViews;
	VkFormat imageFormat;
	VkExtent2D extent;

	uint32_t imageIdx;
	VkSemaphore imageReadySemaphore;
	VkSemaphore imageFinishedSemaphore;

	VkFence fence;	// should we have one per image?
	uint8_t fenceUsed;

	struct pmVkDevice *device;
};

// stores info relating to the queue of commands being run by the gpu
struct pmVkCommandQueue {
	VkQueue presentQueue;
	VkQueue queue;
	VkCommandPool commandPool;
	//VkCommandBuffer *commandBuffers;

	struct pmVkDevice *device;
};

typedef struct {
	VkPipeline pipeline;
	VkPipelineLayout layout;
} pmVkPipeline;

typedef struct {
	VkBuffer buffer;
	VmaAllocation allocation;
} pmVkBuffer;

typedef struct {
	VkImage image;
	VkImageView imageView;
	VmaAllocation allocation;

	VkImageType type;
	VkFormat pixelFormat;

	uint32_t width;
	uint32_t height;
	uint32_t depth;

	uint32_t mipmapCount;
	uint32_t arrayLength;
	uint32_t sampleCount;
} pmVkImage;

typedef struct {
	size_t vertexCapacity;
	size_t currentVertex;
	renderVertex *vertices;
	pmVkBuffer buffer;
} polyBuffer;

typedef struct {
	size_t capacity;
	size_t count;
	VkSampler samplers[2];
	uint8_t *occupied;
	pmVkImage *images;
} textureManager;

struct pmVkMemoryManager {
	pmVkRenderer *renderer;

	VmaAllocator allocator;
};

struct PoolSizeRatio {
	VkDescriptorType type;
	float ratio;
};

typedef struct {
	uint32_t sets_per_pool;

	struct stretchyBuffer *ratios;
	struct stretchyBuffer *full_pools;
	struct stretchyBuffer *ready_pools;
	struct stretchyBuffer *writes;
} pmVkDescriptorAllocator;

typedef struct {
	VkCommandBuffer cmdbuf;
	pmVkBuffer transferbuf;
} pendingImageWrite;

typedef struct {
	pmVkImage img;
	uint8_t remainingFrames;
} pendingImageDelete;

typedef struct partyRenderer {
	//rbRenderer parent;

	struct pmVkWindow *window;
	struct pmVkDevice *device;
	struct pmVkSwapchain *swapchain;
	struct pmVkCommandQueue *queue;
	struct pmVkCommandQueue *memQueue;
	pmVkDescriptorAllocator *descriptorAllocator;

	VkViewport currentViewport;
	VkRect2D currentScissor;
	uint8_t currentDepthTestState;
	uint8_t currentDepthWriteState;
	uint8_t currentBlendState;
	uint8_t currentLineState;

	uint32_t lastDraw;
	uint32_t processedVerts;

	uint8_t textureFilter;

	VkCommandBuffer renderCommandBuffer;
	VkDescriptorSetLayout renderDescriptorLayout;
	VkPipelineLayout renderPipelineLayout;
	VkPipeline renderPipelines[10];
	VkDescriptorSetLayout scalerLayout;
	VkPipelineLayout scalerPipelineLayout;
	VkPipeline scalerPipeline;

	pmVkImage renderImage;
	VkSampler renderSampler;
	pmVkImage depthImage;
	pmVkBuffer renderImageInfoBuffer;
	float aspectRatio;
	uint32_t renderWidth;
	uint32_t renderHeight;

	polyBuffer polyBuffer;
	textureManager textureManager;
	pmVkBuffer scalerBuffer;

	struct stretchyBuffer *pendingImageWrites;
	struct stretchyBuffer *pendingImageDeletes;

	struct pmVkMemoryManager *memoryManager;
} partyRenderer;

VkResult pmVkCreateWindow(void *windowHandle, struct pmVkWindow **window);
void pmVkDestroyWindow(struct pmVkWindow *window);
void pmVkGetDrawableSize(struct pmVkWindow *window, int *pWidth, int *pHeight);
VkSurfaceKHR getWindowSurface(struct pmVkWindow *window);

VkResult createVulkanDevice(struct pmVkWindow *window, struct pmVkDevice **device);
void pmVkDestroyDevice(struct pmVkDevice *device);

VkResult pmVkCreateSwapchain(struct pmVkDevice *device, struct pmVkSwapchain **swapchain);
void pmVkDestroySwapchain(struct pmVkSwapchain *swapchain);

VkResult pmVkCreateCommandQueue(struct pmVkDevice *device, VkCommandPoolCreateFlags flags, struct pmVkCommandQueue **queue);
void pmVkDestroyCommandQueue(struct pmVkCommandQueue *queue);
VkResult createRenderCommandBuffer(partyRenderer *renderer);
void destroyRenderCommandBuffer(partyRenderer *renderer);
VkCommandBuffer startStagingCommandBuffer(partyRenderer *renderer);
void endStagingCommandBuffer(partyRenderer *renderer, VkCommandBuffer cmdbuf);

VkResult createRenderPipelines(partyRenderer *renderer);
void destroyRenderPipelines(partyRenderer *renderer);

VkResult createScalerPipeline(partyRenderer *renderer);

uint8_t createShaderFromFile(partyRenderer *renderer, char *filename, VkShaderModule *dst);
uint8_t createShader(partyRenderer *renderer, uint8_t *code, uint32_t codeSz, VkShaderModule *dst);
void destroyShader(partyRenderer *renderer, VkShaderModule *shader);

VkResult pmVkInitMemoryManager(pmVkRenderer *renderer, struct pmVkMemoryManager **memManager);
void pmVkDestroyMemoryManager(struct pmVkMemoryManager *memoryManager);

void createBuffer(partyRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage usage, pmVkBuffer *dst);
void destroyBuffer(partyRenderer *renderer, pmVkBuffer *buffer);
void *mapBuffer(partyRenderer *renderer, pmVkBuffer *buffer);
void unmapBuffer(partyRenderer *renderer, pmVkBuffer *buffer);

void createRenderTargets(partyRenderer *renderer, uint32_t width, uint32_t height, VkFormat colorFmt, VkFormat depthFmt);
void destroyRenderTargets(partyRenderer *renderer);

pmVkDescriptorAllocator *init_descriptors(partyRenderer *renderer, uint32_t max_sets, struct PoolSizeRatio *poolRatios, size_t poolRatioCount);
void clear_descriptor_pools(partyRenderer *renderer, pmVkDescriptorAllocator *allocator);
void destroy_descriptor_pools(partyRenderer *renderer, pmVkDescriptorAllocator *allocator);
VkDescriptorSet allocate_descriptor_set(partyRenderer *renderer, pmVkDescriptorAllocator *allocator, VkDescriptorSetLayout layout);
void write_descriptor_buffer(pmVkDescriptorAllocator *allocator, int binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type);
void write_descriptor_image(pmVkDescriptorAllocator *allocator, int binding, VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout, VkDescriptorType type);
void write_descriptor_image_array(pmVkDescriptorAllocator *allocator, int binding, int idx, VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout, VkDescriptorType type);
void update_set(partyRenderer *renderer, pmVkDescriptorAllocator *allocator, VkDescriptorSet set);
void clear_writes(pmVkDescriptorAllocator *allocator);

VkResult createTexture(partyRenderer *renderer, uint32_t width, uint32_t height, pmVkImage *result);
void destroyTexture(partyRenderer *renderer, pmVkImage img);
void updateTexture(partyRenderer *renderer, pmVkImage *img, uint32_t width, uint32_t height, void *data);
VkSampler createSampler(partyRenderer *renderer, VkFilter minMagFilter);

#endif