/*
 * Game.cpp
 *
 *  Created on: Feb 28, 2020
 *      Author: ycai
 */

#include "Game.h"

Game::Game()
  : clearColor(0.2f, 0.2f, 0.3f, 1.0f), 
  meshes(),
  models(),
  _mShaderManager(),
  _mProgramManager(_mShaderManager),
  _mDefaultProgram(nullptr)
{}

Game::~Game() 
{

}

void Game::init() 
{
  ShaderInfo vertexShaderInfo(
    "./shaders/VertexShader.glsl",
    GL_VERTEX_SHADER
  );

  ShaderInfo fragmentShaderInfo(
    "./shaders/FragmentShader.glsl",
    GL_FRAGMENT_SHADER
  );

  ShaderProgramInfo programInfo(
    vertexShaderInfo,
    fragmentShaderInfo
  );

  _mDefaultProgram = _mProgramManager.getOrCreate(programInfo);

  auto itMesh = meshes.emplace("Triangle", Mesh());
  Mesh& triangleMesh = (*itMesh.first).second;

  triangleMesh.vertices = {
     -0.5f, -0.5f, 0.0f,  // lower left
     0.5f, -0.5f, 0.0f,   // lower right
     0.0f,  0.5f, 0.0f,   // top
     0.0f, -1.f, 0.0f    // bottom
  };
  triangleMesh.indices = {
    0, 1, 2,
    0, 1, 3
  };
  triangleMesh.initArrayObject();

  auto itModel = models.emplace("TriModel", Model(triangleMesh));
  Model& triModel = (*itModel.first).second;
}

void Game::render() 
{
  // set the clear color
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

  // clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  _mProgramManager.useProgram(_mDefaultProgram);
  for (auto pair : models) 
  {
    pair.second.draw(glm::mat4());
  }
}

void Game::update(float deltaT) 
{
  float timeValue = Timer::GetCurrentTime().time_since_epoch().count();
  float greenValue = sin(timeValue) / 2.0f + 0.5f;

  //Uniform* uniform = _mDefaultProgram->getUniform("globalColor");

  for (auto pair : models)
  {
    pair.second.update(deltaT);
  }
}
