#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;

layout (std140, binding = 0) uniform CameraMatrices
{
    mat4 view;
    mat4 projection;
} cameraMatrices;

layout (location = 0) out vec3 texCoord;

void main()
{
    texCoord = inPosition;
    mat4 view = mat4(mat3(cameraMatrices.view));
    vec4 position = cameraMatrices.projection * view * vec4(inPosition * 250, 1.0);
    gl_Position = position.xyww;
}