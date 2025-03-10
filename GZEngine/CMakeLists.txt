cmake_minimum_required(VERSION 3.16)
project(GZEngine)

message("Building engine!")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/flecs")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/VulkanMemoryAllocator")

set(Vulkan_FOUND OFF)
find_package(Vulkan)
# Instead of adding vulkan which is huge, find the package
# In order to use vulkan validation lib, needs to let the installer set the path
# For debug version, need to have installer
if (NOT Vulkan_FOUND)
    find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)
    IF (Vulkan_LIBRARY)
        set(Vulkan_FOUND ON)
        MESSAGE("Using bundled Vulkan library version")
    ENDIF()
ENDIF()

if (NOT Vulkan_FOUND)
    include("${CMAKE_CURRENT_SOURCE_DIR}/vendor/vulkan/CMakeLists.txt")
endif()

message("Vulkan found: ${Vulkan_LIBRARY}")

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/bigobj>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")
endif()
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm")
target_compile_definitions(glm INTERFACE GLM_FORCE_DEPTH_ZERO_TO_ONE=ON)
target_compile_definitions(glm INTERFACE GLM_FORCE_RADIANS=ON)
target_compile_definitions(glm INTERFACE GLM_ENABLE_CXX_17=ON)
target_compile_definitions(glm INTERFACE GLM_FORCE_SWIZZLE=ON)

# Include directories
list(APPEND INCLUDE_MAPS
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/flecs/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vulkan/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui-node-editor"
)

foreach(HEADER ${INCLUDE_MAPS_HEADERS})
    message(STATUS "Found header file: ${HEADER}")
endforeach()

# Library directories
set(LIBDIR_MAPS
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3/lib/"
)
if (MSV)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(OpenGL REQUIRED)

file(GLOB vendorSources
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vulkan/Include/vulkan/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3/include/SDL3/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp/imgui_stdlib/*.cpp"
    # Imgui backend vulkan + sdl3
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_sdl3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_vulkan.cpp"
    # -- opengl to start, will delete later
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui-node-editor/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui-node-editor/*.h"
)

file(GLOB_RECURSE generatedSources 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/"
)


# Define platform-specific settings
if(WIN32)
    set(SDL3_LIBRARY "${LIBDIR_MAPS}/windows/SDL3.lib")
    # set(OPENGL_LIBRARY "opengl32.lib")
elseif(APPLE)
    set(SDL3_LIBRARY "${LIBDIR_MAPS}/mac/libSDL3.dylib")
    
endif()

add_library(GZEngine STATIC
    ${generatedSources}
    ${vendorSources}
)

target_precompile_headers(GZEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/gzpch.h")

source_group("Vendor sources" FILES ${vendorSources})


# Include directories for GZEngine
target_include_directories(GZEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${INCLUDE_MAPS}
)

target_link_libraries(GZEngine PUBLIC
    ${SDL3_LIBRARY}
    ${OPENGL_LIBRARY}
    flecs::flecs_static
    ${Vulkan_LIBRARY}
    glm::glm
    PRIVATE GPUOpen::VulkanMemoryAllocator
)