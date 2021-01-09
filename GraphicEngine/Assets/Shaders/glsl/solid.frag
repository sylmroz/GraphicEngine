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


layout (location = 0) out vec4 outColor;

vec3 CalcDirectionalLight(DirectionalLightBuffer light)
{
    vec3 lightDir = normalize(vec3(-light.direction));
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * vec3(light.color.diffuse);

    return (diffuse + vec3(light.color.ambient));
}

vec3 CalcPointLight(PointLightBuffer light)
{
    vec3 lightDir = normalize(vec3(light.position) - position);
    float dist = length(position - vec3(light.position));
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));
    float I = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = I * solidColor * vec3(light.color.diffuse);

    return (diffuse + vec3(light.color.ambient)) * attenaution;
}

vec3 CalcSpotLight(SpotLightBuffer light)
{
    vec3 lightDir = normalize(vec3(light.position) - position); 
    float theta = dot(lightDir, normalize(-vec3(light.direction)));
    float epsilon = light.innerCutOff - light.outterCutOff;
    float intesity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);

    float dist = length(position - vec3(light.position));
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));
    float I = max(dot(normal, lightDir), 0.0);

    vec3 diffuse = clamp(I,0 ,1) * solidColor * vec3(light.color.diffuse) * attenaution;

    return (diffuse + vec3(light.color.ambient)) * intesity;
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

        outColor = vec4(lightStrength, 1.0);
    }
}