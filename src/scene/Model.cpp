#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Model::Model(const Primitive* primitive)
  : Node(), _mPrimitive(primitive), material(nullptr), renderWireMesh(false)
{}

Model::~Model()
{}

void Model::draw(const glm::mat4& PV)
{
  if (_mPrimitive == nullptr) return;

  const glm::mat4& model = getGlobalTransform();
  glm::mat4 PVM = PV * model;
  glm::mat3 modelMat3(model);
  glm::mat3 normal = glm::inverseTranspose(modelMat3);

  if (material != nullptr) 
  {
    material->use();
    material->setModelMatrix(model);
    material->setNormalMatrix(normal);
    material->setProjViewModelMatrix(PVM);
  }

  // draw line if wire mesh
  if (renderWireMesh) 
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  _mPrimitive->render();

  // revert
  if (renderWireMesh) 
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  Node::draw(PV);
}

void Model::copyTo(Cloneable* cloned) const
{
  Node::copyTo(cloned);
  Model* copy = dynamic_cast<Model*>(cloned);
  if (!copy)
  {
    Log.print<Severity::warning>("Failed to cast Model during clone");
    return;
  }

  copy->material = material;
  copy->renderWireMesh = renderWireMesh;
}

Model* Model::clone() const
{
  Model* model = new Model(_mPrimitive);
  copyTo(model);
  return model;
}