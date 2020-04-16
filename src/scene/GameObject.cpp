#include "GameObject.h"
#include "../utils/Logger.h"

GameObjectBase::GameObjectBase()
  : _mModelMatrix( glm::mat4(1.0f) ),
  _mPositionMatrix( glm::mat4(1.0f) ),
  _mRotationMatrix( glm::mat4(1.0f) ),
  _mScaleMatrix( glm::mat4(1.0f) ),
  _mPosition( glm::vec3(0) ),
  _mRotationQuaternion( glm::quat(0,0,0,1) ),
  _mScale( glm::vec3(1.f) ),
  _mShouldUpdateModelMatrix( false )
{}

void GameObjectBase::_updateWorldMatrix()
{
  _mModelMatrix = _mPositionMatrix * _mRotationMatrix * _mScaleMatrix;
}

void GameObjectBase::_updatePositionMatrix()
{
  _mPositionMatrix = glm::translate(_mPosition);
}
void GameObjectBase::_updateScaleMatrix()
{
  _mScaleMatrix = glm::scale(_mScale);
}

void GameObjectBase::_updateRotationMatrix()
{
  _mRotationMatrix = glm::toMat4(_mRotationQuaternion);
}

void GameObjectBase::setPosition(const glm::vec3& pos)
{
  _mPosition = pos;
  _updatePositionMatrix();
  _mShouldUpdateModelMatrix = true;
}

void GameObjectBase::setScale(const glm::vec3& scale)
{
  _mScale = scale;
  _updateScaleMatrix();
  _mShouldUpdateModelMatrix = true;
}

void GameObjectBase::setRotationQuaternion(const glm::quat& rotationQuaternion)
{
  _mRotationQuaternion = rotationQuaternion;
  _updateRotationMatrix();
  _mShouldUpdateModelMatrix = true;
}

const glm::vec3& GameObjectBase::getPosition() const
{
  return _mPosition;
}

const glm::vec3& GameObjectBase::getScale() const
{
  return _mScale;
}

const glm::quat& GameObjectBase::getRotationQuaternion() const
{
  return _mRotationQuaternion;
}

void GameObjectBase::_copyTo(GameObjectBase* c) const
{
  if (!c)
  {
    Log.print<Severity::warning>("Trying to copy to a null GameObjectBase!");
    return;
  }

  c->_mShouldUpdateModelMatrix = _mShouldUpdateModelMatrix;
  c->_mModelMatrix = _mModelMatrix;
  c->_mPositionMatrix = _mPositionMatrix;
  c->_mScaleMatrix = _mScaleMatrix;
  c->_mRotationMatrix = _mRotationMatrix;
  c->_mPosition = _mPosition;
  c->_mScale = _mScale;
  c->_mRotationQuaternion = _mRotationQuaternion;
}

const glm::mat4& GameObjectBase::getTransform() const
{
  return _mModelMatrix;
}

const bool& GameObjectBase::isTransformDirty() const
{
  return _mShouldUpdateModelMatrix;
}