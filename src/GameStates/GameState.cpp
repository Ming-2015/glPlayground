#include "GameState.h"

GameState::GameState(const GameResources& resources)
  : _mResources(resources)
{
  _mResources.window.addObservable(this);
}

GameState::~GameState()
{
  destroy();
}

void GameState::load() {
  if (_mIsLoaded) return;

  // For drawing frame buffer to the screen (window's frame buffer)
  screenShader = new ScreenShader(&_mResources.shaderProgramManager);
  screenShader->screenTextureId = _mResources.window.getColorBuffer();
  plane = new Plane(_mResources.primitiveManager, -1, -1, 1, 1);
  plane->material = screenShader;

  _onLoad();
  _mIsLoaded = true;
}

void GameState::destroy() {
  if (!_mIsLoaded) return;

  _onDestroy();
  if (screenShader) delete screenShader;
  if (plane) delete plane;

  _mIsLoaded = false;
}

void GameState::draw() {
  // render to custom window buffer
  glBindFramebuffer(GL_FRAMEBUFFER, _mResources.window.getFrameBuffer());
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_FRAMEBUFFER_SRGB);

  _mScene.prepShaderPrograms(_mResources.shaderProgramManager);
  _onDraw();
  _mScene.draw();

  // use default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FRAMEBUFFER_SRGB);

  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  plane->draw(glm::mat4(1.0f));
}

void GameState::update(float deltaT) {
  _onUpdate(deltaT);
  _mScene.update(deltaT);
  _mResources.primitiveManager.update(deltaT);
}

void GameState::onResize(int width, int height)
{
  if (screenShader) {
    screenShader->screenTextureId = _mResources.window.getColorBuffer();
  }
  glViewport(0, 0, width, height);
}