/*
 * Game.cpp
 *
 *  Created on: Feb 28, 2020
 *      Author: ycai
 */

#include "Game.h"
#include <GLFW/glfw3.h>

Game::Game() : 
  _mShaderManager(),
  _mTextureManager(),
  _mProgramManager(_mShaderManager),
  _mPrimitiveManager(),
  _mWindow(1920, 1080, "Window"),
  _mCurrentState(nullptr),
  resources(
    _mShaderManager, 
    _mProgramManager, 
    _mTextureManager, 
    _mPrimitiveManager, 
    _mWindow
  )
{}

Game::~Game() 
{
  if (_mCurrentState)
  {
    _mCurrentState->destroy();
    delete _mCurrentState;
  }
}

void Game::init() 
{
  _mWindow.initialize();
  _mWindow.addObservable(this);
  _mWindow.setContextCurrent();
  _mWindow.setCursorMode(GLFW_CURSOR_DISABLED);

  _mCurrentState = new TestTriangle(resources);
  _mCurrentState->load();
}

void Game::render() 
{
  if (_mCurrentState)
  {
    _mCurrentState->draw();
  }

  _mWindow.nextFrame();
}

void Game::update(float deltaT)
{
  if (_mCurrentState)
  {
    _mCurrentState->update(deltaT);
  }
}

bool Game::shouldClose() const
{
  return done || _mWindow.shouldClose();
}

void Game::onKey(int key, int scancode, int action, int mods)
{}

void Game::onCursorPos(double xPos, double yPos)
{}

void Game::onMouseButton(int key, int action, int mods)
{}

void Game::onResize(int width, int height)
{}