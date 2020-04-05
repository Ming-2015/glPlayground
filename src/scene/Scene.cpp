#include "Scene.h"
#include <map>
#include <string>

void Scene::setActiveCamera(CameraBase* camera, bool addToScene)
{
  _mActiveCamera = camera;
  if (addToScene && !camera->getParent())
  {
    addChild(camera);
  }
}

CameraBase* Scene::getActiveCamera() const
{
  return _mActiveCamera;
}

void Scene::addLight(Light* light, bool addToScene) 
{
  if (!light) return;

  // not already in lights
  if (_mLights.find(light) == _mLights.end()) {
    _mLights.insert(light);
  }

  if (addToScene && !light->getParent()) {
    addChild(light);
  }
}

void Scene::removeLight(Light* light, bool removeFromScene)
{
  if (!light) return;
  _mLights.erase(light);

  if (removeFromScene && light->getRoot() == this)
  {
    light->setParent(nullptr);
  }
}

void Scene::draw() const
{
  glClear(GL_COLOR_BUFFER_BIT);
  if (_mActiveCamera == nullptr) return;

  glm::mat4 V = _mActiveCamera->getViewMatrix();
  glm::mat4 P = _mActiveCamera->getProjectionMatrix();

  Node::draw(P * V, glm::mat4(1.f));
}

void Scene::useLights(ShaderProgramManager& manager)
{
  auto allPrograms = manager.getAllResources();

  std::map<std::string, int> lightCounts;

  for (auto lightIt = _mLights.begin(); lightIt != _mLights.end(); lightIt++) {
    Light* light = *lightIt;
    std::string lightType = light->getUniformName();

    int lightIdx;
    if (lightCounts.find(lightType) == lightCounts.end()) {
      lightIdx = 0;
      lightCounts[lightType] = 1;
    }
    else {
      lightIdx = lightCounts[lightType]++;
    }

    for (auto programIt = allPrograms.begin(); programIt != allPrograms.end(); programIt++) {
      ShaderProgram* program = (*programIt).second;
      light->setProgramUniform(*program, lightIdx);
    }
  }
}