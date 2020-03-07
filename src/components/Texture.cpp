#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{

}

Texture::~Texture()
{

}

bool Texture::loadFromFile(std::string path, bool generateMipmap)
{
  unsigned char* data = stbi_load(
    path.c_str(), 
    &_mWidth, 
    &_mHeight, 
    &_mNrChannels, 
    0
  );

  if (!data)
  {
    Log.print<Severity::warning>("Failed to load image: ", path);
    return false;
  }

  // generate texture
  glGenTextures(1, &_mId);
  glBindTexture(GL_TEXTURE_2D, _mId);
  glTexImage2D(
    GL_TEXTURE_2D,      // buffer image to a 2D texture 
    0,                  // mipmap level, if this is a pre-generated mipmap
    GL_RGB,             // save as RGB format
    _mWidth,              // width of image
    _mHeight,             // height of image
    0,                  // always 0 (legacy stuff)
    GL_RGB,             // data format of input image
    GL_UNSIGNED_BYTE,   // data primitive type
    data                // data buffer
  );

  if (generateMipmap) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  // de-alloc data
  stbi_image_free(data);

  return true;
}

glm::ivec2 Texture::getDimension() const
{
  return glm::ivec2(_mWidth, _mHeight);
}