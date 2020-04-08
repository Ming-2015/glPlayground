#pragma once
#include "../Light.h"
#include "../GameObject.h"

class DirLight : public Light
{
public:

  glm::vec3 direction;
  glm::vec3 diffuse;
  glm::vec3 ambient;
  glm::vec3 specular;

  DirLight();
  virtual ~DirLight();

  virtual void setProgramUniform(ShaderProgram& shaderProgram, int index);
  virtual std::string getUniformName() const;
};