#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include "../utils/Logger.h"

class Mesh
{
private:
  // for keeping track of number of game objects made
  static int objectCount;

  static const int ATTRIBUTE_POSITION = 0;
  static const int ATTRIBUTE_NORMAL = 1;
  static const int ATTRIBUTE_TEX = 2;

  static const int SIZE_POSITION = 3;
  static const int SIZE_NORMAL = 3;
  static const int SIZE_TEX = 2;

  // unique object id, based on objectCount
  int _mUniqueId;
  unsigned int _mVerticesVbo;
  unsigned int _mNormalsVbo;
  unsigned int _mTexVbo;
  unsigned int _mIndicesEbo;

  unsigned int _mObjectVao;

public:
  Mesh();
  virtual ~Mesh();
  virtual void render() const;

  // should be called after setting all the vertex attributes below!
  virtual void initArrayObject();

  // vertex attributes
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texCoords;

  // elements, assuming triangles
  std::vector<unsigned int> indices;

  // some options
  bool renderWireMesh;
};