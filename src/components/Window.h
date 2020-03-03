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

class Window {
private:
  int _mInitWidth, _mInitHeight;
  std::string _mTitle;
  GLFWwindow* _mWindow;

public:
  Window(int width, int height, std::string title);
  ~Window();

  bool shouldClose() const;
  bool isContextCurrent() const;
  const GLFWwindow* getGLFWWindow() const;

  void setContextCurrent();
  void setResizeCallback(GLFWframebuffersizefun callback);
  void setKeyCallback(GLFWkeyfun callback);

  void initialize();
  void destroy();
  void update();
  void close();

  int getInitWidth() { return _mInitWidth; }
  int getInitHeight() { return _mInitHeight; }
};

#endif /* SRC_WINDOW_H_ */
