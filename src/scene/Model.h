#pragma once
#include "../components/Primitive.h"
#include "GameObject.h"
#include "../components/Material.h"

class Model : public GameObject
{
protected:
  const Primitive* _mPrimitive;

public:
  // material to render the model...
  Material* material;
  virtual void copyTo(Cloneable* cloned) const override;

public:
  // some options
  bool renderWireMesh;

  Model(const Primitive* primitive = nullptr);
  virtual ~Model();
  virtual void update(float deltaT);
  virtual void draw(const glm::mat4& PV, const glm::mat4& M) const;

  virtual Model* clone() const override;
};