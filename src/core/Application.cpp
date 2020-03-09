/*
 * App.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include "Application.h"
#include <stdexcept>

// NON STATIC MEMBERS
Application::Application()
{}

Application::~Application()
{}

void Application::init()
{
  glfwInit();
  game.init();
}

void Application::destroy()
{
  glfwTerminate();
}

int Application::runMainLoop()
{
  float fps = 0;
  fpsPrintTimer.startTimer();
  updateTimer.startTimer();

  while(!game.shouldClose())
  {
    float timeElapsedF = updateTimer.stopTimer();
    updateTimer.startTimer();

    // print every 5 seconds
    if (fpsPrintTimer.getTimeElapsed() > 5000)
    {
      if (timeElapsedF > 0)
      {
        fps = 1000.0f / timeElapsedF;
      }
      fpsPrintTimer.stopTimer();
      fpsPrintTimer.startTimer();
      Log.print<Severity::debug>("FPS: ", fps);
    }

    // update the game
    game.update(timeElapsedF);
    game.render();
  }

  return 0;
}