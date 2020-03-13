/*
 * Game.h
 *
 *  Created on: Feb 28, 2020
 *      Author: ycai
 */

#ifndef SRC_CORE_GAME_H_
#define SRC_CORE_GAME_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>

#include "../utils/Timer.h"
#include "../components/ShaderProgram.h"
#include "../components/Shader.h"

#include "../scene/Model.h"
#include "../components/Material.h"
#include "../scene/Scene.h"
#include "Window.h"

class Game : public WindowObservable {
private:

  // window
  Window _mWindow;

  // close game?
  bool done = false;

  // clear color
  glm::vec4 _mClearColor;

  // resources
  ShaderManager _mShaderManager;
  ShaderProgramManager _mProgramManager;
  TextureManager _mTextureManager;
  PrimitiveManager _mPrimitiveManager;

  // current scene stuffs - all Temporary!! 
  Scene* _mCurrentScene;
  FreeCamera* _mCamera;
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

public:
  Game();
  virtual ~Game();

  // normal functions
  void init();
  void update(float deltaT);
  void render();

  // close the game?
  bool shouldClose() const;

  // callbacks to GLFW window events
  virtual void onResize(int width, int height);
  virtual void onKey(int key, int scancode, int action, int mods);
  virtual void onCursorPos(double xPos, double yPos);
  virtual void onMouseButton(int key, int action, int mods);
};

#endif /* SRC_CORE_GAME_H_ */