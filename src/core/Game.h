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
#include "../components/GameObject.h"
#include "../components/ShaderProgram.h"

class Game {
private:
  glm::vec4 clearColor;
  std::vector<GameObject> gameObjects;

  ShaderProgram basicProgram;

public:
  Game();
  virtual ~Game();

  void init();
  void update(float deltaT);
  void render();
};

#endif /* SRC_CORE_GAME_H_ */
