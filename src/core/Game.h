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
#include "../components/ShaderProgramManager.h"
#include "../components/ShaderManager.h"

#include "../scene/Model.h"
#include "../components/Material.h"
#include "../scene/Scene.h"

class Game {
private:
  // clear color
  glm::vec4 _mClearColor;

  ShaderManager _mShaderManager;
  ShaderProgramManager _mProgramManager;
  TextureManager _mTextureManager;

  Scene* _mCurrentScene;

public:
  Game();
  virtual ~Game();

  void init();
  void update(float deltaT);
  void render() const;
};

#endif /* SRC_CORE_GAME_H_ */