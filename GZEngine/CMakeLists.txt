cmake_minimum_required(VERSION 3.16)
project(GZEngine)

message("Building engine!")

message("Flecs...!")
add_subdirectory("${CMAKE_SOURCE_DIR}/GZEngine/vendor/flecs")

message("Vma...!")
add_subdirectory("${CMAKE_SOURCE_DIR}/GZEngine/vendor/VulkanMemoryAllocator")

message("glm...!")
add_subdirectory("${CMAKE_SOURCE_DIR}/GZEngine/vendor/glm")
target_compile_definitions(glm INTERFACE GLM_FORCE_DEPTH_ZERO_TO_ONE=ON)
target_compile_definitions(glm INTERFACE GLM_FORCE_RADIANS=ON)
target_compile_definitions(glm INTERFACE GLM_ENABLE_CXX_17=ON)
target_compile_definitions(glm INTERFACE GLM_FORCE_SWIZZLE=ON)

message("Jolt...!")
set(CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
add_subdirectory("${CMAKE_SOURCE_DIR}/GZEngine/vendor/JoltPhysics/Build")
target_compile_definitions(Jolt PUBLIC DOUBLE_PRECISION=OFF)
# target_compile_definitions(Jolt PUBLIC GENERATE_DEBUG_SYMBOLS=ON)
# target_compile_definitions(Jolt PUBLIC OVERRIDE_CXX_FLAGS=ON)
# target_compile_definitions(Jolt PUBLIC CROSS_PLATFORM_DETERMINISTIC=OFF)
# target_compile_definitions(Jolt PUBLIC INTERPROCEDURAL_OPTIMIZATION=ON)
# target_compile_definitions(Jolt PUBLIC FLOATING_POINT_EXCEPTIONS_ENABLED=OFF)
target_compile_definitions(Jolt PUBLIC CPP_EXCEPTIONS_ENABLED=OFF)
target_compile_definitions(Jolt PUBLIC CPP_RTTI_ENABLED=OFF)
target_compile_definitions(Jolt PUBLIC JPH_OBJECT_LAYER_BITS=32)

message("SDL...!")
set(SDL_SHARED ON)
set(SDL_STATIC OFF)
set(SDL_TEST_LIBRARY OFF)
add_subdirectory("${CMAKE_SOURCE_DIR}/GZEngine/vendor/SDL")


set(Vulkan_FOUND OFF)
find_package(Vulkan)
# Instead of adding vulkan which is huge, find the package
# In order to use vulkan validation lib, needs to let the install er set the path
# For debug version, need to have installer
set(GZ_VULKAN_VERSION, "1.4.304.1")
   
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "You don't have VulkanSDK please download version or above: 1.4.304.1 at
        https://vulkan.lunarg.com/")
endif()

if (NOT Vulkan_FOUND)
    include("${CMAKE_SOURCE_DIR}/GZEngine/vendor/vulkan/CMakeLists.txt")
endif()

message("Vulkan found: ${Vulkan_LIBRARY}")

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/bigobj>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")
endif()

# Vendor Include directories
list(APPEND VENDOR_INCLUDE_LISTS
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/spdlog/include"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/SDL/include"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/backends"
    "${CMAKE_SOURCE_DIR}/GZEngine"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/misc/cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/flecs/include"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui-node-editor"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/stb/include"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/tiny_obj/include"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/cr"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/glm/glm"
    "${Vulkan_INCLUDE_DIRS}"
    "${JoltPhysics_SOURCE_DIR}/.."
)

file(GLOB vendorSources
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/vulkan/Include/vulkan/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/SDL/include/SDL3/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/misc/cpp/imgui_stdlib/*.cpp"
    # Imgui backend vulkan + sdl3
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/backends/imgui_impl_sdl3.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/backends/imgui_impl_vulkan.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui-node-editor/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui-node-editor/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/stb/include/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/stb/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/tiny_obj/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/tiny_obj/include/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/VulkanMemoryAllocator/include/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/cr/*.h"
)

# Engine include
list(APPEND INCLUDE_LISTS
    "${CMAKE_SOURCE_DIR}/GZEngine/src"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Core"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Renderer"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Physics"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Mesh"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Common"
    "${CMAKE_SOURCE_DIR}/GZEngine/include"
)

file(GLOB_RECURSE sources 
    "${CMAKE_SOURCE_DIR}/GZEngine/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Core/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Core/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Renderer/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Renderer/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Physics/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Physics/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Mesh/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Mesh/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Common/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Common/*.h"
)

add_library(GZEngine SHARED
    ${sources}
    ${vendorSources}
)

target_precompile_headers(GZEngine PUBLIC "${CMAKE_SOURCE_DIR}/GZEngine/src/gzpch.h")

source_group("Vendor sources" FILES ${vendorSources})

# Include directories for GZEngine
target_include_directories(GZEngine PUBLIC
    ${INCLUDE_LISTS}
    ${VENDOR_INCLUDE_LISTS}
)

target_link_libraries(GZEngine PUBLIC
    SDL3::SDL3-shared #default to shared sdl3
    flecs::flecs
    ${Vulkan_LIBRARIES}
    glm::glm
    GPUOpen::VulkanMemoryAllocator
    Jolt
)

set_target_properties(flecs SDL3-shared PROPERTIES
    DEBUG_POSTFIX "_d"
    RELEASE_POSTFIX "_r"
    DIST_POSTFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)