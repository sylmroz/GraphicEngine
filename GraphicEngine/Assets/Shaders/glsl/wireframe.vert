#version 430 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;

layout (std140, binding = 0) uniform ViewProjectionMatrix
{
    mat4 viewProjectionMatrix;
} viewProjectionMatrix;

layout(std140) uniform WireframeModelDescriptor
{
    mat4 modelMatrix;
    vec3 wireframeColor;
} wireframeModelDescriptor;

layout (location = 0) out vec3 wireframeColor;

void main()
{
    wireframeColor = wireframeModelDescriptor.wireframeColor;
    gl_Position = viewProjectionMatrix.viewProjectionMatrix * wireframeModelDescriptor.modelMatrix * vec4(inPosition, 1.0);
}