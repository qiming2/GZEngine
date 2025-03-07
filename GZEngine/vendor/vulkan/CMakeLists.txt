cmake_minimum_required(VERSION 3.16)
message("Use fallback vulkan!")

set(VULKAN_SDK
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vulkan"
)

set(VULKAN_LIB_PATH
    "${VULKAN_SDK}"    
)

if (WIN32)
    set(Vulkan_LIBRARY
        "${VULKAN_LIB_PATH}/windows/vulkan-1.lib"
    )
elseif (UNIX AND NOT APPLE)
    set(Vulkan_LIBRARY
        "${VULKAN_LIB_PATH}/linux/libvulkan.so"
    )
elseif (APPLE)
    set(Vulkan_LIBRARY
        "${VULKAN_LIB_PATH}/macOS/libvulkan.dylib"
    )
endif()
