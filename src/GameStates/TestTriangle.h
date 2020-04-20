#pragma once
#include "GameState.h"
#include <glm/gtc/constants.hpp>
#include "../utils/Timer.h"
#include "../scene/Lights/DirLight.h"
#include "../scene/Lights/PointLight.h"
#include "../scene/Models/Box.h"
#include "../controllers/CameraController.h"
#include "../importers/AssetImporter.h"

class TestTriangle : public GameState
{
protected:
  // clear color
  glm::vec4 _mClearColor;

  // current scene stuffs - all Temporary!! 
  FreeCamera* _mCamera;
  Model* model;

  // need to be deleted!
  FirstPersonFreeCameraController* cameraController = nullptr;
  PhongMaterial* mat = nullptr;

  AssetImporter* importer = nullptr;

  // lights
  std::vector<PointLight*> pointLights;
  std::vector<DirLight* > dirLights;
  float lightAngle = 0;
  float lightRotateSpeed = glm::half_pi<float>() / 2.f;
  float distFromCenter = 4.f;

  // rotation
  float currentAngle = 0.f;
  float rotateSpeed = 1.f;

  virtual void _onUpdate(float deltaT) override;
  virtual void _onDraw() override;
  virtual void _onLoad() override;
  virtual void _onDestroy() override;

public:

  TestTriangle(const GameResources& resources);
  virtual ~TestTriangle();

  // returns a non-empty string if the next state should be rendered
  virtual std::string nextState() const override;

  virtual void onKey(int key, int scancode, int action, int mods) override;
  virtual void onCursorPos(double xPos, double yPos) override;
  virtual void onMouseButton(int key, int action, int mods) override;
};