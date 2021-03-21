#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;

layout (std140) uniform LightSpaceModelMatrices
{
    mat4 lightSpace;
    mat4 model;
} lightSpaceModelMatrices;

layout (std140) uniform ModelMatrix
{
    mat4 model;
} modelMatrix;

void main()
{
    gl_Position = modelMatrix.model * vec4(inPosition, 1.0);
}