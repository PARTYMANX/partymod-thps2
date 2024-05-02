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

#define VK_VALIDATION

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

typedef struct rbVkRenderer rbVKRenderer;

// os-specific
struct rbVkWindow;

struct rbVkPhysicalDevice {
	VkPhysicalDevice device;
	uint32_t queueFamilyIdxs[RB_VK_QUEUE_FAMILY_COUNT];
};

// stores info relating to a GPU
struct rbVkDevice {
	struct rbVkPhysicalDevice physicalDevice;
	VkDevice device;

	struct rbVkWindow *window;
};

// stores info relating to a swapchain: a set of images that are drawn to by the gpu
struct rbVkSwapchain {
	VkSwapchainKHR swapchain;
	uint32_t imageCount;
	VkImage *images;	// TODO: make new texture struct
	VkImageView *imageViews;
	VkFormat imageFormat;
	VkExtent2D extent;

	uint32_t imageIdx;
	VkSemaphore imageReadySemaphore;	// one per image?
	VkSemaphore imageFinishedSemaphore;	// one per image?

	VkFence fence;	// should we have one per image?
	uint8_t fenceUsed;

	struct rbVkDevice *device;
};

// stores info relating to the queue of commands being run by the gpu
struct rbVkCommandQueue {
	VkQueue presentQueue;
	VkQueue queue;
	VkCommandPool commandPool;
	//VkCommandBuffer *commandBuffers;

	struct rbVkDevice *device;
};

typedef struct {
	VkPipeline pipeline;
	VkPipelineLayout layout;
} rbVkPipeline;

typedef struct {
	VkBuffer buffer;
	VmaAllocation allocation;
} rbVkBuffer;

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
} rbVkImage;

typedef struct {
	size_t vertexCapacity;
	size_t currentVertex;
	renderVertex *vertices;
	rbVkBuffer buffer;
} polyBuffer;

typedef struct {
	size_t capacity;
	size_t count;
	VkSampler samplers[2];
	uint8_t *occupied;
	rbVkImage *images;
} textureManager;

/*
	CACHES
*/

/*typedef struct {
	rbVKRenderer* renderer;

	map_t* cache;
} rbVkFramebufferCache;*/

/*typedef struct {
	struct rbVkDevice *device;

	map_t *cache;
	map_t *pipelineCache;
} renderPassCache_t;

typedef struct {
	vid2_renderer_t *renderer;

	map_t *cache;
} framebufferCache_t;

// needs better name, not really a cache
typedef struct {
	vid2_renderer_t *renderer;

	rbHandleList *pipelines;
} pipelineCache_t;

typedef struct {
	vid2_renderer_t *renderer;

	map_t *layoutCache;	// NOTE: actual descriptor set caches are contained within layouts
} descriptorSetCache_t;*/

struct rbVkMemoryManager {
	rbVKRenderer *renderer;

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
	rbVkBuffer transferbuf;
} pendingImageWrite;

typedef struct {
	rbVkImage img;
	uint8_t remainingFrames;
} pendingImageDelete;

typedef struct partyRenderer {
	//rbRenderer parent;

	struct rbVkWindow *window;
	struct rbVkDevice *device;
	struct rbVkSwapchain *swapchain;
	struct rbVkCommandQueue *queue;
	struct rbVkCommandQueue *memQueue;
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

	rbVkImage renderImage;
	VkSampler renderSampler;
	rbVkImage depthImage;
	rbVkBuffer renderImageInfoBuffer;
	float aspectRatio;
	uint32_t renderWidth;
	uint32_t renderHeight;

	polyBuffer polyBuffer;
	textureManager textureManager;
	rbVkBuffer scalerBuffer;

	struct stretchyBuffer *pendingImageWrites;
	struct stretchyBuffer *pendingImageDeletes;

	struct rbVkMemoryManager *memoryManager;

	//rbVkFramebufferCache* framebufferCache;

	//rbHandleList *textureList;

	/*
	renderPassCache_t *renderPassCache;
	framebufferCache_t *framebufferCache;

	pipelineCache_t *pipelineCache;	// what's a better name for this struct?
	descriptorSetCache_t *descriptorSetCache;
	*/

	// TODO: dirty bit for when the buffer needs to be re-recorded
	//rbHandleList *commandBuffers;
	//handleList_t *shaders;
	//handleList_t *images;
} partyRenderer;

VkResult rbVkCreateWindow(void *windowHandle, struct rbVkWindow **window);
void rbVkDestroyWindow(struct rbVkWindow *window);
void rbVkGetDrawableSize(struct rbVkWindow *window, int *pWidth, int *pHeight);
VkSurfaceKHR getWindowSurface(struct rbVkWindow *window);

VkResult createVulkanDevice(struct rbVkWindow *window, struct rbVkDevice **device);
void rbVkDestroyDevice(struct rbVkDevice *device);

VkResult rbVkCreateSwapchain(struct rbVkDevice *device, struct rbVkSwapchain **swapchain);
void rbVkDestroySwapchain(struct rbVkSwapchain *swapchain);

VkResult rbVkCreateCommandQueue(struct rbVkDevice *device, VkCommandPoolCreateFlags flags, struct rbVkCommandQueue **queue);
void rbVkDestroyCommandQueue(struct rbVkCommandQueue *queue);
VkResult createRenderCommandBuffer(partyRenderer *renderer);
void destroyRenderCommandBuffer(partyRenderer *renderer);
VkCommandBuffer startStagingCommandBuffer(partyRenderer *renderer);
void endStagingCommandBuffer(partyRenderer *renderer, VkCommandBuffer cmdbuf);

VkResult createRenderPipelines(partyRenderer *renderer);
void destroyRenderPipelines(partyRenderer *renderer);

VkResult createScalerPipeline(partyRenderer *renderer);

uint8_t createShaderFromFile(partyRenderer *renderer, char *filename, VkShaderModule *dst);
uint8_t createShader(partyRenderer *renderer, uint8_t *code, uint32_t codeSz, VkShaderModule *dst);
void vid2_destroyShader(partyRenderer *renderer, VkShaderModule *shader);

VkResult rbVkInitMemoryManager(rbVKRenderer *renderer, struct rbVkMemoryManager **memManager);
void rbVkDestroyMemoryManager(struct rbVkMemoryManager *memoryManager);

void createBuffer(partyRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage usage, rbVkBuffer *dst);
void destroyBuffer(partyRenderer *renderer, rbVkBuffer *buffer);
void *mapBuffer(partyRenderer *renderer, rbVkBuffer *buffer);
void unmapBuffer(partyRenderer *renderer, rbVkBuffer *buffer);

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

VkResult createTexture(partyRenderer *renderer, uint32_t width, uint32_t height, rbVkImage *result);
void destroyTexture(partyRenderer *renderer, rbVkImage img);
void updateTexture(partyRenderer *renderer, rbVkImage *img, uint32_t width, uint32_t height, void *data);
VkSampler createSampler(partyRenderer *renderer);

#endif