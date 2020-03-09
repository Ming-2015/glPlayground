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
  _mClearColor(0.1f, 0.7f, 0.5f, 1.0f),
  _mCurrentScene(new Scene()),
  _mWindow(1600, 900, "Window"),
  _mCamera(nullptr)
{}

Game::~Game() 
{
  delete _mCurrentScene;
}

void Game::init() 
{
  _mWindow.initialize();
  _mWindow.addObservable(this);
  _mWindow.setContextCurrent();
  _mWindow.setCursorMode(GLFW_CURSOR_DISABLED);
    
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
  _mCamera = new FreeCamera();
  _mCamera->setPosition(glm::vec3(0, 0, 2));
  _mCurrentScene->setActiveCamera(_mCamera);

  glClearColor(_mClearColor.r, _mClearColor.g, _mClearColor.b, _mClearColor.a);
}

void Game::render() 
{
  //_mWindow.setContextCurrent();

  glClear(GL_COLOR_BUFFER_BIT);
  if (_mCurrentScene != nullptr)
  {
    _mCurrentScene->draw();
  }

  _mWindow.nextFrame();
}

void Game::update(float deltaT)
{
  if (goUp)
  {
    glm::vec3 forward = deltaT / 1000.f * _mMoveSpeed * _mCamera->getForwardDirection();
    _mCamera->setPosition(_mCamera->getPosition() + forward);
  }

  if (goDown)
  {
    glm::vec3 backward = deltaT / 1000.f * -_mMoveSpeed * _mCamera->getForwardDirection();
    _mCamera->setPosition(_mCamera->getPosition() + backward);
  }

  if (goLeft)
  {
    glm::vec3 left = deltaT / 1000.f * -_mMoveSpeed * glm::normalize(
      glm::cross(_mCamera->getForwardDirection(), _mCamera->getUp())
    );
    _mCamera->setPosition(_mCamera->getPosition() + left);
  }

  if (goRight)
  {
    glm::vec3 right = deltaT / 1000.f * _mMoveSpeed * glm::normalize(
      glm::cross(_mCamera->getForwardDirection(), _mCamera->getUp())
    );
    _mCamera->setPosition(_mCamera->getPosition() + right);
  }

  if (_mCurrentScene != nullptr)
  {
    _mCurrentScene->update(deltaT);
  }
}

void Game::onKey(int key, int scancode, int action, int mods)
{
  bool flag = false;
  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    flag = true;
  }
  else if (action == GLFW_RELEASE)
  {
    flag = false;
  }

  // key events here...
  switch (key)
  {
  case GLFW_KEY_W:
  case GLFW_KEY_UP:
    goUp = flag;
    break;

  case GLFW_KEY_S:
  case GLFW_KEY_DOWN:
    goDown = flag;
    break;

  case GLFW_KEY_A:
  case GLFW_KEY_LEFT:
    goLeft = flag;
    break;

  case GLFW_KEY_D:
  case GLFW_KEY_RIGHT:
    goRight = flag;
    break;

  case GLFW_KEY_ESCAPE:
    done = flag;
    break;
  }
}

void Game::onCursorPos(double xPos, double yPos)
{
  if (firstTime)
  {
    cursorX = xPos;
    cursorY = yPos;
    firstTime = false;
    return;
  }

  cursorMoveX = xPos - cursorX;
  cursorMoveY = cursorY - yPos;
  cursorX = xPos;
  cursorY = yPos;


  if (cursorMoveX)
  {
    float factor = cursorMoveX * _mHorizontalSpeed;
    yaw += factor;
    cursorMoveX = 0;
  }

  if (cursorMoveY)
  {
    float factor = cursorMoveY * _mVerticalSpeed;
    pitch += factor;
    if (pitch > 89.9f)
    {
      pitch = 89.9f;
    }
    if (pitch < -89.9f)
    {
      pitch = -89.9f;
    }
    cursorMoveY = 0;
  }

  if (!_mCamera->isUsingTarget())
  {
    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    _mCamera->setForwardDirection(forward);
  }
}

void Game::onMouseButton(int key, int action, int mods)
{

}

void Game::onResize(int width, int height)
{
  glViewport(0, 0, width, height);

  if (_mCurrentScene != nullptr)
  {
    if (_mCamera) {
      _mCamera->setAspectRatio(float(width) / float(height));
      Log.print<Severity::debug>("Managed to change aspect ratio!");
    }
  }
}

bool Game::shouldClose() const
{
  return done || _mWindow.shouldClose();
}