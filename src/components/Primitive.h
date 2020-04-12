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
  std::vector<float> texCoords;
  std::vector<float> texCoords_2;
  std::vector<float> texCoords_3;
  std::vector<float> tangents;
  std::vector<float> weights;
  std::vector<unsigned int> joints;
  std::vector<unsigned int> indices;
};

// Used for managing primitive information
// PrimitiveData loaders should generate a unique ID to help identification
// @example: 
//   Primitive* primitive = PrimitiveManager.find(PrimitiveInfo(id));
//   if (!primitive) {
//     PrimitiveData data; // get the data somehow
//     primitive = PrimitiveManager.getOrCreate(id, &data);
//   }
class PrimitiveInfo : public ResourceInfo<PrimitiveInfo>
{
public:
  std::string id;
  PrimitiveData* data;

  // Initialize a primitive info. Data should be allocated dynamically!
  // Data can also be nullptr since data will not be used for comparison, 
  //   but it will not be valid for creation!
  PrimitiveInfo(const std::string& id, PrimitiveData* data = nullptr);
  PrimitiveInfo(const PrimitiveInfo& other) = default;
  PrimitiveInfo& operator=(const PrimitiveInfo&) = default;

  // Will not manage or delete the data!
  ~PrimitiveInfo() {}

  // getters
  bool operator< (const PrimitiveInfo& other) const;
  bool operator== (const PrimitiveInfo& other) const;
  bool isValidForCreation() const;
  
  // const state getters
  const PrimitiveData* getPrimitiveData() const { return data; }
  const std::string& getId() const { return id; }

  // convert to string (for debug/comparison)
  const std::string toString() const;
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
  static const int ATTRIBUTE_JOINT      = 4;
  static const int ATTRIBUTE_WEIGHT     = 5;
  static const int ATTRIBUTE_TEX_2      = 6;
  static const int ATTRIBUTE_TEX_3      = 7;

  static const int SIZE_POSITION = 3;
  static const int SIZE_NORMAL = 3;
  static const int SIZE_TEX = 2;
  static const int SIZE_TANGENT = 4;
  static const int SIZE_JOINT = 4;
  static const int SIZE_WEIGHT = 4;

  static const int SIZE_FACE = 3;

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

class PrimitiveManager : public ResourceManager<PrimitiveInfo, Primitive>, public PrimitiveObservable
{
protected:
  Primitive* const create(const PrimitiveInfo& key);
  void destroy(Primitive* const value);

  // manages when a primitive is drawn...
  const Primitive* _lastDrawnPrimitive = nullptr;

public:
  PrimitiveManager();
  virtual ~PrimitiveManager() { clear(); }
  virtual void onShouldRender(const Primitive* p) override;
  virtual void update(float deltaT);
};