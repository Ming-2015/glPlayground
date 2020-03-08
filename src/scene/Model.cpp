#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Model::Model(const Geometry& mesh)
  : GameObject(), _mMesh(mesh)
{}

Model::~Model()
{}

void Model::update(float deltaT)
{
  GameObject::update(deltaT);
}

void Model::draw(const glm::mat4& PV, const glm::mat4& M) const
{
  if (material) {
    material->use();
  }

  // TODO: do something with the matrices
  _mMesh.render();

  GameObject::draw(PV, M);
}