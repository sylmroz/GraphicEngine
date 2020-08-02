#version 430 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 normal;
layout (location = 1) in vec3 fragPosition;

layout (std140, binding = 1) uniform _light
{
    vec3 lightPosition;
    vec3 eyePosition;
} light;

layout (location = 0) out vec4 outColor;

void main()
{
    vec3 lightDir = normalize(light.lightPosition - fragPosition);
    vec3 eyeDirection = normalize(light.eyePosition - fragPosition);
    float I = max(dot(normal, lightDir), 0.0);
    vec3 color = I * vec3(1.0, 0.0, 0.0);
    outColor = vec4(color, 1.0);
}