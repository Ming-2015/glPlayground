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

#include "../components/ShaderProgram.h"
#include "../components/Shader.h"
#include "../components/Texture.h"
#include "../components/Primitive.h"
#include "../components/Window.h"

#include "../GameStates/TestTriangle.h"

class Game : public WindowObservable {
private:

  // close game?
  bool done = false;

  // resources
  ShaderManager _mShaderManager;
  ShaderProgramManager _mProgramManager;
  TextureManager _mTextureManager;
  PrimitiveManager _mPrimitiveManager;
  Window _mWindow;

  GameState* _mCurrentState;
  GameResources resources;

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
  void onResize(int width, int height);
  void onKey(int key, int scancode, int action, int mods);
  void onCursorPos(double xPos, double yPos);
  void onMouseButton(int key, int action, int mods);
};

#endif /* SRC_CORE_GAME_H_ */