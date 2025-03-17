cmake_minimum_required(VERSION 3.16)
project(GZEngine)

message("Building engine!")

message("Flecs...!")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/flecs")
message("Vma...!")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/VulkanMemoryAllocator")
message("glm...!")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm")
target_compile_definitions(glm INTERFACE GLM_FORCE_DEPTH_ZERO_TO_ONE=ON)
target_compile_definitions(glm INTERFACE GLM_FORCE_RADIANS=ON)
target_compile_definitions(glm INTERFACE GLM_ENABLE_CXX_17=ON)
target_compile_definitions(glm INTERFACE GLM_FORCE_SWIZZLE=ON)

message("Jolt...!")
set(CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/JoltPhysics/Build")
target_compile_definitions(Jolt PUBLIC DOUBLE_PRECISION=OFF)
# target_compile_definitions(Jolt PUBLIC GENERATE_DEBUG_SYMBOLS=ON)
# target_compile_definitions(Jolt PUBLIC OVERRIDE_CXX_FLAGS=ON)
# target_compile_definitions(Jolt PUBLIC CROSS_PLATFORM_DETERMINISTIC=OFF)
# target_compile_definitions(Jolt PUBLIC INTERPROCEDURAL_OPTIMIZATION=ON)
# target_compile_definitions(Jolt PUBLIC FLOATING_POINT_EXCEPTIONS_ENABLED=OFF)
# target_compile_definitions(Jolt PUBLIC CPP_EXCEPTIONS_ENABLED=OFF)
# target_compile_definitions(Jolt PUBLIC CPP_RTTI_ENABLED=OFF)
# target_compile_definitions(Jolt PUBLIC OBJECT_LAYER_BITS=16)


set(Vulkan_FOUND OFF)
find_package(Vulkan REQUIRED)
# Instead of adding vulkan which is huge, find the package
# In order to use vulkan validation lib, needs to let the installer set the path
# For debug version, need to have installer
set(GZ_VULKAN_VERSION, "1.4.304.1")
if (NOT Vulkan_FOUND)
    message("You don't have VulkanSDK please download version or above: ${GZ_VULKAN_VERSION} at
        https://vulkan.lunarg.com/")
endif()

if (NOT Vulkan_FOUND)
    include("${CMAKE_CURRENT_SOURCE_DIR}/vendor/vulkan/CMakeLists.txt")
endif()

message("Vulkan found: ${Vulkan_LIBRARY}")

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/bigobj>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")
endif()

# Include directories
list(APPEND INCLUDE_MAPS
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/misc/cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/flecs/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui-node-editor"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tiny_obj/include"
    "${Vulkan_INCLUDE_DIRS}"
    "${JoltPhysics_SOURCE_DIR}/.."
)

# Library directories
set(LIBDIR_MAPS
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3/lib/"
)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tiny_obj/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tiny_obj/include/*.h"
)

file(GLOB_RECURSE sources 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Physics/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Physics/*.h"
)


# Define platform-specific settings
if(WIN32)
    set(SDL3_LIBRARY "${LIBDIR_MAPS}/windows/SDL3.lib")
elseif(APPLE)
    set(SDL3_LIBRARY "${LIBDIR_MAPS}/mac/libSDL3.dylib")
endif()

message("SDL3 lib: ${SDL3_LIBRARY}")

add_library(GZEngine STATIC
    ${sources}
    ${vendorSources}
)

target_precompile_headers(GZEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/gzpch.h")

source_group("Vendor sources" FILES ${vendorSources})

# Include directories for GZEngine
target_include_directories(GZEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Physics"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${INCLUDE_MAPS}
)

target_link_libraries(GZEngine PUBLIC
    ${SDL3_LIBRARY}
    flecs::flecs_static
    ${Vulkan_LIBRARIES}
    glm::glm
    GPUOpen::VulkanMemoryAllocator
    Jolt
)