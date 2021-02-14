#version 450 core

#extension GL_ARB_separate_shader_objects : enable

#define LIGHT_COUNT 5

layout (triangles, invocations = LIGHT_COUNT) in;
layout (triangle_strip, max_vertices = 3) out;

layout (std140) uniform LightSpaceMatrixArray
{
    mat4 lightSpace[LIGHT_COUNT];
} lightSpaceMatrix;

void main()
{
    for (int i = 0; i < gl_in.length(); ++i)
    {
        gl_Layer = gl_InvocationID;
        gl_Position = lightSpaceMatrix.lightSpace[gl_InvocationID] * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}