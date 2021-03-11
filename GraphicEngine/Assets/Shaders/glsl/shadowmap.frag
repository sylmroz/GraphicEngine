#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in float fragDepth;

void main()
{
    gl_FragDepth = fragDepth;
}