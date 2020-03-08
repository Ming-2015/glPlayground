/*
 * main.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include <iostream>
#include "../utils/Logger.h"
#include "Application.h"

int main(int argc, char **argv)
{
  int retCode = 0;
  try
  {
    Log.print<Severity::info>("Starting the application...");
    Application& app = Application::GetOrCreateApp();
    retCode = app.runMainLoop();
  }
  catch (std::exception e)
  {
    Log.print<Severity::critical>(e.what());
    retCode = -1;
  }

  Application::Cleanup();
  return retCode;
}