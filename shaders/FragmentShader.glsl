#version 460 core

/* final output */
out vec4 FragColor;

/* inputs */
in vec3 fPos;
in vec3 fNormal;
in vec2 fTex;
in vec4 fTangent;
in vec2 fTex_2;
in vec2 fTex_3;

/* colors */
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 ambient;

/* textures */
uniform uint diffuseTexIndex;
uniform sampler2D diffuseTex;

uniform uint specularTexIndex;
uniform sampler2D specularTex;

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
  vec2 diffuseTexCoord = fTex;
  if (diffuseTexIndex == 2)
    diffuseTexCoord = fTex_2;
  if (diffuseTexIndex == 3)
    diffuseTexCoord = fTex_3;

  vec2 specularTexCoord = fTex;
  if (specularTexIndex == 2)
    specularTexCoord = fTex_2;
  if (specularTexIndex == 3)
    specularTexCoord = fTex_3;

  vec4 totalDiffuseLight = vec4(0, 0, 0, 1.f);
  
  for (int i = 0; i < 4; i++) {
    totalDiffuseLight += vec4(pointLights[i].diffuse, 0);
  }

  FragColor = mix( 
    texture(diffuseTex, diffuseTexCoord) * totalDiffuseLight,
    texture(specularTex, specularTexCoord) * vec4(pointLights[0].specular, 1.f),
    0.7
  );
}