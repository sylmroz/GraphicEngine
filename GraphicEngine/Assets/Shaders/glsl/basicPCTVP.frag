#version 430 core

#extension GL_ARB_separate_shader_objects : enable

uniform sampler2D texture1;

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec2 texCoord;

layout (location = 0) out vec4 outColor;

void main()
{
    outColor = mix(texture(texture1, texCoord), vec4(fragColor, 1.0), 0.2);
}