#version 450 core

#extension GL_ARB_separate_shader_objects : enable

precision mediump float;

layout (triangles) in;
layout (triangle_strip, max_vertices = 54) out;

layout (location = 0) in VS_OUT
{
    mat4 view;
    mat4 viewProjection;
    vec3 normal;
} gs_in[];

layout (std140, binding = 1) uniform Eye
{
    vec4 eyePosition;
} eye;

layout (std140, binding = 2) uniform Time
{
    float timestamp;
} time;

layout (std140) uniform GrassParameters
{
    float thick;
    float height;
    float stiffness;
} grassParameters;

layout (std140, binding = 6) uniform WindParameters
{
    vec2 direcion;
    float speed;
} windParameters;

uniform sampler2D windMap;

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;

// https://github.com/opengl-tutorials/ogl/blob/master/tutorial16_shadowmaps/ShadowMapping.fragmentshader
float random(vec3 seed, float i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return (mod(float(10000 * fract(sin(dot_product)) * 43758.5453), 10000) / 10000.0) - 0.5;
}

void generateStraw(int straw, int chunks, vec3 x1x0, vec3 x2x0, vec3 tangent, vec3 grass_normal, vec3 alongNormal, vec2 windTexel)
{
    float s = sign(tangent.x);
    float rnd = random(floor(gl_in[0].gl_Position.xyz * tangent * 1000.0), 50 * straw) * s;
    float rnd2 = 2.0 * random(floor(gl_in[1].gl_Position.xyz * tangent * 1000.0), straw) * s;
    float rnd3 = 2.0 * random(floor(gl_in[2].gl_Position.xyz * tangent * 1000.0), 2 * straw) * s;

    float heightStep = (grassParameters.height * (1.0 - rnd * 0.25)) / (chunks + 1);
    float bendFactor = (rnd * heightStep / 2.0) * (1.0 - grassParameters.stiffness);
    float thick = grassParameters.thick * ((1.0 - rnd3 * 0.25));

    vec2 windFactor = windTexel * heightStep * windParameters.speed;

    vec3 pos = (x1x0 * rnd2 + x2x0 * rnd3) * grass_normal + gl_in[0].gl_Position.xyz;

    normal = mat3(gs_in[0].view) * grass_normal;
    position = pos - tangent * thick;
    gl_Position = gs_in[0].viewProjection * vec4(position, 1.0);
    EmitVertex();

    position = pos + tangent * thick;
    gl_Position = gs_in[0].viewProjection * vec4(position, 1.0);
    EmitVertex();

    vec3 prevPos = pos;
    float thickFactorStep = 1.0 / (chunks + 1);
    
    for (int i = 1; i < chunks; ++i)
    {
        float weight = thickFactorStep * (i + 1);
        float thickFactor = (1.0 - weight) * thick;
        vec3 y = heightStep * alongNormal;
        vec3 x = grass_normal * bendFactor;

        pos = prevPos + x + y;

        alongNormal = normalize(pos - prevPos);
        pos.xz += windFactor * weight;

        pos -= alongNormal * (distance(prevPos, pos) - length(y));

        grass_normal = normalize(cross(tangent, alongNormal));

        normal = mat3(gs_in[0].view) * grass_normal;
        position = pos - tangent * thickFactor;
        gl_Position = gs_in[0].viewProjection * vec4(position, 1.0);
        EmitVertex();

        position = pos + tangent * thickFactor;
        gl_Position = gs_in[0].viewProjection * vec4(position, 1.0);
        EmitVertex();

        prevPos = pos;
    }
    vec3 y = heightStep * alongNormal;
    vec3 x = grass_normal * bendFactor;

    pos = prevPos + x + y;
    alongNormal = normalize(pos - prevPos);
    pos.xz += windFactor;
    grass_normal = normalize(cross(tangent, alongNormal));

    pos -= alongNormal * (distance(prevPos, pos) - length(y));

    normal = mat3(gs_in[0].view) * grass_normal;
    position = pos;
    gl_Position = gs_in[0].viewProjection * vec4(pos, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    float rnd = random(floor(gl_in[0].gl_Position.xyz * 1000.0), 250).x;
    float dist = length(gl_in[0].gl_Position.xyz - eye.eyePosition.xyz) + rnd;

    vec3 x1x0 = (gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    vec3 x2x0 = (gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);

    vec3 nx1x0 = normalize(x1x0);
    vec3 nx2x0 = normalize(x2x0);

    vec3 tangent = normalize(0.3 * (nx1x0 + nx2x0));

    vec3 alongNormal = (gs_in[0].normal + gs_in[1].normal + gs_in[2].normal) / 3;

    vec3 grass_normal = normalize(cross(tangent, alongNormal));

    float timestamp = time.timestamp * windParameters.speed;
    vec2 texelPosition = vec2(gl_in[0].gl_Position.x + timestamp, gl_in[0].gl_Position.z + timestamp) * windParameters.direcion;
    vec2 windTexel = (2.0 * (texture(windMap, texelPosition).rb - vec2(0.5, 0.5))) * 2;

    int grassStraw = 5 - int(smoothstep(0.0, 4.0, dist));
    int chunks = 4 - int(smoothstep(0.0, 3.0, dist));

    for (int straw = 0; straw < grassStraw; ++straw)
    {
        generateStraw(straw, chunks, x1x0, x2x0, tangent, grass_normal, alongNormal, windTexel);
    }
}