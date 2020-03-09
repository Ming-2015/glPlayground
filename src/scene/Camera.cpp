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

// ----------------- forward camera ----------------------------------
ForwardCamera::ForwardCamera()
  : _mPosition(0, 0, 1), _mForwardDirection(0, 0, -1), _mUp(0, 1, 0)
{
  _updateViewMatrix();
}

ForwardCamera::~ForwardCamera()
{}

void ForwardCamera::_updateViewMatrix()
{
  _mViewMatrixCache = glm::lookAt(_mPosition, _mPosition + _mForwardDirection, _mUp);
}

void ForwardCamera::setPosition(const glm::vec3& pos)
{
  if (pos != _mPosition)
  {
    _mPosition = pos;
    _mShouldUpdateView = true;
  }
}

void ForwardCamera::setForwardDirection(const glm::vec3& forward)
{
  if (forward != _mForwardDirection)
  {
    _mForwardDirection = forward;
    _mShouldUpdateView = true;
  }
}

void ForwardCamera::setUp(const glm::vec3& up)
{
  if (up != _mUp)
  {
    _mUp = up;
    _mShouldUpdateView = true;
  }
}

const glm::vec3& ForwardCamera::getPosition() const
{
  return _mPosition;
}

const glm::vec3& ForwardCamera::getForwardDirection() const
{
  return _mForwardDirection;
}

const glm::vec3& ForwardCamera::getUp() const
{
  return _mUp;
}

// --------------- free camera --------------------------
FreeCamera::FreeCamera()
  : TargetCamera(), _mForwardDirection(0,0,-1), _mUseTarget(false)
{}

FreeCamera::~FreeCamera()
{}

void FreeCamera::_updateViewMatrix()
{
  if (_mUseTarget)
  {
    TargetCamera::_updateViewMatrix();
  }
  else
  {
    _mViewMatrixCache = glm::lookAt(_mPosition, _mPosition + _mForwardDirection, _mUp);
  }
}

void FreeCamera::useTarget(bool shouldUseTarget)
{
  if (_mUseTarget == shouldUseTarget)
    return;
  
  _mUseTarget = shouldUseTarget;
  _mShouldUpdateView = true;
}

// NOTE: this won't have effects in target mode
void FreeCamera::setForwardDirection(const glm::vec3& forwardDirection)
{
  glm::vec3 normalized(glm::normalize(forwardDirection));
  if (_mForwardDirection != normalized)
  {
    _mForwardDirection = normalized;
    if (!_mUseTarget) _mShouldUpdateView = true;
  }
}

bool FreeCamera::isUsingTarget() const
{
  return _mUseTarget;
}

// returns the normalized forward direction; works for both target and forward mode
const glm::vec3& FreeCamera::getForwardDirection() const
{
  if (!_mUseTarget)
  {
    return _mForwardDirection;
  }
  else 
  {
    return glm::normalize(_mTarget - _mPosition);
  }
}