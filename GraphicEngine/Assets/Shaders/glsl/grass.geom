#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in;
//layout (points) in;
layout (triangle_strip, max_vertices = 54) out;

layout (location = 0) in VS_OUT
{
    mat4 projection;
    mat4 view;
    vec3 normal;
} gs_in[];

layout (std140) uniform GrassParameters
{
    float thick;
    float height;
    int muberOfGrassPerUnit;
} grassParameters;

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;

// https://github.com/opengl-tutorials/ogl/blob/master/tutorial16_shadowmaps/ShadowMapping.fragmentshader
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return (float(int(10000 * fract(sin(dot_product)) * 43758.5453) % 10000) / 10000.0) - 0.5;
}

void generateStraw(int straw)
{
    vec3 x0 = gl_in[0].gl_Position.xyz;
    vec3 x1 = gl_in[1].gl_Position.xyz;
    vec3 x2 = gl_in[2].gl_Position.xyz;

    vec3 x1x0 = (x1 - x0);
    vec3 x2x0 = (x2 - x0);

    vec3 nx1x0 = normalize(x1 - x0);
    vec3 nx2x0 = normalize(x2 - x0);

    vec3 alongNormal = (gs_in[0].normal + gs_in[1].normal + gs_in[2].normal) / 3;

    vec3 tangent = normalize(0.3 * (nx1x0 + nx2x0));

    vec3 grass_normal = normalize(cross(tangent, alongNormal));
    if (grass_normal.z < 0) 
    {
        grass_normal = -grass_normal;
    }

    float heightStep = grassParameters.height / 4;
    float rnd = random(floor(x0 * tangent * 1000.0), 50 * straw) * ((tangent.x > 0) ? 1.0: -1.0);
    float rnd2 = 2.0 * random(floor(x1 * tangent * 1000.0), straw) * ((tangent.x > 0) ? 1.0: -1.0);
    float rnd3 = 2.0 * random(floor(x2 * tangent * 1000.0), 2 * straw) * ((tangent.x > 0) ? 1.0: -1.0);
    int index = int(16.0 * rnd) % 16;
    float bendFactor = rnd * heightStep / 2.0;

    vec3 pos = gl_in[0].gl_Position.xyz + (x1x0 * rnd2 + x2x0*rnd3) * grass_normal;

    normal = mat3(gs_in[0].view) * normalize(alongNormal + 0.2 * grass_normal);
    
    gl_Position = gs_in[0].projection * gs_in[0].view * vec4(pos - tangent * grassParameters.thick, 1.0);
    EmitVertex();

    gl_Position = gs_in[0].projection * gs_in[0].view * vec4(pos + tangent * grassParameters.thick, 1.0);
    EmitVertex();

    vec3 prevPos = pos;
    
    for (int i = 1; i < 4; ++i)
    {
        vec3 y = heightStep * alongNormal;
        vec3 x = grass_normal * bendFactor;

        pos = prevPos + x + y;

        alongNormal = normalize(pos - prevPos);
        grass_normal = normalize(cross(tangent, alongNormal));
        if (grass_normal.z < 0)
        {
            grass_normal = -grass_normal;
        }

        float L = length(prevPos - pos);
        float l = length(y);
        pos = pos - alongNormal * (L - l);

        normal = mat3(gs_in[0].view) * normalize(alongNormal + 0.2 * grass_normal);

        gl_Position = gs_in[0].projection * gs_in[0].view * vec4(pos - tangent * grassParameters.thick/(i + 1), 1.0);
        EmitVertex();

        gl_Position = gs_in[0].projection * gs_in[0].view * vec4(pos + tangent * grassParameters.thick/(i + 1), 1.0);
        EmitVertex();
        prevPos = pos;
    }
    vec3 y = heightStep * alongNormal;
    vec3 x = grass_normal * bendFactor;

    pos = pos + x + y;
    alongNormal = normalize(pos - prevPos);
    grass_normal = normalize(cross(tangent, alongNormal));
    if (grass_normal.z < 0) 
    {
        grass_normal = -grass_normal;
    }

    float L = length(prevPos - pos);
    float l = length(y);
    pos = pos - alongNormal * (L - l);

    normal = mat3(gs_in[0].view) * normalize(alongNormal + 0.2 * grass_normal);

    gl_Position = gs_in[0].projection * gs_in[0].view * vec4(pos, 1.0);
    EmitVertex();

    EndPrimitive();
}

const int N_GRASS_STRAWS = 6;

void main()
{
    for (int j = 0; j < N_GRASS_STRAWS; j++)
    {
        generateStraw(j);
    }
    /*for (int j = 0; j < N_GRASS_STRAWS; j++)
    {
        int index = int(16.0 * random(floor(gs_in[0].fragPosition * 1000.0), j)) % 16;
        //vec3 tangent = 0.3 * (nx1x0 * poissonDisk[index].x + nx2x0 * poissonDisk[index].y);
        vec3 c1 = cross(alongNormal, vec3(0.0, 0.0, 1.0));
        vec3 c2 = cross(alongNormal, vec3(0.0, 1.0, 0.0));

        vec3 tangent = length(c1) > length(c2) ? normalize(c1) : normalize(c2);;

        vec3 grass_normal = normalize(cross(tangent, alongNormal));
        if (grass_normal.z < 0) {
            grass_normal = -grass_normal;
        }

        for (int i = 0; i < 9; ++i)
        {
            vec3 pos = gl_in[0].gl_Position.xyz;
            vec3 y = grass_xy[i].y * grassParameters.height * alongNormal;
            
            normal = normalize(alongNormal + 0.2 * grass_normal);
            vec3 x = grass_xy[i].x * grassParameters.thick * tangent + grass_normal * weight[i] * grassParameters.height/4;
            //pos = pos + x2x0 * COORDS_V[j] + x1x0 * COORDS_U[j] + x + y;
            pos = pos + x + y;
            gl_Position = gs_in[0].projection * vec4(pos, 1.0);
            EmitVertex();
        }
        EndPrimitive();
    }*/
}