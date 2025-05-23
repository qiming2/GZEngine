cmake_minimum_required(VERSION 3.16)
project(GZEditor)
# Add GZEditor as an executable

message("Building editor!")
file(GLOB editor_src
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
add_executable(GZEditor 
    ${editor_src}
)

if (WIN32)
    set_property(TARGET GZEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
elseif (APPLE)
    set_target_properties(GZEditor PROPERTIES 
                      XCODE_GENERATE_SCHEME TRUE
                      XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# Link GZEngine to GZEditor
target_link_libraries(GZEditor PRIVATE GZEngine)

# This is defined in main CMake
# set_target_properties(GZEditor PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
# )