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

void Scene::addLight(LightBase* light, bool addToScene) 
{
  if (!light) return;
  _mLights.insert(light);

  if (addToScene) 
  {
    Node* lightNode = dynamic_cast<Node*>(light);
    if (!lightNode->getParent()) 
    {
      addChild(lightNode);
    }
  }
}

void Scene::removeLight(LightBase* light, bool removeFromScene)
{
  if (!light) return;
  _mLights.erase(light);

  if (removeFromScene)
  {
    Node* lightNode = dynamic_cast<Node*>(light);
    if (lightNode->getRoot() == this)
    {
      lightNode->setParent(nullptr);
    }
  }
}

void Scene::draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  if (_mActiveCamera == nullptr) return;

  glm::mat4 V = _mActiveCamera->getViewMatrix();
  glm::mat4 P = _mActiveCamera->getProjectionMatrix();

  Node::draw(P * V);
}

void Scene::prepShaderPrograms(ShaderProgramManager& manager)
{
  auto allPrograms = manager.getAllResources();

  std::map<std::string, int> lightCounts;

  for (auto lightIt = _mLights.begin(); lightIt != _mLights.end(); lightIt++) 
  {
    LightBase* light = *lightIt;
    std::string lightType = light->getUniformName();

    int lightIdx;
    if (lightCounts.find(lightType) == lightCounts.end()) 
    {
      lightIdx = 0;
      lightCounts[lightType] = 1;
    }
    else
      lightIdx = lightCounts[lightType]++;

    for (auto programIt = allPrograms.begin(); programIt != allPrograms.end(); programIt++) 
    {
      ShaderProgram* program = (*programIt).second;
      light->setProgramUniform(*program, lightIdx);
    }
  }

  if (_mActiveCamera) {
    for (auto programIt = allPrograms.begin(); programIt != allPrograms.end(); programIt++)
    {
      ShaderProgram* program = (*programIt).second;
      _mActiveCamera->setProgramUniform(*program);
    }
  }
}


Scene* Scene::clone() const
{
  Scene* clone = new Scene();
  copyTo(clone);
  return clone;
}

void Scene::copyTo(Cloneable* cloned) const
{
  Node::copyTo(cloned);
  Scene* clonedScene = dynamic_cast<Scene*>(cloned);
  if (!clonedScene)
  {
    Log.print<Severity::warning>("Failed to cast to Scene in clone");
    return;
  }

  if (_mActiveCamera) 
  {
    auto findCamera = breadthFirstSearch(_mActiveCamera);
    if (findCamera.size() > 0) 
    {
      CameraBase* clonedCamera = dynamic_cast<CameraBase*>(clonedScene->getDescendentByIndices(findCamera));
      clonedScene->_mActiveCamera = clonedCamera;
      
      if (!clonedCamera)
        Log.print<Severity::warning>("Failed to cast found CameraBase during Scene clone");
    }
    else 
    {
      clonedScene->_mActiveCamera = _mActiveCamera->clone();
    }
  }

  for (LightBase* light : _mLights)
  {
    Node* lightNode = dynamic_cast<Node*>(light);
    if (!lightNode)
    {
      Log.print<Severity::warning>("Found a non-node light, which cannot be cloned!");
      continue;
    }

    auto findLight = breadthFirstSearch(lightNode);
    if (findLight.size() > 0)
    {
      LightBase* clonedLight = dynamic_cast<LightBase*>(clonedScene->getDescendentByIndices(findLight));
      if (clonedLight)
        clonedScene->_mLights.insert(clonedLight);
      else
        Log.print<Severity::warning>("Failed to cast found Light during Scene clone");
    }
    else
    {
      clonedScene->_mLights.insert(dynamic_cast<LightBase*>(lightNode->clone()));
    }
  }
}