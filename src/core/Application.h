/*
 * App.h
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <chrono>
#include <iostream>
#include "../utils/Logger.h"
#include "../utils/Timer.h"
#include "Game.h"

class Application {
private:
  Game game;

  Timer fpsPrintTimer;
  Timer updateTimer;

public:
  // private constructor - singleton!
  void init();
  void destroy();
  Application();
  ~Application();

  // main game loop
  int runMainLoop();
};

#endif /* SRC_APP_H_ */
