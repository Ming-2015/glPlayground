#include "GameObject.h"

int GameObject::objectCount = 0;

GameObject::GameObject()
  : _mUniqueId(++objectCount),
  _mVerticesVbo(0), 
  _mNormalsVbo(0), 
  _mTexVbo(0), 
  _mIndicesEbo(0),
  _mObjectVao(0),
  vertices(),
  normals(),
  texCoords(),
  indices()
{

}

GameObject::~GameObject() 
{

}

void GameObject::initArrayBuffer() 
{
  glGenVertexArrays(1, &_mObjectVao);
  glBindVertexArray(_mObjectVao);

  if (vertices.size() > 0) {
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
      GameObject::ATTRIBUTE_POSITION,  // attribute layout
      GameObject::SIZE_POSITION,       // size of attribute
      GL_FLOAT,                        // type of attribute value
      GL_FALSE,                        // normalize data?
      GameObject::SIZE_POSITION * sizeof(float),  // data stride
      (void*)0                         // data offset
    );
    glEnableVertexAttribArray(GameObject::ATTRIBUTE_POSITION);

    // unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  if (normals.size() > 0) {
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
      GameObject::ATTRIBUTE_NORMAL,
      GameObject::SIZE_NORMAL,
      GL_FLOAT,
      GL_FALSE,
      GameObject::SIZE_NORMAL * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(GameObject::ATTRIBUTE_NORMAL);

    // unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  if (texCoords.size() > 0) {
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
      GameObject::ATTRIBUTE_TEX,
      GameObject::SIZE_TEX,
      GL_FLOAT,
      GL_FALSE,
      GameObject::SIZE_TEX * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(GameObject::ATTRIBUTE_TEX);

    // unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  glBindVertexArray(0);
}

void GameObject::update(float deltaT)
{

}

void GameObject::render()
{
  if (!_mObjectVao)
  {
    std::cerr << "Game Object is not initialized yet!" << std::endl;
    return;
  }

  glBindVertexArray(_mObjectVao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}