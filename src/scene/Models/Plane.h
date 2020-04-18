#pragma once

#include "../Model.h"
#include "../../components/Primitive.h"

#pragma once
class Plane : public Model
{
public:
  Plane(PrimitiveManager& primitiveManager, float startX = 0, float startY = 0, float endX = 1, float endY = 1);
  virtual ~Plane();
};