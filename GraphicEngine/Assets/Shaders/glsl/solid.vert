#version 430 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

layout (std140, binding = 0) uniform ViewProjectionMatrix
{
    mat4 viewProjectionMatrix;
} viewProjectionMatrix;

layout(std140) uniform SolidColorModelDescriptor
{
    mat4 modelMatrix;
    mat4 normalMatrix;
    vec3 color;
} solidColorModelDescriptor;

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec3 solidColor;

void main()
{
    solidColor = solidColorModelDescriptor.color;
    normal = normalize(mat3(solidColorModelDescriptor.normalMatrix) * inNormal);
    position = vec3(solidColorModelDescriptor.modelMatrix * vec4(inPosition, 1.0));
    gl_Position = viewProjectionMatrix.viewProjectionMatrix * solidColorModelDescriptor.modelMatrix * vec4(inPosition, 1.0);
}