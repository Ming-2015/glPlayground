/*
 * Window.h
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#ifndef SRC_WINDOW_H_
#define SRC_WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <set>
#include <glm/glm.hpp>

class WindowObservable
{
public:
  virtual void onKey(int key, int scancode, int action, int mods) = 0;
  virtual void onCursorPos(double xPos, double yPos) = 0;
  virtual void onMouseButton(int key, int action, int mods) = 0;
  virtual void onResize(int width, int height) = 0;
};

class Window {
private:
  int _mDefaultWidth, _mDefaultHeight;
  std::string _mTitle;
  GLFWwindow* _mWindow;

  // a very crude observable implementation
  std::set<WindowObservable*> observables;

  // observable callbacks
  void onKey(int key, int scancode, int action, int mods);
  void onCursorPos(double xPos, double yPos);
  void onMouseButton(int key, int action, int mods);
  void onResize(int width, int height);

  static void onKeyCb(GLFWwindow* w, int key, int scancode, int action, int mods);
  static void onCursorPosCb(GLFWwindow* w, double xPos, double yPos);
  static void onMouseButtonCb(GLFWwindow* w, int key, int action, int mods);
  static void onResizeCb(GLFWwindow* w, int width, int height);

public:
  Window(int width, int height, std::string title);
  ~Window();

  // window property getters
  bool isContextCurrent() const;
  bool shouldClose() const;
  int getDefaultWidth() { return _mDefaultWidth; }
  int getDefaultHeight() { return _mDefaultHeight; }
  glm::dvec2 getCursorPosition();

  // window property setters
  void setContextCurrent();
  void setCursorMode(int mode);

  // core functions
  void initialize(bool resizable=false);
  void destroy();
  void nextFrame();
  void close();

  // callback observables
  void addObservable(WindowObservable* observable);
  void removeObservable(WindowObservable* observable);

  GLFWwindow* getWindowHandle() const { return _mWindow; }
};

#endif /* SRC_WINDOW_H_ */
