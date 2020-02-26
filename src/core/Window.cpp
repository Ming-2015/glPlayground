/*
 * Window.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include "Window.h"

GLFWwindow* createWindow(int width, int height, std::string title)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (window == NULL)
  {
      throw "Failed to create GLFW window";
  }

  return window;
}

Window::Window(int width, int height, std::string title):
  _mInitWidth(width), _mInitHeight(height), _mTitle(title), _mWindow(NULL)
{}

Window::~Window()
{
  destroy();
}

void Window::initialize()
{
  _mWindow = createWindow(_mInitWidth, _mInitHeight, _mTitle);
}

void Window::destroy()
{
  if (_mWindow != NULL)
  {
    glfwDestroyWindow(_mWindow);
    _mWindow = NULL;
  }
}

void Window::update()
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

void Window::setResizeCallback(GLFWframebuffersizefun callback)
{
  glfwSetFramebufferSizeCallback(_mWindow, callback);
}

void Window::setKeyCallback(GLFWkeyfun callback)
{
  glfwSetKeyCallback(_mWindow, callback);
}

bool Window::isContextCurrent() const
{
  return glfwGetCurrentContext() == _mWindow;
}

const GLFWwindow* Window::getGLFWWindow() const
{
  return _mWindow;
}
