#version 460 core

/* final output */
out vec4 FragColor;

/* inputs, world coordinates */
in vec3 fPos;
in vec3 fNormal;
in vec2 fTex;
in vec4 fTangent;

/* camera */
struct Camera {
  float minZ;
  float maxZ;
  vec3 position;
};
uniform Camera camera;

/* materials */
struct PhoonMaterial {
  /* colors */
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  /* textures */
  sampler2D ambientTex;
  sampler2D diffuseTex;
  sampler2D specularTex;
  
  /* others */
  int shininess;
};
uniform PhoonMaterial phoonMaterial;

/* lights */
#define NR_POINT_LIGHTS 4
struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];


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

  for (int i = 0; i < NR_POINT_LIGHTS; i++) 
  {
    vec3 surfaceToLight = pointLights[i].position - fPos;
    vec3 lightDir = normalize(surfaceToLight);
    vec3 reflectDir = reflect(-lightDir, fNormal);
    float distFromLight = length(surfaceToLight);
    float nDotL = max(dot(fNormal, lightDir), 0.0);
    float vDotR = max(dot(viewDir, reflectDir), 0.0);
    float spec = pow(vDotR, phoonMaterial.shininess);

    ambientComponent += pointLights[i].ambient;
    diffuseComponent += nDotL * pointLights[i].diffuse;
    specularComponent += pointLights[i].specular * spec;
  }

  vec3 matAmbient = phoonMaterial.ambient.xyz + texture(phoonMaterial.ambientTex, ambientTexCoord).xyz;
  vec3 matDiffuse = phoonMaterial.diffuse.xyz + texture(phoonMaterial.diffuseTex, diffuseTexCoord).xyz;
  vec3 matSpecular = phoonMaterial.specular.xyz + texture(phoonMaterial.specularTex, specularTexCoord).xyz;

  ambientComponent = ambientComponent * matAmbient;
  diffuseComponent = diffuseComponent * matDiffuse;
  specularComponent = specularComponent * matSpecular;

  FragColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0f);
}