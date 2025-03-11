cmake_minimum_required(VERSION 3.16)
project(GZWorkspace)
message("Starting building the project...")

# Set configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "Configurations" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("linker configs...")
# Debug configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGZ_DEBUG")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

# Release configuration
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DGZ_RELEASE")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Dist configuration
set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_DIST} -DGZ_DIST")
set(CMAKE_EXE_LINKER_FLAGS_DIST "${CMAKE_EXE_LINKER_FLAGS_DIST}")

# # Dist Shared configuration
set(CMAKE_CXX_FLAGS_SHARED_DIST "${CMAKE_CXX_FLAGS_SHARED_DIST} -DGZ_SHARED_DIST")
set(CMAKE_SHARED_LINKER_FLAGS_DIST "${CMAKE_SHARED_LINKER_FLAGS_DIST}")

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()
# Have to come after configuration settings!
add_subdirectory("${CMAKE_SOURCE_DIR}/GZEngine")
add_subdirectory("${CMAKE_SOURCE_DIR}/GZEditor")

message("Global configs...")
# Define platform-specific settings
if(WIN32)
    set(PLATFORM_DEFINES "GZ_PLATFORM_WINDOWS")
elseif(APPLE)
    set(PLATFORM_DEFINES "GZ_PLATFORM_APPLE")
endif()

# Platform-specific definitions
target_compile_definitions(GZEngine PUBLIC ${PLATFORM_DEFINES})
target_compile_definitions(GZEditor PUBLIC ${PLATFORM_DEFINES})

add_compile_definitions("$<$<CONFIG:Debug>:GZ_DEBUG>")
add_compile_definitions("$<$<CONFIG:Release>:GZ_RELEASE>")
add_compile_definitions("$<$<CONFIG:Dist>:GZ_DIST>")

message("Properties configs...")
# Configuration-specific settings
set_target_properties(GZEngine GZEditor PROPERTIES
    DEBUG_POSTFIX "_d"
    RELEASE_POSTFIX "_r"
    DIST_POSTFIX ""
)


# message("Finished...")
if (MSVC)
    # set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GZEditor)
endif()