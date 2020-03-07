#version 460 core
out vec4 FragColor;

/* inputs */
in vec3 fPos;
in vec3 fNormal;
in vec2 fTex;

/* textures */
uniform sampler2D diffuseTex;

void main()
{
  FragColor = texture(diffuseTex, fTex);
}