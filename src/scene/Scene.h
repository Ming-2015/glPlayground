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
  std::set<Light* > _mLights;

  virtual void copyTo(Cloneable* cloned) const override;
public:

  // camera
  void setActiveCamera(CameraBase* camera, bool addToScene=true);
  CameraBase* getActiveCamera() const;

  // lights
  void addLight(Light* light, bool addToScene = true);
  void removeLight(Light* light, bool removeFromScene = true);
  const std::set<Light*>& getLights() { return _mLights; }

  // this should be called before a draw call to activate lights!
  void prepShaderPrograms(ShaderProgramManager& manager);

  // NOTE: for scenes, PV will have no effect at all
  virtual void draw(const glm::mat4& PV) override { draw(); }
  virtual void draw();

  virtual Scene* clone() const override;
};