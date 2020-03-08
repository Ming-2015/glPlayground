#include "GameObject.h"

void GameObject::_updateWorldMatrix()
{
  _mModelMatrix = _mPositionMatrix * _mRotationMatrix * _mScaleMatrix;
}

void GameObject::_updatePositionMatrix()
{
  _mPositionMatrix = glm::translate(_mPosition);
}
void GameObject::_updateScaleMatrix()
{
  _mScaleMatrix = glm::scale(_mScale);
}

void GameObject::_updateRotationMatrix()
{
  _mRotationMatrix = glm::toMat4(_mRotationQuaternion);
}

void GameObject::setPosition(const glm::vec3& pos)
{
  _mPosition = pos;
  _updatePositionMatrix();
  _mShouldUpdateModelMatrix = true;
}

void GameObject::setScale(const glm::vec3& scale)
{
  _mScale = scale;
  _updateScaleMatrix();
  _mShouldUpdateModelMatrix = true;
}

void GameObject::setRotationQuaternion(const glm::quat& rotationQuaternion)
{
  _mRotationQuaternion = rotationQuaternion;
  _updateRotationMatrix();
  _mShouldUpdateModelMatrix = true;
}

const glm::vec3& GameObject::getPosition() const
{
  return _mPosition;
}

const glm::vec3& GameObject::getScale() const
{
  return _mScale;
}

const glm::quat& GameObject::getRotationQuaternion() const
{
  return _mRotationQuaternion;
}

void GameObject::update(float deltaT)
{
  if (_mShouldUpdateModelMatrix)
  {
    _updateWorldMatrix();
    _mShouldUpdateModelMatrix = false;
  }

  Node::update(deltaT);
}