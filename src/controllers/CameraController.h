#pragma once
#include "../scene/Camera.h"
#include "../components/Window.h"

class CameraControllerBase {
public:
  virtual void update(float deltaT) = 0;
};

class FreeCameraController : public CameraControllerBase {
protected:
  FreeCamera* _mCamera;
};

class FirstPersonFreeCameraController : public FreeCameraController, public WindowObservable {
protected:
  Window& _mWindow;

  // move direction
  bool _mGoUp = false;
  bool _mGoDown = false;
  bool _mGoLeft = false;
  bool _mGoRight = false;

  // camera rotation
  float _mYaw = -90;
  float _mPitch = 0;

  // cursor
  bool _mFirstTime = false;
  double _mCursorX = 0;
  double _mCursorY = 0;
  double _mCursorMoveX = 0;
  double _mCursorMoveY = 0;

public:

  // move speed
  float moveSpeed = 4.f; // unit per second
  float verticalSpeed = .06f; // radian per pixel
  float horizontalSpeed = .06f; // radian per pixel

  FirstPersonFreeCameraController(Window& window, FreeCamera* camera=nullptr);
  virtual ~FirstPersonFreeCameraController();

  virtual void update(float deltaT);
  virtual void onKey(int key, int scancode, int action, int mods);
  virtual void onCursorPos(double xPos, double yPos);
  virtual void onMouseButton(int key, int action, int mods);
  virtual void onResize(int width, int height);
};