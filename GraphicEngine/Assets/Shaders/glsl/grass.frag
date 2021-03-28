#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) out vec4 outColor;

vec3 lightDir = vec3(1.0, -1.0, 1.0);

void main()
{
    float I = max(dot(normal, -normalize(lightDir)), 0.0);
    vec3 diffuse = I * vec3(0.07058f, 0.509803f, 0.078431f);
    outColor = vec4(diffuse, 0.85f);
}