/*
 * Window.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include "Window.h"
#include <stdexcept>

void Window::onKeyCb(GLFWwindow* w, int key, int scancode, int action, int mods)
{
  static_cast<Window*>(glfwGetWindowUserPointer(w))->onKey(key, scancode, action, mods);
}

void Window::onCursorPosCb(GLFWwindow* w, double xPos, double yPos)
{
  static_cast<Window*>(glfwGetWindowUserPointer(w))->onCursorPos(xPos, yPos);
}

void Window::onMouseButtonCb(GLFWwindow* w, int key, int action, int mods)
{
  static_cast<Window*>(glfwGetWindowUserPointer(w))->onMouseButton(key, action, mods);
}

void Window::onResizeCb(GLFWwindow* w, int width, int height)
{
  static_cast<Window*>(glfwGetWindowUserPointer(w))->onResize(width, height);
}

Window::Window(int width, int height, std::string title):
  _mDefaultWidth(width), _mDefaultHeight(height), _mTitle(title), _mWindow(NULL)
{}

Window::~Window()
{
  destroy();
}

void Window::initialize(bool resizable)
{
  static bool firstTime = true;
  bool initGlfwAndGlad = firstTime;

  glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  _mWindow = glfwCreateWindow(_mDefaultWidth, _mDefaultHeight, _mTitle.c_str(), NULL, NULL);
  if (_mWindow == NULL)
  {
    throw std::runtime_error("Failed to create GLFW window");
  }
  glfwSetWindowUserPointer(_mWindow, this);

  glfwSetKeyCallback(_mWindow, onKeyCb);
  glfwSetCursorPosCallback(_mWindow, onCursorPosCb);
  glfwSetMouseButtonCallback(_mWindow, onMouseButtonCb);
  glfwSetWindowSizeCallback(_mWindow, onResizeCb);

  if (initGlfwAndGlad)
  {
    firstTime = false;
    setContextCurrent();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      glfwTerminate();
      throw std::runtime_error("Failed to initialize GLAD");
    }
  }
}

void Window::addObservable(WindowObservable* observable)
{
  observables.insert(observable);
}

void Window::removeObservable(WindowObservable* observable)
{
  observables.erase(observable);
}

void Window::onKey(int key, int scancode, int action, int mods)
{
  for (auto it : observables)
  {
    it->onKey(key, scancode, action, mods);
  }
}

void Window::onCursorPos(double xPos, double yPos)
{
  for (auto it : observables)
  {
    it->onCursorPos(xPos, yPos);
  }
}

void Window::onMouseButton(int key, int action, int mods)
{
  for (auto it : observables)
  {
    it->onMouseButton(key, action, mods);
  }
}

void Window::onResize(int width, int height)
{
  for (auto it : observables)
  {
    it->onResize(width, height);
  }
}

void Window::destroy()
{
  if (_mWindow != NULL)
  {
    glfwDestroyWindow(_mWindow);
    _mWindow = NULL;
  }
}

void Window::nextFrame()
{
  glfwSwapBuffers(_mWindow);
  glfwPollEvents();
}

bool Window::shouldClose() const
{
  return glfwWindowShouldClose(_mWindow);
}

void Window::close()
{
  glfwSetWindowShouldClose(_mWindow, GLFW_TRUE);
}

void Window::setContextCurrent()
{
  glfwMakeContextCurrent(_mWindow);
}

void Window::setCursorMode(int mode)
{
  glfwSetInputMode(_mWindow, GLFW_CURSOR, mode);
}

bool Window::isContextCurrent() const
{
  return glfwGetCurrentContext() == _mWindow;
}

glm::dvec2 Window::getCursorPosition()
{
  glm::dvec2 ret;
  glfwGetCursorPos(_mWindow, &ret.x, &ret.y);
  return ret;
}