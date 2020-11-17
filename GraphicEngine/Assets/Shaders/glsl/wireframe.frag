#version 430 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 wireframeColor;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(wireframeColor, 1.0);
}