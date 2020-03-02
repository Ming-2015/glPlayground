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
#include "../components/Mesh.h"
#include "../components/Model.h"
#include "../components/ShaderProgramManager.h"
#include "../components/ShaderManager.h"

class Game {
private:
  glm::vec4 clearColor;

  std::map<std::string, Mesh> meshes;
  std::map<std::string, Model> models;

  ShaderManager _mShaderManager;
  ShaderProgramManager _mProgramManager;

  ShaderProgram* _mDefaultProgram;

public:
  Game();
  virtual ~Game();

  void init();
  void update(float deltaT);
  void render();
};

#endif /* SRC_CORE_GAME_H_ */