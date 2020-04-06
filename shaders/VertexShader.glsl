#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec4 aTangent;
layout (location = 4) in uvec4 aJoint;
layout (location = 5) in vec4 aWeight;
layout (location = 6) in vec2 aTex_2;
layout (location = 7) in vec2 aTex_3;

/* pos and normal in world coordinates */
out vec3 fPos;
out vec3 fNormal;
out vec2 fTex;
out vec4 fTangent;
out vec2 fTex_2;
out vec2 fTex_3;

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
    fNormal = normalize(normalMat * aNormal);
    fTex = aTex;
    fTex_2 = aTex_2;
    fTex_3 = aTex_3;
}