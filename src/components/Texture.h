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

  // bind texture to an active target
  void bind(GLenum activeTarget) const 
  {
    glActiveTexture(GL_TEXTURE0 + activeTarget);
    glBindTexture(GL_TEXTURE_2D, _mId);
  }
};