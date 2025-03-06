cmake_minimum_required(VERSION 3.16)
message("Including vulkan!")

add_library(vulkan INTERFACE)
set(VULKAN_SDK 
    "${CMAKE_CURRENT_SOURCE_DIR}/1.4.304.1")

if (WIN32)
    set(VULKAN_LIB
        "${VULKAN_SDK}/Lib/vulkan-1.lib"
    )
elseif (UNIX AND NOT APPLE)
    set(VULKAN_LIB
        "${VULKAN_SDK}/Lib/libvulkan.so"
    )
elseif (APPLE)
    set(VULKAN_LIB
        "${VULKAN_SDK}/Lib/libvulkan.dylib"
    )
endif()

target_include_directories(vulkan INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/1.4.304.1/Include")
target_link_libraries(vulkan INTERFACE "${VULKAN_LIB}")