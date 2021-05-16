#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

layout (std140, binding = 0) uniform CameraMatrices
{
    mat4 view;
    mat4 projection;
} cameraMatrices;

layout(std140) uniform ModelMartices
{
    mat4 modelMatrix;
    mat4 normalMatrix;
} modelMartices;

layout (location = 0) out VS_OUT
{
    mat4 projection;
    mat4 view;
    mat3 normalMatrix;
    vec3 normal;
} vs_out;

void main()
{
    gl_Position = modelMartices.modelMatrix * vec4(inPosition, 1.0);
    vs_out.normal = mat3(modelMartices.normalMatrix) * inNormal;
    vs_out.projection = cameraMatrices.projection;
    vs_out.view = cameraMatrices.view;
    vs_out.normalMatrix = mat3(modelMartices.normalMatrix);
}