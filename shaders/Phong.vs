#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in uvec4 aJoint;
layout (location = 6) in vec4 aWeight;
layout (location = 7) in vec2 aTex_2;
layout (location = 8) in vec2 aTex_3;

/* pos and normal in world coordinates */
out vec3 fPos;
out vec3 fNormal;
out vec2 fTex;
out vec4 fTangent;
out vec2 fTex_2;
out vec2 fTex_3;
flat out uvec4 fJoint;

// model matrix
uniform mat4 modelMat;

// normal matrix
uniform mat3 normalMat;

// projectionMat * viewMat * modelMat
uniform mat4 projViewModelMat;

#define MAX_BONE_MATRICES 96
uniform mat4 boneMatrices[MAX_BONE_MATRICES];
uniform int useBoneMatrices;

void main()
{
  mat4 skinMat = mat4(1.f);

  if (useBoneMatrices == 1)
  {
    skinMat = aWeight.x * boneMatrices[aJoint.x]
            + aWeight.y * boneMatrices[aJoint.y]
            + aWeight.w * boneMatrices[aJoint.z]
            + aWeight.z * boneMatrices[aJoint.w];
  }

  gl_Position = projViewModelMat * skinMat * vec4(aPos, 1.0);
  fPos = vec3(modelMat * skinMat * vec4(aPos, 1.0));

  fNormal = normalize(normalMat * mat3(skinMat) * aNormal);

  fTex = aTex;
  fTex_2 = aTex_2;
  fTex_3 = aTex_3;
  fJoint = aJoint;
}