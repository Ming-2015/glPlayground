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
  Application app;

  Log.print<Severity::info>("Starting the application...");
  app.init();
  retCode = app.runMainLoop();
  app.destroy();

  return retCode;
}