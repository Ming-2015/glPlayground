#pragma once
#include "Node.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera : public Node
{
protected:

  // some initial settings - should be changed later
  float _mMinZ = 0.1f;
  float _mMaxZ = 100.f;
  float _mFovy = 45.f;
  float _mAspectRatio = 16.f/9.f;
  bool _mOrthogonal = false;

  bool _mShouldUpdateView;
  glm::mat4 _mViewMatrixCache;
  virtual void _updateViewMatrix() = 0;

  bool _mShouldUpdateProjection;
  glm::mat4 _mProjectMatrixCache;
  virtual void _updateProjectionMatrix();

public:
  Camera();
  virtual ~Camera();

  // Returns the cached camera matrix
  const glm::mat4& getViewMatrix() const;

  // Force a re-computation of the camera matrix
  const glm::mat4& forceComputeViewMatrix();

  // Returns the cached 
  const glm::mat4& getProjectionMatrix() const;

  // Force a re-computation of the projection matrix
  const glm::mat4& forceComputeProjectionMatrix();

  // update the camera matrices
  void update(float deltaT);

  // getters
  float getMinZ() const { return _mMinZ; }
  float getMaxZ() const { return _mMaxZ; }
  float getFovy() const { return _mFovy; }
  float getAspectRatio() const { return _mAspectRatio; }
  bool isPerspective() const { return !_mOrthogonal; }
  bool isOrthogonal() const { return _mOrthogonal; }

  // setters
  void setMinZ(float minZ);
  void setMaxZ(float maxZ);
  void setFovy(float fovy);
  void setAspectRatio(float aspectRatio);
  void usePerspective();
  void useOrthogonal();
};

class TargetCamera : public Camera
{
protected:
  glm::vec3 _mPosition;
  glm::vec3 _mTarget;
  glm::vec3 _mUp;

  virtual void _updateViewMatrix();

public:

  TargetCamera();
  virtual ~TargetCamera();

  void setPosition(const glm::vec3& pos);
  void setTarget(const glm::vec3& target);
  void setUp(const glm::vec3& up);

  const glm::vec3& getPosition() const;
  const glm::vec3& getTarget() const;
  const glm::vec3& getUp() const;
};