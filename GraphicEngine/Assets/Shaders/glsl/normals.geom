#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout (location = 0) in VS_OUT
{
    mat4 projection;
    vec3 normal;
} gs_in[];

const float magnitude = 0.05;

void generateLine(int index)
{
    gl_Position = gs_in[index].projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gs_in[index].projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * magnitude);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    generateLine(0);
    generateLine(1);
    generateLine(2);
    EndPrimitive();
}