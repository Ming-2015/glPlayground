#include "GameObject.h"

GameObject::GameObject()
  : _mModelMatrix( glm::mat4(1.0f) ),
  _mPositionMatrix( glm::mat4(1.0f) ),
  _mRotationMatrix( glm::mat4(1.0f) ),
  _mScaleMatrix( glm::mat4(1.0f) ),
  _mPosition( glm::vec3(0) ),
  _mRotationQuaternion( glm::quat(0,0,0,1) ),
  _mScale( glm::vec3(1.f) ),
  _mShouldUpdateModelMatrix( false )
{}

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

void GameObject::draw(const glm::mat4& PV, const glm::mat4& M) const
{
  Node::draw(PV, M * _mModelMatrix);
}