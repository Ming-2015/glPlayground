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

  Timer updateElapsed;
  updateElapsed.startTimer(true);
  Timer renderElapsed;
  renderElapsed.startTimer(true);

  float numFrames = 0;
  bool printFps = false;

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

      if (printFps) 
      {
        Log.print<Severity::debug>("FPS of last frame: ", fps);
        Log.print<Severity::debug>("Number of frames passed since last report: ", numFrames);
        Log.print<Severity::debug>("Update Time Elapsed: ", updateElapsed.stopTimer(), "ms");
        Log.print<Severity::debug>("Render Time Elapsed: ", renderElapsed.stopTimer(), "ms");
      }

      updateElapsed.startTimer(true);
      renderElapsed.startTimer(true);
    }

    // update the game
    updateElapsed.resumeTimer();
    game.update(timeElapsedF);
    updateElapsed.pauseTimer();

    renderElapsed.resumeTimer();
    game.render();
    renderElapsed.pauseTimer();

    numFrames++;
  }

  return 0;
}