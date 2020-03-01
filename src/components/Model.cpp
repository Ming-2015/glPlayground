#include "Model.h"

Model::Model(const Mesh& mesh)
  : _mMesh(mesh), 
  _mShouldUpdateModelMatrix(false),
  _mModelMatrix(), 
  _mPositionMatrix(),
  _mScaleMatrix(),
  _mRotationMatrix(),
  _mPosition(0), 
  _mScale(1),
  _mRotationQuaternion(0, 0, 0, 1)
{}

Model::~Model()
{}

void Model::_updateWorldMatrix()
{
  _mModelMatrix = _mPositionMatrix * _mRotationMatrix * _mScaleMatrix;
}

void Model::_updatePositionMatrix()
{
  _mPositionMatrix = glm::translate(_mPosition);
}
void Model::_updateScaleMatrix()
{
  _mScaleMatrix = glm::scale(_mScale);
}

void Model::_updateRotationMatrix()
{
  _mRotationMatrix = glm::toMat4(_mRotationQuaternion);
}

void Model::update(float deltaT)
{
  if (_mShouldUpdateModelMatrix)
  {
    _updateWorldMatrix();
    _mShouldUpdateModelMatrix = false;
  }
}

void Model::draw(const glm::mat4& C) const
{
  // TODO: do something with the matrices
  _mMesh.render();
}

void Model::setPosition(const glm::vec3& pos)
{
  _mPosition = pos;
  _updatePositionMatrix();
  _mShouldUpdateModelMatrix = true;
}

void Model::setScale(const glm::vec3& scale)
{
  _mScale = scale;
  _updateScaleMatrix();
  _mShouldUpdateModelMatrix = true;
}

void Model::setRotationQuaternion(const glm::quat& rotationQuaternion)
{
  _mRotationQuaternion = rotationQuaternion;
  _updateRotationMatrix();
  _mShouldUpdateModelMatrix = true;
}

const glm::vec3& Model::getPosition() const
{
  return _mPosition;
}

const glm::vec3& Model::getScale() const
{
  return _mScale;
}

const glm::quat& Model::getRotationQuaternion() const
{
  return _mRotationQuaternion;
}