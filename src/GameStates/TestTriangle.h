#pragma once
#include "GameState.h"
#include <glm/gtc/constants.hpp>
#include "../utils/Timer.h"
#include "../scene/Lights/DirLight.h"
#include "../scene/Lights/PointLight.h"
#include "../scene/Models/Box.h"
#include "../controllers/CameraController.h"

class TestTriangle : public GameState
{
private:

  Scene* clonedScene = nullptr;

  // clear color
  glm::vec4 _mClearColor;
  Scene* clone;

  // current scene stuffs - all Temporary!! 
  FreeCamera* _mCamera;
  Model* model;
  PhongMaterial* mat;
  Timer timer;

  FirstPersonFreeCameraController* cameraController;

  // point light
  std::vector<PointLight*> pointLights;
  std::vector<DirLight* > dirLights;
  Box* lightBox;
  float lightAngle = 0;
  float lightRotateSpeed = glm::half_pi<float>() / 2.f;
  float distFromCenter = 4.f;

  // rotation
  float currentAngle = 0.f;
  float rotateSpeed = 1.f;

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