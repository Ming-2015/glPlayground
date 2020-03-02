#version 460 core
out vec4 FragColor;

/* inputs */
in vec3 fPos;
in vec3 fNormal;
in vec2 fTex;

uniform vec3 colors[2];
uniform vec4 rand[3];

void main()
{
  vec3 finalColor = colors[0] + colors[1] + rand[0].xyz;

  FragColor = vec4(finalColor, 1.0f);
}