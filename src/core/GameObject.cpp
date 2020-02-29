#include "GameObject.h"

int GameObject::objectCount = 0;

GameObject::GameObject()
  : VBO(0), _mUniqueId(++objectCount)
{

}

GameObject::~GameObject() 
{

}

void GameObject::initArrayBuffer() 
{
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(
    GL_ARRAY_BUFFER, 
    sizeof(float) * vertices.size(), 
    static_cast<const void*>(vertices.data()), 
    GL_STATIC_DRAW
  );

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}