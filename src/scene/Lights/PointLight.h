#pragma once
#include "../Light.h"
#include "../GameObject.h"

class PointLight : public LightBase, public GameObject
{
protected:
  virtual void copyTo(Cloneable* cloned) const override;

public:

  enum class AttenuationType 
  {
    constant,
    linear,
    quadratic,
  };

  AttenuationType attenuationType;
  float attenuationVal;

  glm::vec3 diffuse;
  glm::vec3 ambient;
  glm::vec3 specular;

  PointLight();
  virtual ~PointLight();

  virtual void setProgramUniform(ShaderProgram& shaderProgram, int index) override;
  virtual std::string getUniformName() const override;
  virtual PointLight* clone() const override;
};