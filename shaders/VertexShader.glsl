#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

/* pos and normal in world coordinates */
out vec3 fPos;
out vec3 fNormal;
out vec2 fTex;

// model matrix
uniform mat4 modelMat;

// normal matrix
uniform mat3 normalMat;

// projectionMat * viewMat * modelMat
uniform mat4 projViewModelMat;

void main()
{
    gl_Position = projViewModelMat * vec4(aPos, 1.0);
    fPos = vec3(modelMat * vec4(aPos, 1.0));
    fNormal = normalMat * aNormal;
    fTex = aTex;
}