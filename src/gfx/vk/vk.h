#ifndef _VK_H_
#define _VK_H_

//#include <hash.h>

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
	VkBuffer buffer;
	VmaAllocation allocation;
} rbVkBuffer;

typedef struct {
	VkImage image;

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

typedef struct partyRenderer {
	//rbRenderer parent;

	struct rbVkWindow *window;
	struct rbVkDevice *device;
	struct rbVkSwapchain *swapchain;
	struct rbVkCommandQueue *queue;
	struct rbVkCommandQueue *memQueue;	// move this to a dedicated memory struct that handles the heap and all?

	VkCommandBuffer renderCommandBuffer;
	VkPipeline renderPipeline;

	polyBuffer polyBuffer;

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

VkResult createRenderPipelines(partyRenderer *renderer);
void destroyRenderPipelines(partyRenderer *renderer);

uint8_t createShaderFromFile(partyRenderer *renderer, char *filename, VkShaderModule *dst);
VkShaderModule *createShader(partyRenderer *renderer, uint32_t codeSz, uint8_t *code);
void vid2_destroyShader(partyRenderer *renderer, VkShaderModule *shader);

VkResult rbVkInitMemoryManager(rbVKRenderer *renderer, struct rbVkMemoryManager **memManager);
void rbVkDestroyMemoryManager(struct rbVkMemoryManager *memoryManager);

void createBuffer(partyRenderer *renderer, VkDeviceSize size, VkBufferUsageFlags usageFlags, VmaMemoryUsage usage, rbVkBuffer *dst);
void destroyBuffer(partyRenderer *renderer, rbVkBuffer *buffer);
void *mapBuffer(partyRenderer *renderer, rbVkBuffer *buffer);
void unmapBuffer(partyRenderer *renderer, rbVkBuffer *buffer);

#endif