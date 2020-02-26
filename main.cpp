
/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <iostream>

#include "src/core/Application.h"

int main(int argc, char **argv)
{
  int retCode = 0;
  try
  {
    Application& app = Application::GetOrCreateApp();
    retCode = app.run();
  }
  catch (const char* msg)
  {
    std::cerr << msg << std::endl;
    retCode = -1;
  }

  Application::Cleanup();
  return retCode;
}
