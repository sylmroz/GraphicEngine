#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 solidColor;

struct LightColor
{
    vec4 diffuse;
	vec4 ambient;
	vec4 specular;
};

struct DirectionalLightBuffer
{
    mat4 lightSpace;
	vec4 direction;
	LightColor color;
};

struct PointLightBuffer
{
	vec4 position;
	float constant;
	float linear;
	float quadric;
	LightColor color;
};

struct SpotLightBuffer
{
    mat4 lightSpace;
	vec4 position;
	vec4 direction;
	float innerCutOff;
	float outterCutOff;
	float constant;
	float linear;
	float quadric;
	LightColor color;
};

layout (std430, binding = 7) buffer DirectionalLight
{
    uint light_length;
    DirectionalLightBuffer directionalLights[];
} directionalLight;

layout (std430, binding = 8) buffer PointLight
{
    uint light_length;
    PointLightBuffer pointLights[];
} pointLight;

layout (std430, binding = 9) buffer SpotLight
{
    uint light_length;
    SpotLightBuffer spotLights[];
} spotLight;

layout (std140) uniform Eye
{
    vec4 eyePosition;
} eye;

struct ShadowRenderingOptions
{
    int directional;
    int point;
    int spot;
};

layout (std140) uniform RenderingOptions
{
    ShadowRenderingOptions shadowRendering;
    int ambientOcclusion;
    int globalIllumination;
} renderingOptions;

uniform sampler2DArray shadowMap;
uniform sampler2DArray spotLightShadowMap;

const float Pi = 3.14159265;

const float specularStrength = 0.5;
const float shininess = 32;


float when_gt(float x) 
{
  if (x > 0)
  {
      return 1.0;
  }

  return 0.0;
}

const int numOfSamples = 4;

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

float ShadowMapCalculation(sampler2DArray tex, vec4 fragPosLightSpace, vec3 lightDir, int layer)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if (projCoords.z > 1.0)
        return 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(tex, 0).xy;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    for (int i = 0; i < numOfSamples; ++i)
    {
        int index = int(16.0 * random(floor(position * 1000.0), i)) % 16;
        float depth = texture(tex, vec3(projCoords.xy + poissonDisk[index] * texelSize, layer)).r;
        shadow += projCoords.z > depth ? 1.0 : 0.0;
    }
    shadow /= numOfSamples;
        
    return shadow;
}

layout (location = 0) out vec4 outColor;

subroutine vec3 LightShadingEffectType_t(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight, float shadow);

subroutine(LightShadingEffectType_t)
vec3 DiffuseOnly(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight, float shadow)
{
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * diffuseLight;

    return ((1.0 - shadow) * diffuse + ambientLight);
}

subroutine(LightShadingEffectType_t)
vec3 Phong(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight, float shadow)
{
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * diffuseLight;

    vec3 viewDir = normalize(vec3(eye.eyePosition) - position);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    const float energyConservation = ( 2.0 + shininess ) / ( 2.0 * Pi );
    float spec = energyConservation * pow(max(dot(viewDir, reflectDir), 0.0), shininess) * when_gt(I);
    vec3 specular = specularStrength * spec * specularLight;

    return ((1.0 - shadow) * (diffuse + specular) + ambientLight);
}

subroutine(LightShadingEffectType_t)
vec3 BlinnPhong(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight, float shadow)
{
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * diffuseLight;

    vec3 viewDir = normalize(vec3(eye.eyePosition) - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    const float energyConservation = ( 8.0 + shininess ) / ( 8.0 * Pi );
    float spec = energyConservation * pow(max(dot(normal, halfwayDir), 0.0), shininess) * when_gt(I);
    vec3 specular = specularStrength * spec * specularLight;

    return ((1.0 - shadow) * (diffuse + specular) + ambientLight);
}

subroutine uniform LightShadingEffectType_t LightShadingEffectType;

vec3 CalcDirectionalLight(DirectionalLightBuffer light, int layer)
{
    vec3 lightDir = normalize(vec3(-light.direction));
    vec4 fragPositionightSpace = light.lightSpace * vec4(position, 1.0);
    float shadow = 0.0;
    if (renderingOptions.shadowRendering.directional > 0)
        shadow = ShadowMapCalculation(shadowMap, fragPositionightSpace, lightDir, layer);
    return LightShadingEffectType(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient), shadow);
}

vec3 CalcPointLight(PointLightBuffer light)
{
    vec3 lightDir = normalize(vec3(light.position) - position);

    float dist = length(position - vec3(light.position));
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));

    float shadow = 0.0;
    if (renderingOptions.shadowRendering.point > 0)
        shadow = 0.0; //ShadowMapCalculation(shadowMap, fragPositionightSpace, lightDir, layer);

    return LightShadingEffectType(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient), shadow) * attenaution;
}

vec3 CalcSpotLight(SpotLightBuffer light, int layer)
{
    vec3 lightDir = normalize(vec3(light.position) - position); 

    float theta = dot(lightDir, normalize(-vec3(light.direction)));
    float epsilon = light.innerCutOff - light.outterCutOff;
    float intesity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);

    float dist = length(position - vec3(light.position));
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));

    vec4 fragPositionightSpace = light.lightSpace * vec4(position, 1.0);
    float shadow = 0.0;
    if (renderingOptions.shadowRendering.spot > 0)
        shadow = ShadowMapCalculation(spotLightShadowMap, fragPositionightSpace, lightDir, layer);
    return LightShadingEffectType(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient), shadow) * intesity * attenaution;
}

void main()
{
    if (directionalLight.light_length == 0 &&
        pointLight.light_length == 0 &&
        spotLight.light_length == 0)
    {
        vec3 lightDir = normalize(vec3(eye.eyePosition) - position);
        float I = max(dot(normal, lightDir), 0.0);
        vec3 color = clamp((I + 0.2), 0 , 1) * solidColor;
        outColor = vec4(color, 1.0);
    }

    else
    {
        vec3 lightStrength;
        for (int i = 0; i < directionalLight.light_length; i++ )
        { 
            lightStrength += CalcDirectionalLight(directionalLight.directionalLights[i], i);
        }

        for (int i = 0; i < pointLight.light_length; i++ )
        { 
            lightStrength += CalcPointLight(pointLight.pointLights[i]);
        }

        for (int i = 0; i < spotLight.light_length; i++ )
        { 
            lightStrength += CalcSpotLight(spotLight.spotLights[i], i);
        }

        outColor = vec4(mix(vec3(0.0), vec3(1.0), lightStrength), 1.0);
    }
}