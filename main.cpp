/*
 * main.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include <iostream>
#include "src/utils/Logger.h"
#include "src/core/Application.h"

int main(int argc, char **argv)
{
  int retCode = 0;
  try
  {
    Log.print<Severity::info>("Starting the application...");
    Application& app = Application::GetOrCreateApp();
    retCode = app.runMainLoop();
  }
  catch (const char* msg)
  {
    Log.print<Severity::error>(msg);
    retCode = -1;
  }

  Application::Cleanup();
  return retCode;
}