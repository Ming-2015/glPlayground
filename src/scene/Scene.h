#pragma once

#include "Node.h"
#include "Camera.h"
#include "Model.h"
#include "../components/Window.h"

#include <glm/glm.hpp>
#include <vector>

class Scene : public Node
{
protected:
  // activeCamera needs to be added to the scene separately to be part of the scene
  // i.e. scene->addCamera(cam); scene->addChild(cam);
  Camera* _mActiveCamera = nullptr;

public:

  void setActiveCamera(Camera* camera, bool addToScene=true);
  Camera* getActiveCamera() const;

  // NOTE: for scenes, PV and M will have no effect at all
  virtual void draw(const glm::mat4 PV, const glm::mat4& M) const
  {
    draw();
  }

  virtual void draw() const;
};