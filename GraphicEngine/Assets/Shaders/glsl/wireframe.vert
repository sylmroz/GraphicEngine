#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;

layout (std140, binding = 0) uniform CameraMatrices
{
    mat4 view;
    mat4 projection;
} cameraMatrices;

layout(std140) uniform WireframeModelDescriptor
{
    mat4 modelMatrix;
    vec4 wireframeColor;
} wireframeModelDescriptor;

layout (location = 0) out vec3 wireframeColor;

void main()
{
    wireframeColor = vec3(wireframeModelDescriptor.wireframeColor);
    gl_Position = cameraMatrices.projection * cameraMatrices.view * wireframeModelDescriptor.modelMatrix * vec4(inPosition, 1.0);
}