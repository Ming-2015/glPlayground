#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include <iostream>
#include <set>
#include "../utils/Logger.h"
#include "../utils/ResourceManager.hpp"

// used for storing primitive data
struct PrimitiveData {
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> tangents;
  std::vector<float> bitangents;

  unsigned int numComponents = 2;
  std::vector<float> texCoords;
  unsigned int numComponents_2 = 2;
  std::vector<float> texCoords_2;
  unsigned int numComponents_3 = 2;
  std::vector<float> texCoords_3;

  std::vector<float> weights;
  std::vector<unsigned int> joints;
  std::vector<unsigned int> indices;
};

class Primitive;
class PrimitiveObservable 
{
public:
  virtual void onShouldRender(const Primitive* p) = 0;
};

// NOTE: this class does not support interleaved buffer for now...
// Only reason for an interleaved buffer would be performance gain
// But I will not focus on getting more performance for now
class Primitive
{
private:
  // for keeping track of number of game objects made
  static int objectCount;

  // unique object id, based on objectCount
  int _mUniqueId;

  // vertex position
  unsigned int numVertices    = 0;
  bool _mHasVerticesVbo       = false;
  unsigned int _mVerticesVbo  = 0;

  // vertex normal
  unsigned int numNormals     = 0;
  bool _mHasNormalsVbo        = false;
  unsigned int _mNormalsVbo   = 0;

  // vertex texture coordinate
  unsigned int numTexCoords   = 0;
  bool _mHasTexVbo            = false;
  unsigned int _mTexVbo       = 0;

  // vertex texture coordinate 2
  unsigned int numTexCoords_2 = 0;
  bool _mHasTexVbo_2          = false;
  unsigned int _mTexVbo_2     = 0;

  // vertex texture coordinate 3
  unsigned int numTexCoords_3 = 0;
  bool _mHasTexVbo_3          = false;
  unsigned int _mTexVbo_3     = 0;

  // vertex tangent
  unsigned int numTangents    = 0;
  bool _mHasTangentsVbo       = false;
  unsigned int _mTangentsVbo  = 0;

  // vertex bitangent
  unsigned int numBitangents  = 0;
  bool _mHasBitangentsVbo     = false;
  unsigned int _mBitangentsVbo = 0;

  // bone weights
  unsigned int numWeights     = 0;
  bool _mHasWeightsVbo        = false;
  unsigned int _mWeightsVbo   = 0;

  // bone joints
  unsigned int numJoints      = 0;
  bool _mHasJointsVbo         = false;
  unsigned int _mJointsVbo    = 0;

  // triangular faces
  unsigned int numFaces       = 0;
  bool _mHasIndicesEbo        = false;
  unsigned int _mIndicesEbo   = 0;

  // object VAO
  bool _mHasObjectVao         = false;
  unsigned int _mObjectVao    = 0;

  std::set<PrimitiveObservable*> observers;

public:
  static const int ATTRIBUTE_POSITION   = 0;
  static const int ATTRIBUTE_NORMAL     = 1;
  static const int ATTRIBUTE_TEX        = 2;
  static const int ATTRIBUTE_TANGENT    = 3;
  static const int ATTRIBUTE_BITANGENT  = 4;
  static const int ATTRIBUTE_JOINT      = 5;
  static const int ATTRIBUTE_WEIGHT     = 6;
  static const int ATTRIBUTE_TEX_2      = 7;
  static const int ATTRIBUTE_TEX_3      = 8;

  static const int SIZE_POSITION = 3;
  static const int SIZE_NORMAL = 3;
  //static const int SIZE_TEX = 2;
  static const int SIZE_TANGENT = 3;
  static const int SIZE_BITANGENT = 3;
  static const int SIZE_JOINT = 4;
  static const int SIZE_WEIGHT = 4;

  static const int SIZE_FACE = 3;
  static const int MAX_TEX_COORDINATE_SUPPORTED = 3;

public:
  virtual void bindVao() const;
  virtual void render() const;

  void addObservable(PrimitiveObservable* o);
  void removeObservable(PrimitiveObservable* o);

  // should be called after setting all the vertex attributes below!
  void initArrayObject(const PrimitiveData* data);
  void deleteArrayObject();

  Primitive();
  virtual ~Primitive();
};

class PrimitiveManager : public ResourceManager<std::string, PrimitiveData, Primitive>, public PrimitiveObservable
{
protected:
  Primitive* const create(const std::string& key, const PrimitiveData& data) override;
  void destroy(Primitive* const value);

  // manages when a primitive is drawn...
  const Primitive* _lastDrawnPrimitive = nullptr;

public:
  PrimitiveManager();
  virtual ~PrimitiveManager() { clear(); }
  virtual void onShouldRender(const Primitive* p) override;
  virtual void update(float deltaT);
};