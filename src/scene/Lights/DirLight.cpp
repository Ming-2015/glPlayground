#include "./DirLight.h"

DirLight::DirLight() :
  direction(0, -1, 0),
  diffuse(1.f, 1.f, 1.f),
  ambient(0.f),
  specular(.3f)
{}

DirLight::~DirLight()
{}

void DirLight::setProgramUniform(ShaderProgram& shaderProgram, int index)
{
  std::string uniformName = getUniformName() + "[" + std::to_string(index) + "]";

  std::string diffuseName = uniformName + ".diffuse";
  std::string specularName = uniformName + ".specular";
  std::string ambientName = uniformName + ".ambient";
  std::string directionName = uniformName + ".direction";

  Uniform* diffuseUniform = shaderProgram.getUniformByName(diffuseName);
  Uniform* specularUniform = shaderProgram.getUniformByName(specularName);
  Uniform* ambientUniform = shaderProgram.getUniformByName(ambientName);
  Uniform* directionUniform = shaderProgram.getUniformByName(directionName);

  if (diffuseUniform)
    diffuseUniform->setUniform(diffuse);

  if (specularUniform)
    specularUniform->setUniform(specular);

  if (ambientUniform)
    ambientUniform->setUniform(ambient);

  if (directionUniform)
    directionUniform->setUniform(direction);
}

std::string DirLight::getUniformName() const
{
  return "dirLights";
}