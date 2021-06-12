#version 450 core

#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

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

layout (std140, binding = 1) uniform Eye
{
    vec4 eyePosition;
} eye;

layout (std430, binding = 3) buffer DirectionalLight
{
    uint light_length;
    DirectionalLightBuffer directionalLights[];
} directionalLight;

layout (std430, binding = 4) buffer PointLight
{
    uint light_length;
    PointLightBuffer pointLights[];
} pointLight;

layout (std430, binding = 5) buffer SpotLight
{
    uint light_length;
    SpotLightBuffer spotLights[];
} spotLight;

struct ShadowRenderingOptions
{
    int directional;
    int point;
    int spot;
};

layout (std140, binding = 7) uniform RenderingOptions
{
    ShadowRenderingOptions shadowRendering;
    int ambientOcclusion;
    int globalIllumination;
} renderingOptions;

struct GrassColor
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 translucent;
};

layout (std140) uniform GrassMaterial
{
    GrassColor front;
    GrassColor back;
    float shininess;
} grassMaterial;

uniform sampler2DArray directionalLightShadowMap;
uniform sampler2DArray spotLightShadowMap;
uniform samplerCubeArray pointLightShadowMap;

const float Pi = 3.14159265;


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
    vec4 tmpvar_3;
    tmpvar_3.xyz = seed;
    tmpvar_3.w = i;
    float tmpvar_4;
    tmpvar_4 = (4.375855e+8 * fract(sin(dot (tmpvar_3, vec4(12.9898, 78.233, 45.164, 94.673)))));
    return (((tmpvar_4 + -((10000.0 * floor((tmpvar_4 * 0.0001))))) * 0.0001) + -0.5);
}

float ShadowMapCalculation(sampler2DArray tex, vec4 fragPosLightSpace, vec3 lightDir, int layer)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    if (projCoords.z > 1.0)
        return 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(tex, 0).xy;
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    for (int i = 0; i < 4; ++i)
    {
        int index = int(16.0 * random(floor(position * 1000.0), i)) % 16;
        float depth = texture(tex, vec3(projCoords.xy + poissonDisk[index] * texelSize, layer)).r;
        shadow += (projCoords.z - bias) > depth ? 1.0 : 0.0;
    }
    shadow /= numOfSamples;
        
    return shadow;
}

float PointShadowMapCalculation(samplerCubeArray tex, vec4 fragPosLightSpace, float currentDepth, int layer)
{
    float closestDepth = texture(tex, vec4(fragPosLightSpace.xyz, layer)).r * 25.0;
    float bias = 0.05; 
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

vec4 GrassRendering(vec3 normal, vec3 lightDir, vec3 diffuseLight, vec3 specularLight, vec3 ambientLight, float shadow)
{
    GrassColor grassColor = gl_FrontFacing == true ? grassMaterial.front : grassMaterial.back;
    vec4 translucent = gl_FrontFacing == true ? grassMaterial.back.translucent : grassMaterial.front.translucent;
    float I = dot(normal, lightDir);
    if (I >= 0)
    {
        vec4 diffuse = vec4((1.0 - shadow) * I * grassColor.diffuse.rgb * diffuseLight, grassColor.diffuse.a);

        vec3 viewDir = normalize(eye.eyePosition.xyz - position);
        vec3 halfwayDir = normalize(lightDir + viewDir);
    
        const float energyConservation = ( 8.0 + grassMaterial.shininess ) / ( 8.0 * Pi );
        float spec = energyConservation * pow(max(dot(normal, halfwayDir), 0.0), grassMaterial.shininess);
        vec4 specular = vec4((1.0 - shadow) * spec * grassColor.specular.rgb * specularLight, grassColor.specular.a);

        return (diffuse + specular);// + vec4(grassColor.ambient.rgb * ambientLight, grassColor.ambient.a);
    }
    else
    {
        vec4 translucentSh = vec4((1.0 - shadow) * (-I) * translucent.rgb * diffuseLight, translucent.a);

        return translucentSh;// + vec4(grassColor.ambient.rgb * ambientLight, grassColor.ambient.a);
    }
}

vec4 CalcDirectionalLight(DirectionalLightBuffer light, int layer)
{
    vec3 lightDir = -light.direction.xyz;
    vec4 fragPositionightSpace = light.lightSpace * vec4(position, 1.0);
    float shadow = 0.0;
    if (renderingOptions.shadowRendering.directional > 0)
        shadow = ShadowMapCalculation(directionalLightShadowMap, fragPositionightSpace, lightDir, layer);    
    return GrassRendering(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient), shadow);
}

vec4 CalcPointLight(PointLightBuffer light, int layer)
{
    vec3 lightDir = normalize(light.position.xyz - position);

    float dist = length(position - light.position.xyz);
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));

    float shadow = 0.0;
    if (renderingOptions.shadowRendering.point > 0)
        shadow = PointShadowMapCalculation(pointLightShadowMap, vec4(position - light.position.xyz, 1.0), dist, layer);
    return GrassRendering(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient), shadow) * attenaution;
}

vec4 CalcSpotLight(SpotLightBuffer light, int layer)
{
    vec3 lightDir = normalize(light.position.xyz - position); 

    float theta = dot(lightDir, -light.direction.xyz);
    float epsilon = light.innerCutOff - light.outterCutOff;
    float intesity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);

    float dist = length(position - vec3(light.position));
    float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadric * (dist * dist));

    vec4 fragPositionightSpace = light.lightSpace * vec4(position, 1.0);
    float shadow = 0.0;
    if (renderingOptions.shadowRendering.spot > 0)
        shadow = ShadowMapCalculation(spotLightShadowMap, fragPositionightSpace, lightDir, layer);
    return GrassRendering(normal, lightDir, vec3(light.color.diffuse), vec3(light.color.specular), vec3(light.color.ambient), shadow) * intesity * attenaution;
}

layout (location = 0) out vec4 outColor;

void main()
{
    if (directionalLight.light_length == 0 &&
        pointLight.light_length == 0 &&
        spotLight.light_length == 0)
    {
        vec3 lightDir = normalize(eye.eyePosition.xyz - position);
        outColor = GrassRendering(normal, lightDir, vec3(1.0), vec3(1.0), vec3(0.01), 0.0);
    }

    else
    {
        vec4 lightStrength;
        for (int i = 0; i < directionalLight.light_length; i++ )
        { 
            lightStrength += CalcDirectionalLight(directionalLight.directionalLights[i], i);
        }

        for (int i = 0; i < pointLight.light_length; i++ )
        { 
            lightStrength += CalcPointLight(pointLight.pointLights[i], i);
        }

        for (int i = 0; i < spotLight.light_length; i++ )
        { 
            lightStrength += CalcSpotLight(spotLight.spotLights[i], i);
        }

        outColor = vec4(mix(vec4(0.0), vec4(1.0), lightStrength));
    }
}


/*int numOfSamples = 4;
vec2 poissonDisk[16];
float when_gt(in float x_1)
{
  float tmpvar_2;
  bool tmpvar_3;
  tmpvar_3 = (0.0 < x_1);
  if (tmpvar_3) tmpvar_2 = 1.0;
  if (!(tmpvar_3)) tmpvar_2 = 0.0;
  return tmpvar_2;
}

//;
//;
float random(in vec3 seed_4, in int i_5)
{
  vec4 tmpvar_6;
  tmpvar_6.xyz = seed_4;
  tmpvar_6.w = float(i_5);
  float tmpvar_7;
  tmpvar_7 = fract((sin(dot (tmpvar_6, vec4(12.9898, 78.233, 45.164, 94.673))) * 43758.55));
  return tmpvar_7;
}

float ShadowMapCalculation(in sampler2DArray tex_8, in vec4 fragPosLightSpace_9, in vec3 lightDir_10, in int layer_11)
{
  float tmpvar_12;
  int i_13;
  float bias_14;
  vec2 texelSize_15;
  float shadow_16;
  vec3 projCoords_17;
  projCoords_17 = (((fragPosLightSpace_9.xyz * (1.0/(fragPosLightSpace_9.w))) * 0.5) + 0.5);
  if ((1.0 < projCoords_17.z))
  {
    tmpvar_12 = 0.0;
  }
  else
  {
    shadow_16 = 0.0;
    texelSize_15 = (1.0/(vec2(textureSize (tex_8, 0).xy)));
    bias_14 = max ((0.005 * (1.0 + -(dot (normal, lightDir_10)))), 0.0005);
    i_13 = 0;
    bool tmpvar_18;
    tmpvar_18 = bool(0);
    while (true)
{
      if ((i_13 >= 4))
      {
        tmpvar_18 = bool(1);
      }
      else
      {
        float tmpvar_19;
        tmpvar_19 = random (floor((position * 1000.0)), i_13);
        int tmpvar_20;
        tmpvar_20 = (int(mod (int((16.0 * tmpvar_19)), 16)));
        vec3 tmpvar_21;
        vec2 tmpvar_22;
        if ((tmpvar_20 < 8))
        {
          if ((tmpvar_20 < 4))
          {
            tmpvar_22 = poissonDisk[uint(0)];
            bvec3 tmpvar_23;
            tmpvar_23 = lessThanEqual (ivec3(tmpvar_20), ivec3(1, 2, 3));
            if (tmpvar_23.x) tmpvar_22 = poissonDisk[1u];
            if (tmpvar_23.y) tmpvar_22 = poissonDisk[2u];
            if (tmpvar_23.z) tmpvar_22 = poissonDisk[3u];
          }
          else
          {
            tmpvar_22 = poissonDisk[4u];
            bvec3 tmpvar_24;
            tmpvar_24 = lessThanEqual (ivec3(tmpvar_20), ivec3(5, 6, 7));
            if (tmpvar_24.x) tmpvar_22 = poissonDisk[5u];
            if (tmpvar_24.y) tmpvar_22 = poissonDisk[6u];
            if (tmpvar_24.z) tmpvar_22 = poissonDisk[7u];
          }
}
else
{
          if ((tmpvar_20 < 12))
          {
            tmpvar_22 = poissonDisk[8u];
            bvec3 tmpvar_25;
            tmpvar_25 = lessThanEqual (ivec3(tmpvar_20), ivec3(9, 10, 11));
            if (tmpvar_25.x) tmpvar_22 = poissonDisk[9u];
            if (tmpvar_25.y) tmpvar_22 = poissonDisk[10u];
            if (tmpvar_25.z) tmpvar_22 = poissonDisk[11u];
          }
          else
          {
            tmpvar_22 = poissonDisk[12u];
            bvec3 tmpvar_26;
            tmpvar_26 = lessThanEqual (ivec3(tmpvar_20), ivec3(13, 14, 15));
            if (tmpvar_26.x) tmpvar_22 = poissonDisk[13u];
            if (tmpvar_26.y) tmpvar_22 = poissonDisk[14u];
            if (tmpvar_26.z) tmpvar_22 = poissonDisk[15u];
          }
}
tmpvar_21.xy = (projCoords_17.xy + (tmpvar_22 * texelSize_15));
        tmpvar_21.z = float(layer_11);
        float tmpvar_27;
        bool tmpvar_28;
        tmpvar_28 = (texture (tex_8, vec3(tmpvar_21.xy, layer_11)).r > (projCoords_17.z + -(bias_14)));
        if (tmpvar_28) tmpvar_27 = 1.0;
        if (!(tmpvar_28)) tmpvar_27 = 0.0;
        shadow_16 = (shadow_16 + tmpvar_27);
        i_13++;
      }
if (tmpvar_18)
{
        break;
      }
;
    }
;
    shadow_16 = (shadow_16 * 0.25);
    tmpvar_12 = shadow_16;
  }
return tmpvar_12;
}

float PointShadowMapCalculation(in samplerCubeArray tex_29, in vec4 fragPosLightSpace_30, in float currentDepth_31, in int layer_32)
{
  vec4 tmpvar_33;
  tmpvar_33.xyz = fragPosLightSpace_30.xyz;
  tmpvar_33.w = float(layer_32);
  float tmpvar_34;
  bool tmpvar_35;
  tmpvar_35 = ((texture (tex_29, tmpvar_33.x * 25.0) < (currentDepth_31 + -0.05));
  if (tmpvar_35) tmpvar_34 = 1.0;
  if (!(tmpvar_35)) tmpvar_34 = 0.0;
  return tmpvar_34;
}

vec4 GrassRendering(in vec3 normal_36, in vec3 lightDir_37, in vec3 diffuseLight_38, in vec3 specularLight_39, in vec3 ambientLight_40, in float shadow_41)
{
  vec4 tmpvar_42;
  GrassColor tmpvar_43;
  if (gl_FrontFacing == bool(1))
  {
    tmpvar_43 = grassMaterial.front ;
  }
  else
  {
    tmpvar_43 = grassMaterial.back ;
  }
vec4 tmpvar_44;
  if (gl_FrontFacing == bool(1))
  {
    tmpvar_44 = grassMaterial.back .translucent ;
  }
  else
  {
    tmpvar_44 = grassMaterial.front .translucent ;
  }
float tmpvar_45;
  tmpvar_45 = dot (normal_36, lightDir_37);
  if ((tmpvar_45 >= 0.0))
  {
    vec4 tmpvar_46;
    tmpvar_46.xyz = ((((1.0 + -(shadow_41)) * tmpvar_45) * tmpvar_43.diffuse .xyz) * diffuseLight_38);
    tmpvar_46.w = tmpvar_43.diffuse .w;
    vec3 tmpvar_47;
    tmpvar_47 = (eye.eyePosition .xyz + -(position));
    vec3 tmpvar_48;
    tmpvar_48 = (lightDir_37 + (tmpvar_47 * 1.0/sqrt(abs(dot (tmpvar_47, tmpvar_47)))));
    vec4 tmpvar_49;
    tmpvar_49.xyz = ((((1.0 + -(shadow_41)) * (((8.0 + grassMaterial.shininess ) * 0.03978873) * exp2((grassMaterial.shininess  * log2(
      max (dot (normal_36, (tmpvar_48 * 1.0/sqrt(abs(dot (tmpvar_48, tmpvar_48))))), 0.0)
    ))))) * tmpvar_43.specular .xyz) * specularLight_39);
    tmpvar_49.w = tmpvar_43.specular .w;
    tmpvar_42 = (tmpvar_46 + tmpvar_49);
  }
  else
  {
    vec4 tmpvar_50;
    tmpvar_50.xyz = ((((1.0 + -(shadow_41)) * abs(tmpvar_45)) * tmpvar_44.xyz) * diffuseLight_38);
    tmpvar_50.w = tmpvar_44.w;
    tmpvar_42 = tmpvar_50;
  }
return tmpvar_42;
}

vec4 CalcDirectionalLight(in DirectionalLightBuffer light_51, in int layer_52)
{
  float shadow_53;
  vec3 lightDir_54;
  vec3 tmpvar_55;
  tmpvar_55 = -(light_51.direction ).xyz;
  vec3 tmpvar_56;
  tmpvar_56 = (tmpvar_55 * 1.0/sqrt(abs(dot (tmpvar_55, tmpvar_55))));
  lightDir_54 = tmpvar_56;
  vec4 tmpvar_57;
  tmpvar_57 = ((((light_51.lightSpace [0] * position.x) + (light_51.lightSpace [1] * position.y)) + (light_51.lightSpace [2] * position.z)) + light_51.lightSpace [3]);
  shadow_53 = 0.0;
  if ((0 < renderingOptions.shadowRendering .directional ))
  {
    float tmpvar_58;
    tmpvar_58 = ShadowMapCalculation (directionalLightShadowMap, tmpvar_57, tmpvar_56, layer_52);
    shadow_53 = tmpvar_58;
  }
;
  vec4 tmpvar_59;
  tmpvar_59 = GrassRendering (normal, lightDir_54, light_51.color .diffuse .xyz, light_51.color .specular .xyz, light_51.color .ambient .xyz, shadow_53);
  return tmpvar_59;
}

layout (location = 0) out vec4 outColor;

void main()
{
numOfSamples = 4;
poissonDisk = vec2[16](vec2(-0.9420162, -0.3990622), vec2(0.9455861, -0.7689072), vec2(-0.0941841, -0.9293887), vec2(0.3449594, 0.2938776), vec2(-0.9158858, 0.4577143), vec2(-0.8154423, -0.8791246), vec2(-0.3827754, 0.2767684), vec2(0.974844, 0.7564838), vec2(0.4432333, -0.9751155), vec2(0.5374298, -0.4737342), vec2(-0.2649691, -0.4189302), vec2(0.7919751, 0.1909019), vec2(-0.2418884, 0.9970651), vec2(-0.8140996, 0.9143759), vec2(0.1998413, 0.7864137), vec2(0.1438316, -0.1410079));
  if (directionalLight.light_length == uint(0))
  {
    vec3 tmpvar_60;
    tmpvar_60 = (eye.eyePosition .xyz + -(position));
    vec4 tmpvar_61;
    tmpvar_61 = GrassRendering (normal, (tmpvar_60 * 1.0/sqrt(abs(dot (tmpvar_60, tmpvar_60)))), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(0.01, 0.01, 0.01), 0.0);
    outColor = tmpvar_61;
  }
  else
  {
    int i_62;
    vec4 lightStrength_63;
    i_62 = 0;
    bool tmpvar_64;
    tmpvar_64 = bool(0);
    while (true)
{
      if ((uint(i_62) >= directionalLight.light_length ))
      {
        tmpvar_64 = bool(1);
      }
      else
      {
        vec4 tmpvar_65;
        DirectionalLightBuffer tmpvar_66;
        tmpvar_65 = CalcDirectionalLight (tmpvar_66, i_62);
        lightStrength_63 = (lightStrength_63 + tmpvar_65);
        i_62++;
      }
if (tmpvar_64)
{
        break;
      }
;
    }
;
    outColor = lightStrength_63;
  }
}
*/