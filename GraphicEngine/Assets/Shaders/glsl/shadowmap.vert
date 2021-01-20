#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout(std140) uniform LightSpaceModelMatrices
{
    mat4 lightSpace;
    mat4 model;
} lightSpaceModelMatrices;

layout (location = 0) out vec3 position;

void main()
{
    gl_Position = lightSpaceModelMatrices.lightSpace * lightSpaceModelMatrices.model * vec4(position, 1.0);
}