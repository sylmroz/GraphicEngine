#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in;
layout (triangle_strip, max_vertices = 91) out;

layout (location = 0) in VS_OUT
{
    mat4 projection;
    mat4 view;
    mat3 normalMatrix;
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

layout (std140) uniform WindParameters
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
	return (float(int(10000 * fract(sin(dot_product)) * 43758.5453) % 10000) / 10000.0) - 0.5;
}

void generateStraw(int straw, int chunks)
{
    vec3 x0 = gl_in[0].gl_Position.xyz;
    vec3 x1 = gl_in[1].gl_Position.xyz;
    vec3 x2 = gl_in[2].gl_Position.xyz;

    vec3 x1x0 = (x1 - x0);
    vec3 x2x0 = (x2 - x0);

    vec3 nx1x0 = normalize(x1 - x0);
    vec3 nx2x0 = normalize(x2 - x0);

    vec3 tangent = normalize(0.3 * (nx1x0 + nx2x0));

    vec3 alongNormal = (gs_in[0].normal + gs_in[1].normal + gs_in[2].normal) / 3;

    vec3 grass_normal = normalize(cross(tangent, alongNormal));
    if (grass_normal.z < 0) 
    {
        grass_normal = -grass_normal;
    }

    float rnd = random(floor(x0 * tangent * 1000.0), 50 * straw) * ((tangent.x > 0) ? 1.0: -1.0);
    float rnd2 = 2.0 * random(floor(x1 * tangent * 1000.0), straw) * ((tangent.x > 0) ? 1.0: -1.0);
    float rnd3 = 2.0 * random(floor(x2 * tangent * 1000.0), 2 * straw) * ((tangent.x > 0) ? 1.0: -1.0);

    float heightStep = (grassParameters.height * (1.0 - rnd * 0.25)) / (chunks + 1);
    float bendFactor = (rnd * heightStep / 2.0) * (1.0 - grassParameters.stiffness);
    float thick = grassParameters.thick * ((1.0 - rnd3 * 0.25));

    vec3 pos = gl_in[0].gl_Position.xyz + (x1x0 * rnd2 + x2x0 * rnd3) * grass_normal;
    float texelSize = textureSize(windMap, 0).x;
    float timestamp = time.timestamp / 10;
    vec2 windTexel = (2.0 * (texture(windMap, (vec2(pos.x + timestamp, pos.z + timestamp))).rb - vec2(0.5, 0.5)))/5;
    vec2 windFactor = windTexel * /*vec2(sin(pos.x + time.timestamp), sin(pos.z + time.timestamp)) **/ heightStep;// //(windTexel.rb * windParameters.speed * heightStep) * 10 * vec2(sin(pos.x + timestamp), pos.z + timestamp);

    normal = mat3(gs_in[0].view) * grass_normal;
    position = pos - tangent * thick;
    gl_Position = gs_in[0].projection * gs_in[0].view * vec4(position, 1.0);
    EmitVertex();

    position = pos + tangent * thick;
    gl_Position = gs_in[0].projection * gs_in[0].view * vec4(position, 1.0);
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
        pos.xz += windFactor * weight;// (windFactor * weight * (1.0 - abs(dot(grass_normal, vec3(windParameters.direcion.x, 0.0, windParameters.direcion.y))))) * windParameters.direcion;

        float L = length(prevPos - pos);
        float l = length(y);
        pos = pos - alongNormal * (L - l);

        grass_normal = normalize(cross(tangent, alongNormal));
        if (grass_normal.z < 0)
        {
            grass_normal = -grass_normal;
        }

        normal = mat3(gs_in[0].view) * grass_normal;
        position = pos - tangent * thick;
        gl_Position = gs_in[0].projection * gs_in[0].view * vec4(position, 1.0);
        EmitVertex();

        position = pos + tangent * thick;
        gl_Position = gs_in[0].projection * gs_in[0].view * vec4(position, 1.0);
        EmitVertex();

        prevPos = pos;
    }
    vec3 y = heightStep * alongNormal;
    vec3 x = grass_normal * bendFactor;

    pos = prevPos + x + y;
    alongNormal = normalize(pos - prevPos);
    pos.xz +=  windFactor;//(windFactor * (1.0 - abs(dot(grass_normal, vec3(windParameters.direcion.x, 0.0, windParameters.direcion.y))))) * windParameters.direcion;
    grass_normal = normalize(cross(tangent, alongNormal));
    if (grass_normal.z < 0) 
    {
        grass_normal = -grass_normal;
    }

    float L = length(prevPos - pos);
    float l = length(y);
    pos = pos - alongNormal * (L - l);

    normal = mat3(gs_in[0].view) * grass_normal;
    position = pos;
    gl_Position = gs_in[0].projection * gs_in[0].view * vec4(pos, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    int grassStraw = 7;
    int chunks = 6;
    float rnd = random(floor(gl_in[0].gl_Position.xyz * 1000.0), 250);
    float dist = length(gl_in[0].gl_Position.xyz - eye.eyePosition.xyz) + rnd;
    if (dist >= 3.5 && dist < 7)
    {
        chunks = 5;
        grassStraw = 7;
    }
    else if (dist >= 7 && dist < 11)
    {
        grassStraw = 5;
        chunks = 4;
    }
    else if (dist >= 11 && dist < 15)
    {
        grassStraw = 3;
        chunks = 3;
    }
    else if (dist > 15 && dist < 20)
    {
        chunks = 2;
        grassStraw = 2;
    }
    else if (dist >= 20)
    {
        chunks = 1;
        grassStraw = 1;
    }
    for (int j = 0; j < grassStraw; j++)
    {
        generateStraw(j, chunks);
    }
}