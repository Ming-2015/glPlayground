#include "Scene.h"


Scene::Scene(Window* window)
  : _mWindow(window)
{

}

Scene::~Scene()
{
  for (Node* n : rootNodes)
  {
    delete n;
  }
  rootNodes.clear();
}

void Scene::update(float deltaT)
{
  for (Node* n : rootNodes)
  {
    n->update(deltaT);
  }
}

void Scene::draw() const
{
  if (!activeCamera) return;

  glm::mat4 V = activeCamera->getViewMatrix();
  glm::mat4 P = activeCamera->getProjectionMatrix();

  for (Node* n : rootNodes)
  {
    n->draw(P * V, glm::mat4());
  }
}