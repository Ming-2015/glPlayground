#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

/* pos and normal in world coordinates */
out vec3 fPos;
out vec3 fNormal;
out vec2 fTex;

/* matrix inputs */
// model matrix
uniform mat4 model;
// projectionMat * viewMat * modelMat
uniform mat4 projViewModel;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fPos = aPos;
    fNormal = aNormal;
    fTex = aTex;
}