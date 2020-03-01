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
#include "../components/Mesh.h"
#include "../components/ShaderProgram.h"
#include "../components/ShaderManager.h"

class Game {
private:
  glm::vec4 clearColor;
  std::vector<Mesh> gameObjects;

  ShaderManager shaderManager;
  ShaderProgram basicProgram;

public:
  Game();
  virtual ~Game();

  void init();
  void update(float deltaT);
  void render();
};

#endif /* SRC_CORE_GAME_H_ */
