#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

class GameObjectBase
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

  // for convenience only...
  void _copyTo(GameObjectBase* other) const;

public:
  GameObjectBase();

  void setPosition(const glm::vec3& pos);
  void setScale(const glm::vec3& scale);
  void setRotationQuaternion(const glm::quat& rotationQuaternion);

  const glm::vec3& getPosition() const;
  const glm::vec3& getScale() const;
  const glm::quat& getRotationQuaternion() const;

  const glm::mat4& getTransform() const;
  const bool& isTransformDirty() const;
};