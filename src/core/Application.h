/*
 * App.h
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include "../utils/Logger.h"
#include "../utils/Timer.h"
#include "../components/Window.h"
#include "Game.h"

class Application {
private:
  // members
  Window window;
  Game game;
  Timer fpsPrintTimer;

  // private constructor - singleton!
  void init();
  Application();
  ~Application();

  // single ton functions
  static Application* App;
  static void onResizeCallback(GLFWwindow* window, int width, int height);
  static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
  int runMainLoop();

  // return the app (single ton, created on demand)
  static Application& GetOrCreateApp();
  static void Cleanup();

  // callbacks to GLFW events
  void onResize(int width, int height);
  void onKey(int key, int scancode, int action, int mods);
};

#endif /* SRC_APP_H_ */
