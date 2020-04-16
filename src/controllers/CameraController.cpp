#include "CameraController.h"


FirstPersonFreeCameraController::FirstPersonFreeCameraController(Window& window, FreeCamera* camera)
  : _mWindow(window)
{
  _mCamera = camera;
  window.addObservable(this);
}

FirstPersonFreeCameraController::~FirstPersonFreeCameraController() {}

void FirstPersonFreeCameraController::update(float deltaT)
{
  if (!_mCamera) return;

  if (_mGoUp)
  {
    glm::vec3 forward = deltaT / 1000.f * moveSpeed * _mCamera->getForwardDirection();
    _mCamera->setPosition(_mCamera->getPosition() + forward);
  }

  if (_mGoDown)
  {
    glm::vec3 backward = deltaT / 1000.f * -moveSpeed * _mCamera->getForwardDirection();
    _mCamera->setPosition(_mCamera->getPosition() + backward);
  }

  if (_mGoLeft)
  {
    glm::vec3 left = deltaT / 1000.f * -moveSpeed * glm::normalize(
      glm::cross(_mCamera->getForwardDirection(), _mCamera->getUp())
    );
    _mCamera->setPosition(_mCamera->getPosition() + left);
  }

  if (_mGoRight)
  {
    glm::vec3 right = deltaT / 1000.f * moveSpeed * glm::normalize(
      glm::cross(_mCamera->getForwardDirection(), _mCamera->getUp())
    );
    _mCamera->setPosition(_mCamera->getPosition() + right);
  }
  _mCamera->update(deltaT);
}

void FirstPersonFreeCameraController::onKey(int key, int scancode, int action, int mods)
{
  bool flag = false;
  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    flag = true;
  }
  else if (action == GLFW_RELEASE)
  {
    flag = false;
  }

  // key events here...
  switch (key)
  {
  case GLFW_KEY_W:
  case GLFW_KEY_UP:
    _mGoUp = flag;
    break;

  case GLFW_KEY_S:
  case GLFW_KEY_DOWN:
    _mGoDown = flag;
    break;

  case GLFW_KEY_A:
  case GLFW_KEY_LEFT:
    _mGoLeft = flag;
    break;

  case GLFW_KEY_D:
  case GLFW_KEY_RIGHT:
    _mGoRight = flag;
    break;
  }
}

void FirstPersonFreeCameraController::onCursorPos(double xPos, double yPos)
{
  if (_mFirstTime)
  {
    _mCursorX = xPos;
    _mCursorY = yPos;
    _mFirstTime = false;
    return;
  }

  _mCursorMoveX = xPos - _mCursorX;
  _mCursorMoveY = _mCursorY - yPos;
  _mCursorX = xPos;
  _mCursorY = yPos;

  if (_mCursorMoveX)
  {
    float factor = _mCursorMoveX * horizontalSpeed;
    _mYaw += factor;
    _mCursorMoveX = 0;
  }

  if (_mCursorMoveY)
  {
    float factor = _mCursorMoveY * verticalSpeed;
    _mPitch += factor;
    if (_mPitch > 89.9f)
    {
      _mPitch = 89.9f;
    }
    if (_mPitch < -89.9f)
    {
      _mPitch = -89.9f;
    }
    _mCursorMoveY = 0;
  }

  if (_mCamera && !_mCamera->isUsingTarget())
  {
    glm::vec3 forward;
    forward.x = cos(glm::radians(_mYaw)) * cos(glm::radians(_mPitch));
    forward.y = sin(glm::radians(_mPitch));
    forward.z = sin(glm::radians(_mYaw)) * cos(glm::radians(_mPitch));
    _mCamera->setForwardDirection(forward);
  }
}

void FirstPersonFreeCameraController::onMouseButton(int key, int action, int mods)
{

}

void FirstPersonFreeCameraController::onResize(int width, int height)
{
  if (_mCamera) {
    if (width > 0 && height > 0)
      Log.print<Severity::debug>("Managed to change aspect ratio, width: ", width, " height: ", height);
      _mCamera->setAspectRatio(float(width) / float(height));
  }
}