#pragma once
#include "Node.h"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

class GameObject : public virtual Node
{
protected:
  // cached matrices for faster updates
  bool _mShouldUpdateModelMatrix;
  glm::mat4 _mModelMatrix;
  glm::mat4 _mPositionMatrix;
  glm::mat4 _mScaleMatrix;
  glm::mat4 _mRotationMatrix;

  // note: using quaternion for rotation
  glm::vec3 _mPosition;
  glm::vec3 _mScale;
  glm::quat _mRotationQuaternion;

  // update world matrix
  void _updateWorldMatrix();
  void _updatePositionMatrix();
  void _updateScaleMatrix();
  void _updateRotationMatrix();

public:

  GameObject();

  virtual void draw(const glm::mat4& PV, const glm::mat4& M) const;
  virtual void update(float deltaT);

  virtual void setPosition(const glm::vec3& pos);
  virtual void setScale(const glm::vec3& scale);
  virtual void setRotationQuaternion(const glm::quat& rotationQuaternion);

  virtual const glm::vec3& getPosition() const;
  virtual const glm::vec3& getScale() const;
  virtual const glm::quat& getRotationQuaternion() const;
};