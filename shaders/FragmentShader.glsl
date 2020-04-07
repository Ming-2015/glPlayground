#version 460 core

/* final output */
out vec4 FragColor;

/* inputs, world coordinates */
in vec3 fPos;
in vec3 fNormal;
in vec2 fTex;
in vec4 fTangent;

/* camera */
struct Camera 
{
  float minZ;
  float maxZ;
  vec3 position;
};
uniform Camera camera;

/* materials */
struct PhongMaterial 
{
  /* colors */
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  /* textures */
  sampler2D ambientTex;
  sampler2D diffuseTex;
  sampler2D specularTex;
  
  /* others */
  int shininess;
};
uniform PhongMaterial phongMaterial;

/* lights */
#define NR_POINT_LIGHTS 4
struct PointLight
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];


#define NR_DIR_LIGHTS 2
struct DirLight 
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform DirLight dirLights[NR_DIR_LIGHTS];


struct LightOutput 
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

LightOutput CalcPointLight(PointLight light, vec3 normal, vec3 fragPos,vec3 viewDir) 
{
  vec3 surfaceToLight = light.position - fragPos;
  vec3 lightDir = normalize(surfaceToLight);
  vec3 reflectDir = reflect(-lightDir, normal);
  float distFromLight = length(surfaceToLight);
  float nDotL = max(dot(normal, lightDir), 0.0);
  float vDotR = max(dot(viewDir, reflectDir), 0.0);
  float spec = pow(vDotR, phongMaterial.shininess);

  float attenuation = light.constant
    + light.linear * distFromLight
    + light.quadratic * distFromLight * distFromLight;

  attenuation = max(attenuation, 0.01f);
  attenuation = 1.f / attenuation;

  LightOutput ret = {
    light.ambient * attenuation, 
    light.diffuse * nDotL * attenuation, 
    light.specular * spec * attenuation
  };

  return ret;
}

LightOutput CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  // surface to source
  vec3 lightDir = normalize(-light.direction);
  float nDotL = max(dot(normal, lightDir), 0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float vDotR = max(dot(viewDir, reflectDir), 0.0);
  float spec = pow(vDotR, phongMaterial.shininess);
  
  LightOutput ret = { light.ambient, light.diffuse * nDotL, light.specular * spec };
  return ret;
}

void main()
{
  vec3 ambientComponent = vec3(0);
  vec3 diffuseComponent = vec3(0);
  vec3 specularComponent = vec3(0);

  vec2 diffuseTexCoord = fTex;
  vec2 specularTexCoord = fTex;
  vec2 ambientTexCoord = fTex;
  
  vec3 surfaceToCamera = camera.position - fPos;
  vec3 viewDir = normalize(surfaceToCamera);
  LightOutput total = { vec3(0), vec3(0), vec3(0) };
  
  vec3 matAmbient = phongMaterial.ambient + texture(phongMaterial.ambientTex, ambientTexCoord).xyz;
  vec3 matDiffuse = phongMaterial.diffuse + texture(phongMaterial.diffuseTex, diffuseTexCoord).xyz;
  vec3 matSpecular = phongMaterial.specular + texture(phongMaterial.specularTex, specularTexCoord).xyz;

  for (int i = 0; i < NR_POINT_LIGHTS; i++)
  {
    LightOutput o = CalcPointLight(pointLights[i], fNormal, fPos, viewDir);
    total.ambient += o.ambient;
    total.diffuse += o.diffuse;
    total.specular += o.specular;
  }

  for (int i = 0; i < NR_DIR_LIGHTS; i++)
  {
    LightOutput o = CalcDirLight(dirLights[i], fNormal, viewDir);
    total.ambient += o.ambient;
    total.diffuse += o.diffuse;
    total.specular += o.specular;
  }

  total.ambient *= matAmbient;
  total.diffuse *= matDiffuse;
  total.specular *= matSpecular;

  FragColor = vec4(total.ambient + total.diffuse + total.specular, 1.0f);
}
