#include "Mesh.h"

int Mesh::objectCount = 0;

Mesh::Mesh()
  : _mUniqueId(++objectCount),
  _mVerticesVbo(0), 
  _mNormalsVbo(0), 
  _mTexVbo(0), 
  _mIndicesEbo(0),
  _mObjectVao(0),
  vertices(),
  normals(),
  texCoords(),
  indices(),
  renderWireMesh(false)
{

}

Mesh::~Mesh() 
{

}

void Mesh::initArrayObject() 
{
  glGenVertexArrays(1, &_mObjectVao);
  glBindVertexArray(_mObjectVao);

  if (vertices.size() > 0) 
  {
    // generate buffer object
    glGenBuffers(1, &_mVerticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mVerticesVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(float) * vertices.size(),
      static_cast<const void*>(vertices.data()),
      GL_STATIC_DRAW
    );
    // define position vertex attribute
    glVertexAttribPointer(
      Mesh::ATTRIBUTE_POSITION,  // attribute layout
      Mesh::SIZE_POSITION,       // size of attribute
      GL_FLOAT,                        // type of attribute value
      GL_FALSE,                        // normalize data?
      Mesh::SIZE_POSITION * sizeof(float),  // data stride
      (void*)0                         // data offset
    );
    glEnableVertexAttribArray(Mesh::ATTRIBUTE_POSITION);
  }

  if (normals.size() > 0) 
  {
    // generate buffer object
    glGenBuffers(1, &_mNormalsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mNormalsVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(float) * normals.size(),
      static_cast<const void*>(normals.data()),
      GL_STATIC_DRAW
    );
    // define normal vertex attribute
    glVertexAttribPointer(
      Mesh::ATTRIBUTE_NORMAL,
      Mesh::SIZE_NORMAL,
      GL_FLOAT,
      GL_FALSE,
      Mesh::SIZE_NORMAL * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(Mesh::ATTRIBUTE_NORMAL);
  }

  if (texCoords.size() > 0) 
  {
    // generate buffer object
    glGenBuffers(1, &_mTexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mTexVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(float) * texCoords.size(),
      static_cast<const void*>(texCoords.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Mesh::ATTRIBUTE_TEX,
      Mesh::SIZE_TEX,
      GL_FLOAT,
      GL_FALSE,
      Mesh::SIZE_TEX * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(Mesh::ATTRIBUTE_TEX);
  }

  if (indices.size() > 0)
  {
    glGenBuffers(1, &_mIndicesEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mIndicesEbo);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(unsigned int) * indices.size(),
      static_cast<const void*>(indices.data()),
      GL_STATIC_DRAW
    );
  }

  glBindVertexArray(0);
}

void Mesh::render() const
{
  if (!_mObjectVao)
  {
    Log.print<Severity::warning>("Mesh ", _mUniqueId, "'s VAO is not initialized yet!");
    return;
  }

  glBindVertexArray(_mObjectVao);

  if (renderWireMesh) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (_mIndicesEbo) glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  else if (_mVerticesVbo) glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
  else {
    Log.print<Severity::warning>("Mesh ", _mUniqueId, " does not have vertices!");
    return;
  }

  glBindVertexArray(0);
}