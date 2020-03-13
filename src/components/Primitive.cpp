#include "Primitive.h"

// PrimitiveInfo implementation
PrimitiveInfo::PrimitiveInfo(const std::string& id, PrimitiveData* data)
  : id(id), data(data)
{}

const std::string PrimitiveInfo::toString() const
{
  return id;
}

bool PrimitiveInfo::operator< (const PrimitiveInfo& other) const
{
  return id < other.id;
}

bool PrimitiveInfo::operator== (const PrimitiveInfo& other) const
{
  return id == other.id;
}

bool PrimitiveInfo::isValidForCreation() const
{
  return !id.empty() && data != nullptr;
}

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
  numTexCoords = data->texCoords.size() / SIZE_TEX;
  numTexCoords_2 = data->texCoords_2.size() / SIZE_TEX;
  numTexCoords_3 = data->texCoords_3.size() / SIZE_TEX;
  numNormals = data->normals.size() / SIZE_NORMAL;
  numTangents = data->tangents.size() / SIZE_TANGENT;
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
      Primitive::SIZE_TEX,
      GL_FLOAT,
      GL_FALSE,
      Primitive::SIZE_TEX * sizeof(data->texCoords[0]),
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
      Primitive::SIZE_TEX,
      GL_FLOAT,
      GL_FALSE,
      Primitive::SIZE_TEX * sizeof(data->texCoords_2[0]),
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
      Primitive::SIZE_TEX,
      GL_FLOAT,
      GL_FALSE,
      Primitive::SIZE_TEX * sizeof(data->texCoords_3[0]),
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

void Primitive::render() const
{
  if (!_mHasObjectVao)
  {
    Log.print<Severity::warning>("Mesh ", _mUniqueId, "'s VAO is not initialized yet!");
    return;
  }

  glBindVertexArray(_mObjectVao);

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
  }

  glBindVertexArray(0);
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
    _mTangentsVbo = false;
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

Primitive* const PrimitiveManager::create(const PrimitiveInfo& key)
{
  if (!key.data)
  {
    Log.print<Severity::error>("Trying to create a primitive without data");
    throw std::runtime_error("Failed to create primitive with no data!");
  }

  Primitive* p = new Primitive();
  p->initArrayObject(key.data);
  return p;
}

void PrimitiveManager::destroy(Primitive* const value)
{
  value->deleteArrayObject();
  delete value;
}

PrimitiveManager::PrimitiveManager()
{}