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
  // set the clear color
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

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
  triangleMesh.texCoords = {
    0, 0,
    1.f, 0,
    0.5f, 1.f,
    0.5f, -.5f
  };
  triangleMesh.indices = {
    0, 1, 2,
    0, 1, 3
  };

  triangleMesh.initArrayObject();

  auto itModel = models.emplace("TriModel", Model(triangleMesh));
  Model& triModel = (*itModel.first).second;

  TestMaterial* mat = new TestMaterial(_mProgramManager, *_mDefaultProgram);
  triModel.material = mat;

  mat->diffuseTex = new Texture();
  mat->diffuseTex->loadFromFile("assets/wall.jpg", true);

  mat->specularTex = new Texture();
  mat->specularTex->loadFromFile("assets/fujiwara.jpg", true);
}

void Game::render() 
{
  // clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  for (auto pair : models) 
  {
    pair.second.draw(glm::mat4());
  }
}

void Game::update(float deltaT) 
{
  for (auto pair : models)
  {
    pair.second.update(deltaT);
  }
}
