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
#include "Window.h"

class Application {
private:
  Window window;

  static void onResizeCallback(GLFWwindow* window, int width, int height);
  static void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

  // private constructor - singleton!
  Application();
  ~Application();
  static Application* App;

public:

  int run();

  // return the app (single ton, created on demand)
  static Application& GetOrCreateApp();
  static void Cleanup();

  void onResize(int width, int height);
  void onKey(int key, int scancode, int action, int mods);
};

#endif /* SRC_APP_H_ */
