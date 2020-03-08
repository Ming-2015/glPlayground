#include "Camera.h"

// Camera
Camera::Camera()
  : _mShouldUpdateView(false), 
  _mViewMatrixCache(),
  _mShouldUpdateProjection(false),
  _mProjectMatrixCache( glm::perspective(_mFovy, _mAspectRatio, _mMinZ, _mMaxZ) )
{}

Camera::~Camera()
{}

void Camera::_updateProjectionMatrix()
{
  if (_mOrthogonal)
  {
    // TODO: figure out orthogonal matrix
    //_mProjectMatrixCache = glm::ortho()
  }
  else
  {
    _mProjectMatrixCache = glm::perspective(_mFovy, _mAspectRatio, _mMinZ, _mMaxZ);
  }
}

// Returns the cached camera matrix
const glm::mat4& Camera::getViewMatrix() const
{
  return _mViewMatrixCache;
}

// Force a re-computation of the camera matrix and get the view matrix
const glm::mat4& Camera::forceComputeViewMatrix()
{
  if (_mShouldUpdateView)
  {
    _updateViewMatrix();
  }

  return _mViewMatrixCache;
}


// Returns the cached 
const glm::mat4& Camera::getProjectionMatrix() const
{
  return _mProjectMatrixCache;
}

// Force a re-computation of the projection matrix
const glm::mat4& Camera::forceComputeProjectionMatrix()
{
  if (_mShouldUpdateProjection)
  {
    _updateProjectionMatrix();
  }

  return _mProjectMatrixCache;
}

// update the camera
void Camera::update(float deltaT)
{
  if (_mShouldUpdateView)
  {
    _updateViewMatrix();
  }

  if (_mShouldUpdateProjection)
  {
    _updateProjectionMatrix();
  }
}

void Camera::setMinZ(float minZ)
{
  if (_mMinZ != minZ)
  {
    _mMinZ = minZ;
    _mShouldUpdateProjection = true;
  }
}

void Camera::setMaxZ(float maxZ)
{
  if (_mMaxZ != maxZ)
  {
    _mMaxZ = maxZ;
    _mShouldUpdateProjection = true;
  }
}

void Camera::setFovy(float fovy)
{
  if (_mFovy != fovy)
  {
    _mFovy = fovy;
    _mShouldUpdateProjection = true;
  }
}

void Camera::setAspectRatio(float aspectRatio)
{
  if (_mAspectRatio != aspectRatio)
  {
    _mAspectRatio = aspectRatio;
    _mShouldUpdateProjection = true;
  }
}

void Camera::usePerspective()
{
  if (_mOrthogonal) 
  {
    _mOrthogonal = false;
    _mShouldUpdateProjection = true;
  }
}

void Camera::useOrthogonal()
{
  // orthogonal is disabled for now...
}

// ------------------------ target camera ------------------------
TargetCamera::TargetCamera()
  : _mPosition(0,0,1), _mTarget(0,0,0), _mUp(0,1,0)
{
  _updateViewMatrix();
}

TargetCamera::~TargetCamera()
{}

void TargetCamera::_updateViewMatrix()
{
  _mViewMatrixCache = glm::lookAt(_mPosition, _mTarget, _mUp);
}

void TargetCamera::setPosition(const glm::vec3& pos)
{
  if (pos != _mPosition)
  {
    _mPosition = pos;
    _mShouldUpdateView = true;
  }
}

void TargetCamera::setTarget(const glm::vec3& target)
{
  if (target != _mTarget)
  {
    _mTarget = target;
    _mShouldUpdateView = true;
  }
}

void TargetCamera::setUp(const glm::vec3& up)
{
  if (up != _mUp)
  {
    _mUp = up;
    _mShouldUpdateView = true;
  }
}

const glm::vec3& TargetCamera::getPosition() const
{
  return _mPosition;
}

const glm::vec3& TargetCamera::getTarget() const
{
  return _mTarget;
}

const glm::vec3& TargetCamera::getUp() const
{
  return _mUp;
}