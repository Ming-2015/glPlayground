#version 460 core
out vec4 FragColor;

/* inputs */
in vec3 fPos;
in vec3 fNormal;
in vec2 fTex;

/* textures */
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

void main()
{
  
  FragColor = mix( 
    texture(diffuseTex, fTex),
    texture(specularTex, fTex),
    0.5
  );
}