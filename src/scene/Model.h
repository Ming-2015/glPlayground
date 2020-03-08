#pragma once
#include "../components/Geometry.h"
#include "GameObject.h"
#include "../components/Material.h"

class Model : public GameObject
{
protected:
  const Geometry* _mMesh;

public:
  // material to render the model...
  Material* material = nullptr;

public:

  Model(const Geometry* mesh);
  virtual ~Model();
  virtual void update(float deltaT);
  virtual void draw(const glm::mat4& PV, const glm::mat4& M) const;
};