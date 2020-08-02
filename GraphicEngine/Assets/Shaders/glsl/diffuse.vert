#version 430 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

layout (std140, binding = 0) uniform MVP
{
    mat4 vp; 
} mvp;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 fragPosition;

void main()
{
    outNormal = normalize(inNormal);
    fragPosition = inPosition;
    gl_Position = mvp.vp * vec4(inPosition, 1.0);
}