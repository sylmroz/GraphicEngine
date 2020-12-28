#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 texCoord;

uniform samplerCube skybox;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = texture(skybox, texCoord);
}