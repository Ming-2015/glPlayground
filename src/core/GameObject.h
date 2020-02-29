#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>


class GameObject
{
private:
  static int objectCount;
  int _mUniqueId;
  unsigned int VBO;

public:
  GameObject();
  virtual ~GameObject();

  void initArrayBuffer();

  std::vector<float> vertices;
  std::vector<float> colors;
  std::vector<float> normals;
  std::vector<float> texCoords;

  // triangles only
  std::vector<float> indices;
};