#pragma once
#include "Node.h"
#include "../components/Mesh.h"
#include "Material.h"

class Model : Node
{
protected:
  const Mesh& _mMesh;

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
  // material to render the model...
  Material* material = nullptr;

public:

  Model(const Mesh& mesh);
  virtual ~Model();
  virtual void update(float deltaT);
  virtual void draw(const glm::mat4& C) const;

  void setPosition(const glm::vec3& pos);
  void setScale(const glm::vec3& scale);
  void setRotationQuaternion(const glm::quat& rotationQuaternion);

  const glm::vec3& getPosition() const;
  const glm::vec3& getScale() const;
  const glm::quat& getRotationQuaternion() const;
};