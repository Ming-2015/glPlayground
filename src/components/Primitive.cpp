#include "Primitive.h"

// Primitive implementation
int Primitive::objectCount = 0;
Primitive::Primitive()
  : _mUniqueId(++objectCount)
{}

Primitive::~Primitive() 
{}

void Primitive::initArrayObject(const PrimitiveData* data)
{
  // assuming all triangles
  numVertices = data->vertices.size() / SIZE_POSITION;
  numTexCoords = data->texCoords.size() / data->numComponents;
  numTexCoords_2 = data->texCoords_2.size() / data->numComponents_2;
  numTexCoords_3 = data->texCoords_3.size() / data->numComponents_3;
  numNormals = data->normals.size() / SIZE_NORMAL;
  numTangents = data->tangents.size() / SIZE_TANGENT;
  numBitangents = data->bitangents.size() / SIZE_BITANGENT;
  numFaces = data->indices.size() / SIZE_FACE;
  numWeights = data->weights.size() / SIZE_WEIGHT;
  numJoints = data->joints.size() / SIZE_JOINT;

  if (numVertices == 0) 
  {
    Log.print<Severity::warning>("Trying to initialize a VAO with no vertices!");
  }

  if (numTexCoords > 0 && numVertices != numTexCoords)
  {
    Log.print<Severity::warning>("Trying to initialize tex coords with different size!");
  }

  if (numTexCoords_2 > 0 && numVertices != numTexCoords_2) 
  {
    Log.print<Severity::warning>("Trying to initialize tex coords 2 with different size!");
  }

  if (numTexCoords_3 > 0 && numVertices != numTexCoords_3)
  {
    Log.print<Severity::warning>("Trying to initialize tex coords 3 with different size!");
  }

  if (numNormals > 0 && numVertices != numNormals) 
  {
    Log.print<Severity::warning>("trying to initialize normals with different size!");
  }

  if (numTangents > 0 && numVertices != numTangents) 
  {
    Log.print<Severity::warning>("Trying to initialize tangents with different size!");
  }

  if (numBitangents > 0 && numVertices != numBitangents)
  {
    Log.print<Severity::warning>("Trying to initialize bitangents with different size!");
  }

  if (numWeights > 0 && numVertices != numWeights)
  {
    Log.print<Severity::warning>("Trying to initialize bone weights with different size!");
  }

  if (numJoints > 0 && numVertices != numJoints)
  {
    Log.print<Severity::warning>("Trying to initialize bone joints with different size!");
  }

  if (numFaces == 0) 
  {
    Log.print<Severity::warning>("Trying to initialize a mesh without faces - using drawArrays by default!");
  }

  glGenVertexArrays(1, &_mObjectVao);
  glBindVertexArray(_mObjectVao);
  _mHasObjectVao = true;

  if (numVertices) 
  {
    // generate buffer object
    glGenBuffers(1, &_mVerticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mVerticesVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->vertices[0]) * data->vertices.size(),
      static_cast<const void*>(data->vertices.data()),
      GL_STATIC_DRAW
    );

    // define position vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_POSITION,  
      Primitive::SIZE_POSITION,      
      GL_FLOAT,                       
      GL_FALSE,                        
      Primitive::SIZE_POSITION * sizeof(data->vertices[0]),  
      (void*)0                         
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_POSITION);

    _mHasVerticesVbo = true;
  }

  if (numNormals) 
  {
    // generate buffer object
    glGenBuffers(1, &_mNormalsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mNormalsVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->normals[0]) * data->normals.size(),
      static_cast<const void*>(data->normals.data()),
      GL_STATIC_DRAW
    );
    // define normal vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_NORMAL,
      Primitive::SIZE_NORMAL,
      GL_FLOAT,
      GL_FALSE,
      Primitive::SIZE_NORMAL * sizeof(data->normals[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_NORMAL);

    _mHasNormalsVbo = true;
  }

  if (numTexCoords) 
  {
    // generate buffer object
    glGenBuffers(1, &_mTexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mTexVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->texCoords[0]) * data->texCoords.size(),
      static_cast<const void*>(data->texCoords.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_TEX,
      data->numComponents,
      GL_FLOAT,
      GL_FALSE,
      data->numComponents * sizeof(data->texCoords[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_TEX);

    _mHasTexVbo = true;
  }

  if (numTexCoords_2)
  {
    // generate buffer object
    glGenBuffers(1, &_mTexVbo_2);
    glBindBuffer(GL_ARRAY_BUFFER, _mTexVbo_2);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->texCoords_2[0]) * data->texCoords_2.size(),
      static_cast<const void*>(data->texCoords_2.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_TEX_2,
      data->numComponents_2,
      GL_FLOAT,
      GL_FALSE,
      data->numComponents_2 * sizeof(data->texCoords_2[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_TEX_2);

    _mHasTexVbo_2 = true;
  }

  if (numTexCoords_3)
  {
    // generate buffer object
    glGenBuffers(1, &_mTexVbo_3);
    glBindBuffer(GL_ARRAY_BUFFER, _mTexVbo_3);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->texCoords_3[0]) * data->texCoords_3.size(),
      static_cast<const void*>(data->texCoords_3.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_TEX_3,
      data->numComponents_3,
      GL_FLOAT,
      GL_FALSE,
      data->numComponents_3 * sizeof(data->texCoords_3[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_TEX_3);

    _mHasTexVbo_3 = true;
  }

  if (numTangents)
  {
    // generate buffer object
    glGenBuffers(1, &_mTangentsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mTangentsVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->tangents[0])* data->tangents.size(),
      static_cast<const void*>(data->tangents.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_TANGENT,
      Primitive::SIZE_TANGENT,
      GL_FLOAT,
      GL_FALSE,
      Primitive::SIZE_TANGENT * sizeof(data->tangents[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_TANGENT);

    _mHasTangentsVbo = true;
  }

  if (numBitangents)
  {
    // generate buffer object
    glGenBuffers(1, &_mBitangentsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mBitangentsVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->bitangents[0]) * data->bitangents.size(),
      static_cast<const void*>(data->bitangents.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_BITANGENT,
      Primitive::SIZE_BITANGENT,
      GL_FLOAT,
      GL_FALSE,
      Primitive::SIZE_BITANGENT * sizeof(data->bitangents[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_BITANGENT);

    _mHasBitangentsVbo = true;
  }

  if (numWeights)
  {
    // generate buffer object
    glGenBuffers(1, &_mWeightsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mWeightsVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->weights[0])* data->weights.size(),
      static_cast<const void*>(data->weights.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_WEIGHT,
      Primitive::SIZE_WEIGHT,
      GL_FLOAT,
      GL_FALSE,
      Primitive::SIZE_WEIGHT * sizeof(data->weights[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_WEIGHT);

    _mHasWeightsVbo = true;
  }

  if (numJoints)
  {
    // generate buffer object
    glGenBuffers(1, &_mJointsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mJointsVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(data->joints[0])* data->joints.size(),
      static_cast<const void*>(data->joints.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Primitive::ATTRIBUTE_JOINT,
      Primitive::SIZE_JOINT,
      GL_UNSIGNED_INT,
      GL_FALSE,
      Primitive::SIZE_JOINT * sizeof(data->joints[0]),
      (void*)0
    );
    glEnableVertexAttribArray(Primitive::ATTRIBUTE_JOINT);

    _mHasJointsVbo = true;
  }

  if (_mHasJointsVbo != _mHasWeightsVbo)
  {
    Log.print<Severity::warning>("Only has one of weights and joints!");
  }

  if (numFaces)
  {
    glGenBuffers(1, &_mIndicesEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mIndicesEbo);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(data->indices[0]) * data->indices.size(),
      static_cast<const void*>(data->indices.data()),
      GL_STATIC_DRAW
    );

    _mHasIndicesEbo = true;
  }

  glBindVertexArray(0);
}

void Primitive::bindVao() const
{
  if (!_mHasObjectVao)
  {
    Log.print<Severity::warning>("Mesh ", _mUniqueId, "'s VAO is not initialized yet!");
    return;
  }

  glBindVertexArray(_mObjectVao);
}

void Primitive::addObservable(PrimitiveObservable* o)
{
  observers.insert(o);
}

void Primitive::removeObservable(PrimitiveObservable* o)
{
  observers.erase(o);
}

void Primitive::render() const
{
  for (auto observer : observers)
  {
    observer->onShouldRender(this);
  }

  if (_mHasIndicesEbo) 
  {
    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
  }
  else if (_mHasVerticesVbo) 
  {
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
  }
  else 
  {
    Log.print<Severity::warning>("Mesh ", _mUniqueId, " does not have vertices!");
  }

  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR)
  {
    Log.print<Severity::warning>("Encountered a GL error: ", err);
    std::string error;
    switch (err)
    {
    case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
    case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
    case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
    case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
    case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
    case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }
    //std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    Log.print<Severity::warning>("Error summary: ", error);
  }
}

void Primitive::deleteArrayObject()
{
  if (_mHasVerticesVbo)
  {
    glDeleteBuffers(1, &_mVerticesVbo);
    _mHasVerticesVbo = false;
  }

  if (_mHasNormalsVbo)
  {
    glDeleteBuffers(1, &_mNormalsVbo);
    _mHasNormalsVbo = false;
  }

  if (_mHasTangentsVbo)
  {
    glDeleteBuffers(1, &_mTangentsVbo);
    _mHasTangentsVbo = false;
  }

  if (_mHasBitangentsVbo)
  {
    glDeleteBuffers(1, &_mBitangentsVbo);
    _mHasBitangentsVbo = false;
  }

  if (_mHasTexVbo)
  {
    glDeleteBuffers(1, &_mTexVbo);
    _mTexVbo = false;
  }

  if (_mHasTexVbo_2)
  {
    glDeleteBuffers(1, &_mTexVbo_2);
    _mTexVbo_2 = false;
  }

  if (_mHasTexVbo_3)
  {
    glDeleteBuffers(1, &_mTexVbo_3);
    _mTexVbo_3 = false;
  }

  if (_mHasWeightsVbo)
  {
    glDeleteBuffers(1, &_mWeightsVbo);
    _mWeightsVbo = false;
  }

  if (_mHasJointsVbo)
  {
    glDeleteBuffers(1, &_mJointsVbo);
    _mJointsVbo = false;
  }

  if (_mHasIndicesEbo)
  {
    glDeleteBuffers(1, &_mIndicesEbo);
    _mIndicesEbo = false;
  }

  if (_mHasObjectVao)
  {
    glDeleteVertexArrays(1, &_mObjectVao);
    _mHasObjectVao = false;
  }
}

Primitive* const PrimitiveManager::create(const std::string& key, const PrimitiveData& data)
{
  Primitive* p = new Primitive();
  p->initArrayObject(&data);
  p->addObservable(this);
  return p;
}

void PrimitiveManager::destroy(Primitive* const value)
{
  value->deleteArrayObject();
  delete value;
}

PrimitiveManager::PrimitiveManager()
{}

void PrimitiveManager::onShouldRender(const Primitive* d)
{
  if (_lastDrawnPrimitive != d)
  {
    _lastDrawnPrimitive = d;
    _lastDrawnPrimitive->bindVao();
  }
}

void PrimitiveManager::update(float deltaT)
{
  // resets the last drawn primitive to prevent errors
  _lastDrawnPrimitive = nullptr;
}