#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../utils/Logger.h"

class Texture
{
protected:

  int _mWidth;
  int _mHeight;
  int _mNrChannels;
  unsigned int _mId;

public:

  Texture();
  virtual ~Texture();
  glm::ivec2 getDimension() const;
  bool loadFromFile(std::string path, bool generateMipmap=false);

  unsigned int getTextureHandler() const 
  {
    return _mId;
  }

  void bindTexture() const 
  {
    glBindTexture(GL_TEXTURE_2D, _mId);
  }
};