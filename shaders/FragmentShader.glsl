#version 460 core
out vec4 FragColor;

/* inputs */
in vec3 fPos;
in vec3 fNormal;
in vec2 fTex;

void main()
{
    FragColor = vec4(fPos, 1.0f);
} 