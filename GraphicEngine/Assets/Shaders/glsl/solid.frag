#version 430 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 solidColor;

layout (std140) uniform Eye
{
    vec3 eyePosition;
} eye;

layout (std140) uniform Light
{
    vec3 position;
    vec3 color;
} light;

layout (location = 0) out vec4 outColor;

void main()
{
    vec3 lightDir = normalize(eye.eyePosition - position);
    float I = max(dot(normal, lightDir), 0.0);
    vec3 color = (I + 0.05) * solidColor;
    outColor = vec4(color, 1.0);
}