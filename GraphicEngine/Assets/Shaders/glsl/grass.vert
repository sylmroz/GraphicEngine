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
    vec3 fragPosition;
    vec3 normal;
} vs_out;

void main()
{
    vs_out.fragPosition = vec3(modelMartices.modelMatrix * vec4(inPosition, 1.0));
    gl_Position = cameraMatrices.view * modelMartices.modelMatrix * vec4(inPosition, 1.0);
    vs_out.normal = normalize(mat3(modelMartices.normalMatrix) * inNormal);
    vs_out.projection = cameraMatrices.projection;
}