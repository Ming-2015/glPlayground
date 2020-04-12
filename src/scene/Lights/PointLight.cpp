#include "PointLight.h"
#include "../../utils/Logger.h"

PointLight::PointLight()
  : GameObject(),
  attenuationType(AttenuationType::linear),
  attenuationVal(1.f),
  diffuse(1.f, 1.f, 1.f),
  ambient(1.f, 1.f, 1.f),
  specular(1.f, 1.f, 1.f)
{
  _mPosition = glm::vec3(0, 10.f, 0);
}

PointLight::~PointLight()
{}

void PointLight::setProgramUniform(ShaderProgram& shaderProgram, int index)
{
  std::string uniformName = getUniformName() + "[" + std::to_string(index) + "]";

  std::string diffuseName = uniformName + ".diffuse";
  std::string specularName = uniformName + ".specular";
  std::string ambientName = uniformName + ".ambient";
  std::string positionName = uniformName + ".position";
  std::string constantName = uniformName + ".constant";
  std::string linearName = uniformName + ".linear";
  std::string quadraticName = uniformName + ".quadratic";

  Uniform* diffuseUniform = shaderProgram.getUniformByName(diffuseName);
  Uniform* specularUniform = shaderProgram.getUniformByName(specularName);
  Uniform* ambientUniform = shaderProgram.getUniformByName(ambientName);
  Uniform* positionUniform = shaderProgram.getUniformByName(positionName);
  Uniform* constantUniform = shaderProgram.getUniformByName(constantName);
  Uniform* linearUniform = shaderProgram.getUniformByName(linearName);
  Uniform* quadraticUniform = shaderProgram.getUniformByName(quadraticName);

  if (diffuseUniform)
    diffuseUniform->setUniform(diffuse);

  if (specularUniform)
    specularUniform->setUniform(specular);
  
  if (ambientUniform) 
    ambientUniform->setUniform(ambient);

  if (positionUniform) 
    positionUniform->setUniform(_mPosition);

  if (constantUniform) 
  {
    if (attenuationType == AttenuationType::constant)
      constantUniform->setUniform(attenuationVal);
    else 
      constantUniform->setUniform(0.f);
  }

  if (linearUniform)
  {
    if (attenuationType == AttenuationType::linear) 
      linearUniform->setUniform(attenuationVal);
    else 
      linearUniform->setUniform(0.f);
  }

  if (quadraticUniform) 
  {
    if (attenuationType == AttenuationType::quadratic)
      quadraticUniform->setUniform(attenuationVal);
    else 
      quadraticUniform->setUniform(0.f);
  }
}

std::string PointLight::getUniformName() const
{
  return "pointLights";
}

void PointLight::copyTo(Cloneable* other) const
{
  GameObject::copyTo(other);
  PointLight* light = dynamic_cast<PointLight*>(other);

  if (!light)
  {
    Log.print<Severity::warning>("Failed to cast point light in clone");
    return;
  }

  light->attenuationType = attenuationType;
  light->attenuationVal = attenuationVal;

  light->diffuse = diffuse;
  light->ambient = ambient;
  light->specular = specular;
}

PointLight* PointLight::clone() const
{
  PointLight* light = new PointLight();
  copyTo(light);
  return light;
}