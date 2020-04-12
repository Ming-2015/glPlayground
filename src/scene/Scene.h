#pragma once

#include "Node.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "../components/ShaderProgram.h"

#include <glm/glm.hpp>
#include <vector>
#include <set>

class Scene : public virtual Node
{
protected:
  // activeCamera needs to be added to the scene separately to be part of the scene
  // i.e. scene->addCamera(cam); scene->addChild(cam);
  CameraBase* _mActiveCamera = nullptr;
  std::set<LightBase *> _mLights;

  virtual void copyTo(Cloneable* cloned) const override;
public:

  // camera
  void setActiveCamera(CameraBase* camera, bool addToScene=true);
  CameraBase* getActiveCamera() const;

  // lights
  void addLight(LightBase* light, bool addToScene = true);
  void removeLight(LightBase* light, bool removeFromScene = true);
  const std::set<LightBase*>& getLights() { return _mLights; }

  // this should be called before a draw call to activate lights!
  void prepShaderPrograms(ShaderProgramManager& manager);

  // NOTE: for scenes, PV and M will have no effect at all
  virtual void draw(const glm::mat4& PV, const glm::mat4& M) const override { draw(); }
  virtual void draw() const;

  virtual Scene* clone() const override;
};