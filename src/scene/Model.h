#pragma once
#include "../components/Primitive.h"
#include "GameObject.h"
#include "../components/Material.h"

class Model : public GameObject
{
protected:
  const Primitive* _mMesh;

public:
  // material to render the model...
  Material* material;

public:
  // some options
  bool renderWireMesh;

  Model(const Primitive* mesh = nullptr);
  virtual ~Model();
  virtual void update(float deltaT);
  virtual void draw(const glm::mat4& PV, const glm::mat4& M) const;
};