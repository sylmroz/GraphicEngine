#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (triangles) in;
layout (triangle_strip, max_vertices = 54) out;

layout (location = 0) in VS_OUT
{
    mat4 projection;
    vec3 fragPosition;
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

const vec2 grass_xy[5] = vec2[5](vec2(-0.5, 0.0), vec2(0.5, 0.0), vec2(1.7, 0.7), vec2(2.2, 0.7), vec2(2.0, 1.0));
const float COORDS_U[6] = float[6](0.125000, 0.125000, 0.437500, 0.125000, 0.437500, 0.750000);
const float COORDS_V[6] = float[6](0.750000, 0.437500, 0.437500, 0.125000, 0.125000, 0.125000);
const int N_GRASS_STRAWS = 4;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

// https://github.com/opengl-tutorials/ogl/blob/master/tutorial16_shadowmaps/ShadowMapping.fragmentshader
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}


void main()
{
    vec3 x0 = gl_in[0].gl_Position.xyz;
    vec3 x1 = gl_in[1].gl_Position.xyz;
    vec3 x2 = gl_in[2].gl_Position.xyz;
    
    vec3 x1x0 = (x1 - x0);
    vec3 x2x0 = (x2 - x0);

    vec3 nx1x0 = normalize(x1 - x0);
    vec3 nx2x0 = normalize(x2 - x0);

    vec3 alongNormal = (gs_in[0].normal + gs_in[1].normal + gs_in[2].normal) / 3;
   
    for (int j = 0; j < N_GRASS_STRAWS; j++)
    {
        int index = int(16.0 * random(floor(gs_in[0].fragPosition * 1000.0), j)) % 16;
        vec3 tangent = 0.3 * (nx1x0 * poissonDisk[index].x + nx2x0 * poissonDisk[index].y);

        vec3 grass_normal = normalize(cross(tangent, alongNormal));
        if (grass_normal.z < 0) {
            grass_normal = -grass_normal;
        }

        for (int i = 0; i < 5; ++i)
        {
            vec3 pos = x0;
            vec3 y = grass_xy[i].y * grassParameters.height * alongNormal;
            
            normal = normalize(alongNormal + 0.2 * grass_normal);;
            vec3 x = grass_xy[i].x * grassParameters.thick * tangent;
            pos = pos + x2x0 * COORDS_V[j] + x1x0 * COORDS_U[j] + x + y;
            gl_Position = gs_in[0].projection * vec4(pos, 1.0);
            EmitVertex();
        }
        EndPrimitive();
    }
}