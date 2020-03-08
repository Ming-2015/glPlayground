/*
 * Game.cpp
 *
 *  Created on: Feb 28, 2020
 *      Author: ycai
 */

#include "Game.h"

Game::Game() : 
  _mShaderManager(),
  _mTextureManager(),
  _mProgramManager(_mShaderManager),
  _mClearColor(0.1f, 0.7f, 0.5f, 1.0f),
  _mCurrentScene(new Scene())
{}

Game::~Game() 
{
  delete _mCurrentScene;
}

void Game::init() 
{
  // our temporary mesh
  Geometry* triangleMesh = new Geometry();
  triangleMesh->vertices = {
     -0.5f, -0.5f, 0.0f,  // lower left
     0.5f, -0.5f, 0.0f,   // lower right
     0.0f,  0.5f, 0.0f,   // top
     0.0f, -1.f, 0.0f    // bottom
  };
  triangleMesh->texCoords = {
    0, 0,
    1.f, 0,
    0.5f, 1.f,
    0.5f, -.5f
  };
  triangleMesh->indices = {
    0, 1, 2,
    0, 1, 3
  };
  triangleMesh->initArrayObject();

  // our model
  Model* triModel = new Model(triangleMesh);
  PhoonMaterial* mat = new PhoonMaterial(&_mProgramManager);

  triModel->material = mat;

  // set the textures for the material
  TextureInfo wallTex(
    "assets/wall.jpg",
    true
  );
  mat->diffuseTex = _mTextureManager.getOrCreate(wallTex);

  TextureInfo fujiwaraTex(
    "assets/fujiwara.jpg",
    true
  );
  mat->specularTex = _mTextureManager.getOrCreate(fujiwaraTex);

  _mCurrentScene->addChild(triModel);
  TargetCamera* cam = new TargetCamera();
  _mCurrentScene->setActiveCamera(cam);
  cam->setPosition(glm::vec3(0, 0, 2));

  glClearColor(_mClearColor.r, _mClearColor.g, _mClearColor.b, _mClearColor.a);
}

void Game::update(float deltaT)
{
  // rotate camera 45 degress per second around y axis
  TargetCamera* camera = dynamic_cast<TargetCamera*>(_mCurrentScene->getActiveCamera());
  float angle = 45.f * deltaT / 1000.f;
  glm::vec3 axis(0, 1.f, 0);
  glm::vec3 currentPos = camera->getPosition();
  glm::vec3 newPos = glm::rotate(glm::radians(angle), axis) * glm::vec4( currentPos, 1.0f );
  camera->setPosition(newPos);


  if (_mCurrentScene != nullptr)
  {
    _mCurrentScene->update(deltaT);
  }
}

void Game::render() const
{
  glClear(GL_COLOR_BUFFER_BIT);
  if (_mCurrentScene != nullptr)
  {
    _mCurrentScene->draw();
  }
}

void Game::setWindowSize(int width, int height)
{
  glViewport(0, 0, width, height);
  
  if (_mCurrentScene != nullptr)
  {
    PerspectiveCamera* camera = dynamic_cast<PerspectiveCamera*>(_mCurrentScene->getActiveCamera());
    if (camera) {
      camera->setAspectRatio(float(width) / float(height));
      Log.print<Severity::debug>("Managed to change aspect ratio!");
    }
  }
}

void Game::onKeyEvent(int key, int scancode, int action, int mods)
{
  // key events here...
}