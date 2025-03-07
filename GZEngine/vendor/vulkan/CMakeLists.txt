cmake_minimum_required(VERSION 3.16)
message("Building vulkan!")

add_library(vulkan INTERFACE)
set(VULKAN_SDK
    "${CMAKE_CURRENT_SOURCE_DIR}/1.4.304.1"
)

set(VULKAN_LIB_PATH
    "${VULKAN_SDK}/Lib"    
)

if (WIN32)
    set(VULKAN_LIB
        "${VULKAN_LIB_PATH}/windows/vulkan-1.lib"
    )
elseif (UNIX AND NOT APPLE)
    set(VULKAN_LIB
        "${VULKAN_LIB_PATH}/linux/libvulkan.so"
    )
elseif (APPLE)
    set(VULKAN_LIB
        "${VULKAN_LIB_PATH}/macOS/libvulkan.dylib"
    )
endif()

target_include_directories(vulkan INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/1.4.304.1/Include")
target_link_libraries(vulkan INTERFACE "${VULKAN_LIB}")