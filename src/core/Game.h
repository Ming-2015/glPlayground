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

class Game {
private:
  glm::vec4 clearColor;

public:
  Game();
  virtual ~Game();

  void init();
  void update(float deltaT);
  void render();
};

#endif /* SRC_CORE_GAME_H_ */
