#pragma once
#include "Node.h"
#include "../components/ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class CameraBase : public virtual Node
{
protected:
  virtual void _updateViewMatrix() = 0;
  virtual void _updateProjectionMatrix() = 0;
  virtual void copyTo(Cloneable* cloned) const override;

public:
  virtual const glm::mat4& getViewMatrix() const = 0;
  virtual const glm::mat4& getProjectionMatrix() const = 0;
  virtual const glm::mat4& forceComputeViewMatrix() = 0;
  virtual const glm::mat4& forceComputeProjectionMatrix() = 0;
  virtual void update(float deltaT) = 0;
  virtual void setProgramUniform(ShaderProgram& shaderProgram) = 0;
  virtual CameraBase* clone() const override = 0;
};

class PerspectiveCamera : public CameraBase
{
protected:
  // some initial settings - should be changed later
  float _mMinZ = 0.1f;
  float _mMaxZ = 100.f;
  float _mFovy = 45.f;
  float _mAspectRatio = 16.f / 9.f;

  bool _mShouldUpdateView;
  glm::mat4 _mViewMatrixCache;

  bool _mShouldUpdateProjection;
  glm::mat4 _mProjectMatrixCache;

protected:
  virtual void _updateProjectionMatrix() override;
  virtual void copyTo(Cloneable* cloned) const override;

public:
  PerspectiveCamera();
  virtual ~PerspectiveCamera();

  // Returns the cached camera matrix
  const glm::mat4& getViewMatrix() const override;

  // Force a re-computation of the camera matrix
  const glm::mat4& forceComputeViewMatrix() override;

  // Returns the cached 
  const glm::mat4& getProjectionMatrix() const override;

  // Force a re-computation of the projection matrix
  const glm::mat4& forceComputeProjectionMatrix() override;

  // update the camera matrices
  virtual void update(float deltaT) override;

  // getters
  float getMinZ() const { return _mMinZ; }
  float getMaxZ() const { return _mMaxZ; }
  float getFovy() const { return _mFovy; }

  // setters
  void setMinZ(float minZ);
  void setMaxZ(float maxZ);
  void setFovy(float fovy);
  void setAspectRatio(float aspectRatio);

  virtual void setProgramUniform(ShaderProgram& shaderProgram) override;
  virtual PerspectiveCamera* clone() const override = 0;
};

// always look at a particular point in world space
class TargetCamera : public PerspectiveCamera
{
protected:
  glm::vec3 _mPosition;
  glm::vec3 _mTarget;
  glm::vec3 _mUp;

  virtual void _updateViewMatrix() override;
  virtual void copyTo(Cloneable* cloned) const override;

public:

  TargetCamera();
  virtual ~TargetCamera();

  void setPosition(const glm::vec3& pos);
  void setTarget(const glm::vec3& target);
  void setUp(const glm::vec3& up);

  const glm::vec3& getPosition() const;
  const glm::vec3& getTarget() const;
  const glm::vec3& getUp() const;

  virtual void setProgramUniform(ShaderProgram& shaderProgram) override;
  virtual TargetCamera* clone() const override;
};

// always looks at a certain direction
class ForwardCamera : public PerspectiveCamera
{
protected:
  glm::vec3 _mPosition;
  glm::vec3 _mForwardDirection;
  glm::vec3 _mUp;

  virtual void _updateViewMatrix() override;
  virtual void copyTo(Cloneable* cloned) const override;

public:

  ForwardCamera();
  virtual ~ForwardCamera();

  void setPosition(const glm::vec3& pos);
  void setForwardDirection(const glm::vec3& direction);
  void setUp(const glm::vec3& up);

  const glm::vec3& getPosition() const;
  const glm::vec3& getForwardDirection() const;
  const glm::vec3& getUp() const;

  virtual void setProgramUniform(ShaderProgram& shaderProgram) override;
  virtual ForwardCamera* clone() const override;
};

// has both target and forward camera, on a switch
class FreeCamera : public TargetCamera
{
protected:

  // always a normalized vector
  glm::vec3 _mForwardDirection;
  bool _mUseTarget;

  virtual void _updateViewMatrix() override;
  virtual void copyTo(Cloneable* cloned) const override;

public:

  FreeCamera();
  virtual ~FreeCamera();

  void useTarget(bool shouldUseTarget);
  void setForwardDirection(const glm::vec3& forwardDirection);

  bool isUsingTarget() const;
  const glm::vec3& getForwardDirection() const;
  virtual FreeCamera* clone() const override;
};