cmake_minimum_required(VERSION 3.10)

project(thps2patch)

set(SRC "${CMAKE_SOURCE_DIR}/src")
set(THIRDPARTY "${CMAKE_SOURCE_DIR}/thirdparty")

set(SOURCE_FILES
	"${SRC}/main.c"
	"${SRC}/patch.c"
	"${SRC}/gfx/gfx.c"
	"${SRC}/gfx/gfx_movie.c"
	"${SRC}/window.c"
	"${SRC}/mem.c"
	"${SRC}/input.c"
	"${SRC}/event.c"
	"${SRC}/window.c"
	"${SRC}/config.c"
	"${SRC}/util/hash.c"
	"${SRC}/util/sb.c"
	# "${SRC}/script.c"
	# "${SRC}/patches.c"
)

set(HEADER_FILES
	"${SRC}/global.h"
	"${SRC}/patch.h"
	"${SRC}/gfx/gfx.h"
	"${SRC}/gfx/gfx_movie.h"
	"${SRC}/gfx/gfx_global.h"
	"${SRC}/window.h"
	"${SRC}/mem.h"
	"${SRC}/input.h"
	"${SRC}/event.h"
	"${SRC}/window.h"
	"${SRC}/config.h"
	"${SRC}/util/hash.h"
	"${SRC}/util/sb.h"
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

set(HEADER_FILES
	${HEADER_FILES}
	"${SRC}/gfx/vk/vk.h"
	"${SRC}/gfx/vk/gfx_vk.h"
)

set(SOURCE_FILES
	${SOURCE_FILES}
	"${SRC}/gfx/vk/gfx_vk_buffer.c"
	"${SRC}/gfx/vk/gfx_vk_cmdbuffer.c"
	"${SRC}/gfx/vk/gfx_vk_descriptorset.c"
	"${SRC}/gfx/vk/gfx_vk_device.c"
	"${SRC}/gfx/vk/gfx_vk_mem.c"
	"${SRC}/gfx/vk/gfx_vk_pipeline.c"
	"${SRC}/gfx/vk/gfx_vk_shader.c"
	"${SRC}/gfx/vk/gfx_vk_swapchain.c"
	"${SRC}/gfx/vk/gfx_vk_texture.c"
	"${SRC}/gfx/vk/gfx_vk_win32.c"
	"${SRC}/gfx/vk/gfx_vk.c"
	"${SRC}/gfx/vk/gfx_vk_shader_data.c"
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
	"${THIRDPARTY}/incbin"
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
	"${THIRDPARTY}/incbin"
)
