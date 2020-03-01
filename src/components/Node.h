#pragma once
#include <glm/glm.hpp>

class Node
{
public:
  virtual void draw(const glm::mat4& C) const = 0;
  virtual void update(float deltaT) = 0;
};