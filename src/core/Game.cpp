/*
 * Game.cpp
 *
 *  Created on: Feb 28, 2020
 *      Author: ycai
 */

#include "Game.h"

Game::Game()
  : clearColor(0.2f, 0.2f, 0.3f, 1.0f), 
    gameObjects(),
    basicProgram()
{}

Game::~Game() 
{

}

void Game::init() 
{
  basicProgram.initShaderProgram(
    "./shaders/VertexShader.glsl",
    "./shaders/FragmentShader.glsl"
  );

  gameObjects.emplace_back();
  gameObjects[0].vertices = {
     -0.5f, -0.5f, 0.0f,  // lower left
     0.5f, -0.5f, 0.0f,   // lower right
     0.0f,  0.5f, 0.0f    // top
  };
  gameObjects[0].indices = {
    0, 1, 2
  };
  gameObjects[0].initArrayBuffer();
}

void Game::render() 
{
  // set the clear color
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

  // clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  basicProgram.useProgram();
  for (auto gameObject : gameObjects) 
  {
    gameObject.render();
  }
}

void Game::update(float deltaT) 
{
  for (auto gameObject : gameObjects)
  {
    gameObject.update(deltaT);
  }
}
