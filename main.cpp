/*
 * main.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: ycai
 */

#include <iostream>
#include "src/core/Application.h"

int main(int argc, char **argv)
{
  int retCode = 0;
  try
  {
    Application& app = Application::GetOrCreateApp();
    retCode = app.runMainLoop();
  }
  catch (const char* msg)
  {
    std::cerr << msg << std::endl;
    retCode = -1;
  }

  Application::Cleanup();
  return retCode;
}
