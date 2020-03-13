#version 460 core
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

void main()
{
  vec2 diffuseTexCoord = fTex;
  if (diffuseTexIndex == 2)
    diffuseTexCoord = fTex_2;
  if (diffuseTexIndex == 3)
    diffuseTexCoord = fTex_3;

  vec2 specularTexCoord = fTex;
  if (specularTexCoord == 2)
    specularTexCoord = fTex_2;
  if (specularTexCoord == 3)
    specularTexCoord = fTex_3;

  FragColor = mix( 
    texture(diffuseTex, diffuseTexCoord),
    texture(specularTex, specularTexCoord),
    0.7
  );
}