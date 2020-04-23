#version 430 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

uniform mat4 MVP;

layout (location = 0) out vec3 fragColor;

void main()
{
    fragColor = inColor;
    gl_Position = MVP * vec4(inPosition, 1.0);
}