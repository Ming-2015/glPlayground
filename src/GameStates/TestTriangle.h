#pragma once
#include "GameState.h"
#include "../utils/Timer.h"
#include <glm/gtc/constants.hpp>

class TestTriangle : public GameState
{
private:

  // clear color
  glm::vec4 _mClearColor;

  // current scene stuffs - all Temporary!! 
  FreeCamera* _mCamera;
  Model* triModel;
  PhoonMaterial* mat;
  Timer timer;

  // key movement
  float _mMoveSpeed = 4.f; // speed = unit per second
  bool goUp = false;
  bool goDown = false;
  bool goLeft = false;
  bool goRight = false;

  // cursor movement
  bool firstTime = true;
  float _mVerticalSpeed = .06f; // degrees per pixel moved
  float _mHorizontalSpeed = .06f;
  float yaw = -90, pitch = 0;
  double cursorX = 0;
  double cursorY = 0;
  double cursorMoveX = 0;
  double cursorMoveY = 0;

  // triangle rotate speed - radians per second
  float currentAngle = 0;
  float rotateSpeed = glm::half_pi<float>();

  virtual void _onUpdate(float deltaT);
  virtual void _onDraw();

public:

  TestTriangle(const GameResources& resources);
  virtual ~TestTriangle();

  // resouce allocation/deallocation
  virtual void _onLoad();
  virtual void _onDestroy();

  // returns a non-empty string if the next state should be rendered
  virtual const std::string& nextState() const;

  virtual void onKey(int key, int scancode, int action, int mods);
  virtual void onCursorPos(double xPos, double yPos);
  virtual void onMouseButton(int key, int action, int mods);
  virtual void onResize(int width, int height);
};