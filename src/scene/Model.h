#pragma once
#include "../components/Primitive.h"
#include "../components/Material.h"
#include "Node.h"
#include "./Skeleton.h"

class Model : public Node
{
protected:
  const Primitive* _mPrimitive;

public:
  // material to render the model...
  Material* material = nullptr;
  virtual void copyTo(Cloneable* cloned) const override;

public:
  // some options
  bool renderWireMesh;

  Model(const Primitive* primitive = nullptr);
  virtual ~Model();
  virtual void draw(const glm::mat4& PV) override;

  virtual Model* clone() const override;
};