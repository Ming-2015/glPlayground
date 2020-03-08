/*
 * App.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include "Application.h"
#include <stdexcept>

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
  : window(1600, 900, "My Window"),
    game()
{}

Application::~Application()
{}

void Application::onResize(int width, int height)
{
  game.setWindowSize(width, height);
}

void Application::onKey(int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    window.close();
  }
  game.onKeyEvent(key, scancode, action, mods);
}

void Application::init()
{
  glfwInit();
  window.initialize();
  window.setContextCurrent();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
	  glfwTerminate();
	  throw std::runtime_error("Failed to initialize GLAD");
  }

  // NOTE: order is important here. gladLoadGLLoader must run first
  game.init();
  game.setWindowSize(window.getInitWidth(), window.getInitHeight());
}

int Application::runMainLoop()
{
  float fps = 0;
  fpsPrintTimer.startTimer();
  updateTimer.startTimer();

  while(!window.shouldClose())
  {
    float timeElapsedF = updateTimer.stopTimer();
    updateTimer.startTimer();
    if (timeElapsedF > 0)
    {
      fps = 1000.0f / timeElapsedF;
    }

    // print every 5 seconds
    if (fpsPrintTimer.getTimeElapsed() > 5000)
    {
      fpsPrintTimer.stopTimer();
      fpsPrintTimer.startTimer();
      Log.print<Severity::debug>("FPS: ", fps);
    }

    game.update(timeElapsedF);
    game.render();

    window.update();
  }

  window.destroy();
  glfwTerminate();
  return 0;
}
