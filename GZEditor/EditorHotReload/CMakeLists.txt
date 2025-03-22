cmake_minimum_required(VERSION 3.16)
project(GZEditorHotReload)
# Add GZEditor as an executable

message("Building Hot reloader!")
file(GLOB loader_src
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/*.h"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui-node-editor/*.cpp"
    "${CMAKE_SOURCE_DIR}/GZEngine/vendor/imgui-node-editor/*.h"
)

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

add_library(GZEditorHotReload SHARED 
    ${loader_src}
)

target_include_directories(GZEditorHotReload PUBLIC
    ${INCLUDE_LISTS}
    ${VENDOR_INCLUDE_LISTS}
)


# if (WIN32)
#     set_property(TARGET GZEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
# elseif (APPLE)
#     set_target_properties(GZEditor PROPERTIES 
#                       XCODE_GENERATE_SCHEME TRUE
#                       XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
# endif()



# Don't link to GZEditor
target_link_libraries(GZEditorHotReload PRIVATE GZEngine)

set_target_properties(GZEditorHotReload PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)