#include "Scene.h"

void Scene::setActiveCamera(Camera* camera, bool addToScene)
{
  _mActiveCamera = camera;
  if (addToScene)
  {
    addChild(camera);
  }
}

Camera* Scene::getActiveCamera() const
{
  return _mActiveCamera;
}

void Scene::draw() const
{
  glClear(GL_COLOR_BUFFER_BIT);
  if (_mActiveCamera == nullptr) return;

  glm::mat4 V = _mActiveCamera->getViewMatrix();
  glm::mat4 P = _mActiveCamera->getProjectionMatrix();

  Node::draw(P * V, glm::mat4());
}