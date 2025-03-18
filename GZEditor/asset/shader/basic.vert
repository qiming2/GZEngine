#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout( push_constant ) uniform constants
{
    mat4 model;
} pc;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_uv;

layout(location = 0) out vec3 out_color;
layout(location = 1) out vec2 out_uv;

void main() {
    gl_Position = ubo.proj * ubo.view * pc.model * vec4(in_position, 1.0);
    out_color = in_color;
    out_uv = in_uv;
}