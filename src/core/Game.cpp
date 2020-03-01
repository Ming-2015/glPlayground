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
    shaderManager(),
    basicProgram()
{}

Game::~Game() 
{

}

void Game::init() 
{
  ShaderInfo vertexShaderInfo = {
    "./shaders/VertexShader.glsl",
    GL_VERTEX_SHADER
  };

  ShaderInfo fragmentShaderInfo = {
    "./shaders/FragmentShader.glsl",
    GL_FRAGMENT_SHADER
  };

  const Shader& vertexShader = shaderManager.getOrCreate(vertexShaderInfo);
  const Shader& fragmentShader = shaderManager.getOrCreate(fragmentShaderInfo);

  basicProgram.initShaderProgram(
    vertexShader,
    fragmentShader
  );

  gameObjects.emplace_back();
  gameObjects[0].vertices = {
     -0.5f, -0.5f, 0.0f,  // lower left
     0.5f, -0.5f, 0.0f,   // lower right
     0.0f,  0.5f, 0.0f,   // top
     0.0f, -1.f, 0.0f    // bottom
  };
  gameObjects[0].indices = {
    0, 1, 2,
    0, 1, 3
  };
  gameObjects[0].initArrayObject();
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
