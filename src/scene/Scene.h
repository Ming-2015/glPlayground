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

  // activeCamera will be part of the nodes
  Camera* activeCamera = nullptr;
  Window* _mWindow = nullptr;
  std::vector<Node*> rootNodes;

public:

  Scene(Window* window);
  virtual ~Scene();

  virtual void update(float deltaT);

  // NOTE: for scenes, PV and M will have no effect at all
  virtual void draw(const glm::mat4 PV, const glm::mat4& M) const
  {
    draw();
  }

  virtual void draw() const;
};