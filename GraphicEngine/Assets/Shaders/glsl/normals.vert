#version 440 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

layout (std140, binding = 0) uniform ViewProjectionMatrix
{
    mat4 viewProjectionMatrix;
} viewProjectionMatrix;

layout(std140) uniform ModelMartices
{
    mat4 modelMatrix;
    mat4 normalMatrix;
} modelMartices;

layout (location = 0) out VS_OUT
{
    vec3 normal;
} vs_out;

void main()
{
    gl_Position = viewProjectionMatrix.viewProjectionMatrix * modelMartices.modelMatrix * vec4(inPosition, 1.0);
    vs_out.normal = normalize(mat3(modelMartices.normalMatrix) * inNormal);
}
