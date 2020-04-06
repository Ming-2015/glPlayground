#pragma once
#include "../Light.h"
#include "../GameObject.h"

class PointLight : public Light, public GameObject
{
public:

  enum class AttenuationType {
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

  virtual void setProgramUniform(ShaderProgram& shaderProgram, int index);
  virtual std::string getUniformName() const;

  //using GameObject::draw;
  //using GameObject::update;
  //using GameObject::addChild;
  //using GameObject::removeChild;
  //using GameObject::getParent;
  //using GameObject::setParent;
  //using GameObject::getRoot;
  //using GameObject::getChildren;
};