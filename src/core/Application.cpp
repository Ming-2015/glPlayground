/*
 * App.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include "Application.h"

// STATIC MEMBERS
Application* Application::App = NULL;
Application& Application::GetOrCreateApp() {
  if (App == NULL)
  {
    App = new Application();
    App->init();
    App->window.setResizeCallback(Application::onResizeCallback);
    App->window.setKeyCallback(Application::onKeyCallback);
  }

  return *App;
}

void Application::Cleanup() {
  if (App != NULL)
  {
    delete App;
    App = NULL;
  }
}

void Application::onResizeCallback(GLFWwindow* window, int width, int height)
{
  Application& app = GetOrCreateApp();
  app.onResize(width, height);
}

void Application::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  Application& app = GetOrCreateApp();
  app.onKey(key, scancode, action, mods);
}

// NON STATIC MEMBERS
Application::Application()
  : window(800, 600, "My Window"),
    game()
{}

Application::~Application()
{}

void Application::onResize(int width, int height)
{
  glViewport(0, 0, width, height);
}

void Application::onKey(int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    window.close();
  }
}

void Application::init()
{
  glfwInit();
  window.initialize();
  window.setContextCurrent();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	  glfwTerminate();
	  throw "Failed to initialize GLAD";
  }

  // NOTE: order is important here. gladLoadGLLoader must run first
  glViewport(0, 0, window.getInitWidth(), window.getInitHeight());

  game.init();
}

int Application::runMainLoop()
{
  auto timeEnd = std::chrono::system_clock::now();
  auto timeStart = timeEnd;
  std::chrono::duration<double, std::milli> timeElapsed;
  float fps = 0;

  while(!window.shouldClose())
  {
    timeStart = timeEnd;
    timeEnd = std::chrono::system_clock::now();
    timeElapsed = timeEnd - timeStart;
    float timeElapsedF = timeElapsed.count();
    if (timeElapsedF > 0)
    {
      fps = 1000.0f / timeElapsedF;
    }

    std::cout << "FPS: " << fps << std::endl;

    game.update(timeElapsedF);
    game.render();

    window.update();
  }

  window.destroy();
  glfwTerminate();
  return 0;
}
