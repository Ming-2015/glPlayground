#include "Plane.h"


Plane::Plane(PrimitiveManager& primitiveManager, float startX, float startY, float endX, float endY)
  : Model(nullptr)
{
  Primitive* planePrimitive = nullptr;
  std::stringstream idStream;
  idStream << "createPlane_" << startX << "_" << startY << "_" << endX << "_" << endY;
  std::string id = idStream.str();

  if (!(planePrimitive = primitiveManager.find(id)))
  {
    PrimitiveData planeMesh;

    glm::vec3 p1(startX, startY, 0);
    glm::vec3 p2(endX, startY, 0);
    glm::vec3 p3(endX, endY, 0);
    glm::vec3 p4(startX, endY, 0);

    planeMesh.vertices = {
      p1.x, p1.y, p1.z,
      p2.x, p2.y, p2.z,
      p3.x, p3.y, p3.z,
      p4.x, p4.y, p4.z,
    };

    planeMesh.normals = {
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,
      0, 0, -1
    };

    planeMesh.texCoords = {
      0, 0,
      1.f, 0,
      1.f, 1.f,
      0, 1.f,
    };

    planeMesh.indices = {
      0, 1, 2,
      0, 2, 3,
    };

    planePrimitive = primitiveManager.insert(id, planeMesh);
  }

  _mPrimitive = planePrimitive;
}

Plane::~Plane()
{

}