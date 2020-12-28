#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 solidColor;

layout (std140) uniform Eye
{
    vec4 eyePosition;
} eye;

layout (std140) uniform Light
{
    vec4 position;
    vec4 color;
} light;

layout (location = 0) out vec4 outColor;

void main()
{
    vec3 lightDir = normalize(position - vec3(eye.eyePosition));
    float I = max(dot(normal, lightDir), 0.0);
    vec3 color = clamp((I + 0.2),0 , 1) * solidColor;
    outColor = vec4(color, 1.0);
}