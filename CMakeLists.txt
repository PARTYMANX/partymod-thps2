cmake_minimum_required(VERSION 3.10)

project(thps2patch)

set(SRC "${CMAKE_SOURCE_DIR}/src")
set(THIRDPARTY "${CMAKE_SOURCE_DIR}/thirdparty")

set(SOURCE_FILES
	"${SRC}/main.c"
	"${SRC}/patch.c"
	"${SRC}/gfx.c"
	"${SRC}/window.c"
	# "${SRC}/config.c"
	# "${SRC}/input.c"
	# "${SRC}/hash.c"
	# "${SRC}/script.c"
	# "${SRC}/patches.c"
)

set(HEADER_FILES
	"${SRC}/global.h"
	"${SRC}/patch.h"
	"${SRC}/gfx.h"
	"${SRC}/window.h"
	# "${SRC}/config.h"
	# "${SRC}/input.h"
	# "${SRC}/hash.h"
	# "${SRC}/script.h"
)

list(APPEND INCLUDES ${THIRDPARTY})

# add sdl2
find_package(SDL2 REQUIRED)
list(APPEND LIBS 
	SDL2::SDL2
	SDL2::SDL2main
)
list(APPEND INCLUDES ${SDL2_INCLUDE_DIRS})

# add libraries for vulkan renderer

# add vulkan
find_package(Vulkan REQUIRED)
list(APPEND VKLIBS ${Vulkan_LIBRARIES})
list(APPEND VKINCLUDES ${Vulkan_INCLUDE_DIRS})

# add vk_mem_alloc
add_library(vk_mem_alloc "${PROJECT_SOURCE_DIR}/thirdparty/VulkanMemoryAllocator/vk_mem_alloc.cpp")
target_link_libraries(vk_mem_alloc PUBLIC ${Vulkan_LIBRARIES})
target_include_directories(vk_mem_alloc PUBLIC ${Vulkan_INCLUDE_DIRS})
list(APPEND VKLIBS vk_mem_alloc)
list(APPEND VKINCLUDES "${PROJECT_SOURCE_DIR}/thirdparty/VulkanMemoryAllocator")

set(SOURCE_FILES
	${SOURCE_FILES}
	#"${SRC}/gfx/vk/vid_vk.h"
	"${SRC}/gfx/vk/vk.h"
	# new things go here
	"${SRC}/gfx/vk/gfx_vk.h"
)

set(SOURCE_FILES
	${SOURCE_FILES}
	"${SRC}/gfx/vk/vid_vk_buffer.c"
	"${SRC}/gfx/vk/vid_vk_cmdbuffer.c"
	#"${SRC}/gfx/vk/vid_vk_descriptorset.c"
	"${SRC}/gfx/vk/vid_vk_device.c"
	#"${SRC}/gfx/vk/vid_vk_framebuffer.c"
	"${SRC}/gfx/vk/vid_vk_mem.c"
	"${SRC}/gfx/vk/vid_vk_pipeline.c"
	#"${SRC}/gfx/vk/vid_vk_renderer.c"
	#"${SRC}/gfx/vk/vid_vk_renderpass.c"
	"${SRC}/gfx/vk/vid_vk_shader.c"
	"${SRC}/gfx/vk/vid_vk_swapchain.c"
	#"${SRC}/gfx/vk/vid_vk_texture.c"
	"${SRC}/gfx/vk/vid_vk_win32.c"
	# new things go here
	"${SRC}/gfx/vk/gfx_vk.c"
)

add_library(partymod SHARED)

target_sources(partymod PRIVATE
	${SOURCE_FILES}
	${HEADER_FILES}
)

target_link_libraries(partymod PUBLIC 
	${LIBS}
	${VKLIBS}
)

target_include_directories(partymod PUBLIC
	"${PROJECT_SOURCE_DIR}/src"
	${VKINCLUDES}
)

add_executable(partyconfig)

target_sources(partyconfig PUBLIC config/main.c)
target_link_libraries(partyconfig PRIVATE 
	${LIBS}
	comctl32.lib
	UxTheme.lib
)
target_include_directories(partyconfig PUBLIC
	"${PROJECT_SOURCE_DIR}/src"
	${INCLUDES}
)

add_executable(incbin)

target_sources(incbin PUBLIC 
	"${THIRDPARTY}/incbin/incbin.c"
	"${THIRDPARTY}/incbin/incbin.h"
)

add_executable(partypatcher)

target_sources(partypatcher PUBLIC
	"${CMAKE_SOURCE_DIR}/patcher/main.c"
	"${CMAKE_SOURCE_DIR}/patcher/data.c"
)

target_include_directories(partypatcher PUBLIC
	"${CMAKE_SOURCE_DIR}/src"
	${THIRDPARTY}
)
