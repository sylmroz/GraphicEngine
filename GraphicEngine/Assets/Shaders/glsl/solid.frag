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


const float Pi = 3.14159265;

const float specularStrength = 0.5;
const float shininess = 32;


float when_gt(float x) {
  if (x > 0)
  {
      return 1.0;
  }

  return 0.0;
}

layout (location = 0) out vec4 outColor;

subroutine vec3 LightShadingEffectType_t(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight);

subroutine(LightShadingEffectType_t)
vec3 DiffuseOnly(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight)
{
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * diffuseLight;

    return (diffuse + ambientLight);
}

subroutine(LightShadingEffectType_t)
vec3 Phong(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight)
{
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * diffuseLight;

    vec3 viewDir = normalize(vec3(eye.eyePosition) - position);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    const float energyConservation = ( 2.0 + shininess ) / ( 2.0 * Pi );
    float spec = energyConservation * pow(max(dot(viewDir, reflectDir), 0.0), shininess) * when_gt(I);
    vec3 specular = specularStrength * spec * specularLight;

    return (diffuse + specular + ambientLight);
}

subroutine(LightShadingEffectType_t)
vec3 BlinnPhong(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight)
{
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * diffuseLight;

    vec3 viewDir = normalize(vec3(eye.eyePosition) - position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    const float energyConservation = ( 8.0 + shininess ) / ( 8.0 * Pi );
    float spec = energyConservation * pow(max(dot(normal, halfwayDir), 0.0), shininess) * when_gt(I);
    vec3 specular = specularStrength * spec * specularLight;

    return (diffuse + specular + ambientLight);
}

subroutine uniform LightShadingEffectType_t LightShadingEffectType;

vec3 CalcDirectionalLight(DirectionalLightBuffer light)
{
    return LightShadingEffectType(normal, normalize(vec3(-light.direction)), vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient));
}

vec3 CalcPointLight(PointLightBuffer light)
{
    vec3 lightDir = normalize(vec3(light.position) - position);

    float dist = length(position - vec3(light.position));
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));

    return LightShadingEffectType(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient)) * attenaution;
}

vec3 CalcSpotLight(SpotLightBuffer light)
{
    vec3 lightDir = normalize(vec3(light.position) - position); 

    float theta = dot(lightDir, normalize(-vec3(light.direction)));
    float epsilon = light.innerCutOff - light.outterCutOff;
    float intesity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);

    float dist = length(position - vec3(light.position));
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));

    return LightShadingEffectType(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient)) * intesity * attenaution;
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
            lightStrength += CalcDirectionalLight(directionalLight.directionalLights[i]);
        }

        for (int i = 0; i < pointLight.light_length; i++ )
        { 
            lightStrength += CalcPointLight(pointLight.pointLights[i]);
        }

        for (int i = 0; i < spotLight.light_length; i++ )
        { 
            lightStrength += CalcSpotLight(spotLight.spotLights[i]);
        }

        outColor = vec4(mix(vec3(0.0), vec3(1.0), lightStrength), 1.0);
    }
}