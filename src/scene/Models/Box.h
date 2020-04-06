#include "../Model.h"
#include "../../components/Primitive.h"

#pragma once
class Box : public Model
{
public:
  Box(PrimitiveManager& primitiveManager, float w = 1.f, float h = 1.f, float d = 1.f, bool invertNormal=false);
  virtual ~Box();
};