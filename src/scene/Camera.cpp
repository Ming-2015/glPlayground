#include "Camera.h"

// Camera
PerspectiveCamera::PerspectiveCamera()
  : _mShouldUpdateView(false), 
  _mViewMatrixCache( glm::mat4(1.0f) ),
  _mShouldUpdateProjection(false),
  _mProjectMatrixCache( glm::perspective(_mFovy, _mAspectRatio, _mMinZ, _mMaxZ) )
{}

PerspectiveCamera::~PerspectiveCamera()
{}

void PerspectiveCamera::_updateProjectionMatrix()
{
  _mProjectMatrixCache = glm::perspective(_mFovy, _mAspectRatio, _mMinZ, _mMaxZ); 
}

// Returns the cached camera matrix
const glm::mat4& PerspectiveCamera::getViewMatrix() const
{
  return _mViewMatrixCache;
}

// Force a re-computation of the camera matrix and get the view matrix
const glm::mat4& PerspectiveCamera::forceComputeViewMatrix()
{
  if (_mShouldUpdateView)
  {
    _updateViewMatrix();
  }

  return _mViewMatrixCache;
}


// Returns the cached 
const glm::mat4& PerspectiveCamera::getProjectionMatrix() const
{
  return _mProjectMatrixCache;
}

// Force a re-computation of the projection matrix
const glm::mat4& PerspectiveCamera::forceComputeProjectionMatrix()
{
  if (_mShouldUpdateProjection)
  {
    _updateProjectionMatrix();
  }

  return _mProjectMatrixCache;
}

// update the camera
void PerspectiveCamera::update(float deltaT)
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

void PerspectiveCamera::setMinZ(float minZ)
{
  if (_mMinZ != minZ)
  {
    _mMinZ = minZ;
    _mShouldUpdateProjection = true;
  }
}

void PerspectiveCamera::setMaxZ(float maxZ)
{
  if (_mMaxZ != maxZ)
  {
    _mMaxZ = maxZ;
    _mShouldUpdateProjection = true;
  }
}

void PerspectiveCamera::setFovy(float fovy)
{
  if (_mFovy != fovy)
  {
    _mFovy = fovy;
    _mShouldUpdateProjection = true;
  }
}

void PerspectiveCamera::setAspectRatio(float aspectRatio)
{
  if (_mAspectRatio != aspectRatio)
  {
    _mAspectRatio = aspectRatio;
    _mShouldUpdateProjection = true;
  }
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