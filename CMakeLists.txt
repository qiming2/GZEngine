cmake_minimum_required(VERSION 3.16)
project(GZEngine)

add_subdirectory("${CMAKE_SOURCE_DIR}/GZEngine/vendor/flecs")
# Set configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "Configurations" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Output directories
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}")
set(OBJECTS_DIR "${CMAKE_BINARY_DIR}/bin-obj/${CMAKE_CFG_INTDIR}")

# Include directories
set(INCLUDE_MAPS
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/spdlog/include"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/SDL3/include"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/backends"
    "${CMAKE_SOURCE_DIR}/GZEngine"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/misc/cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/flecs/include"
)

# Library directories
set(LIBDIR_MAPS
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/SDL3/lib/"
)
if (MSV)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(OpenGL REQUIRED)
# Define platform-specific settings
if(WIN32)
    set(PLATFORM_DEFINES "GZ_PLATFORM_WINDOWS")
    set(SDL3_LIBRARY "${LIBDIR_MAPS}/windows/SDL3.lib")
    # set(OPENGL_LIBRARY "opengl32.lib")
elseif(APPLE)
    set(PLATFORM_DEFINES "GZ_PLATFORM_APPLE")
    set(SDL3_LIBRARY "${LIBDIR_MAPS}/mac/libSDL3.dylib")
    
endif()

# target_sources(SDL3-shared PRIVATE ${shared_sources} ${both_sources})
# Add GZEngine as a static library

file(GLOB generatedSources 
    "${CMAKE_SOURCE_DIR}/GZEngine/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Core/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Core/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/misc/cpp/imgui_stdlib/*.cpp"
    # Imgui backend vulkan + sdl3
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/backends/imgui_impl_sdl3.cpp"
    # "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui_impl_vulkan.cpp"
    -- opengl to start, will delete later
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/backends/imgui_impl_opengl3.cpp"
)

add_library(GZEngine STATIC
    ${generatedSources}
)
target_sources(GZEngine PRIVATE ${generatedSources})


# Include directories for GZEngine
target_include_directories(GZEngine PUBLIC
    "${CMAKE_SOURCE_DIR}/GZEngine/src"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Core"
    "${CMAKE_SOURCE_DIR}/GZEngine/include"
    ${INCLUDE_MAPS}
)

# Link libraries for GZEngine
target_link_libraries(GZEngine
    ${SDL3_LIBRARY}
    ${OPENGL_LIBRARY}
    flecs::flecs_static
)

project(GZEditor)
# Add GZEditor as an executable
file(GLOB editor_src
    "${CMAKE_SOURCE_DIR}/GZEditor/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEditor/src/*.h"
)
add_executable(GZEditor 
    ${editor_src}
)

if (WIN32)
    set_property(TARGET GZEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")
elseif (APPLE)
    set_target_properties(GZEditor PROPERTIES 
                      XCODE_GENERATE_SCHEME TRUE
                      XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")
endif()
target_sources(GZEditor PRIVATE ${editor_src})

# Include directories for GZEditor
target_include_directories(GZEditor PUBLIC
    "${CMAKE_SOURCE_DIR}/GZEngine/src"
    "${CMAKE_SOURCE_DIR}/GZEngine/src/Core"
    "${CMAKE_SOURCE_DIR}/GZEngine/include"
    ${INCLUDE_MAPS}
)

# Link GZEngine to GZEditor
target_link_libraries(GZEditor PUBLIC GZEngine)

# Platform-specific definitions
target_compile_definitions(GZEngine PUBLIC ${PLATFORM_DEFINES})
target_compile_definitions(GZEditor PUBLIC ${PLATFORM_DEFINES})

# Configuration-specific settings
set_target_properties(GZEngine GZEditor PROPERTIES
    DEBUG_POSTFIX "_d"
    RELEASE_POSTFIX ""
    DIST_POSTFIX "_dist"
)

# Debug configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGZ_DEBUG")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

# Release configuration
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DGZ_RELEASE")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Dist configuration
set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_DIST} -DGZ_DIST")
set(CMAKE_EXE_LINKER_FLAGS_DIST "${CMAKE_EXE_LINKER_FLAGS_DIST}")

if (MSVC)
    # set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GZEditor)
endif()

if (WIN32)
# Post-build commands (e.g., copy SDL3 DLL for Windows)
add_custom_command(TARGET GZEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${LIBDIR_MAPS}/windows/SDL3.dll"
    "$<TARGET_FILE_DIR:GZEditor>"
)

add_custom_command(TARGET GZEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${LIBDIR_MAPS}/windows/SDL3.pdb"
    "$<TARGET_FILE_DIR:GZEditor>"
)
endif()