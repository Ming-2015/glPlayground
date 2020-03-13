#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// TextureInfo implementation
TextureInfo::TextureInfo()
  : _mTexPath(), _mGenerateMipMap(false)
{}

TextureInfo::TextureInfo(const std::string& texPath, bool generateMipMap)
  : _mTexPath(texPath), _mGenerateMipMap(generateMipMap)
{}

TextureInfo::TextureInfo(const TextureInfo& other)
  : _mTexPath(other._mTexPath), _mGenerateMipMap(other._mGenerateMipMap)
{}

bool TextureInfo::operator< (const TextureInfo & other) const
{
  if (_mTexPath != other._mTexPath)
  {
    return _mTexPath < other._mTexPath;
  }
  return _mGenerateMipMap < other._mGenerateMipMap;
}

bool TextureInfo::operator== (const TextureInfo & other) const
{
  if (_mTexPath == other._mTexPath
    && _mGenerateMipMap == other._mGenerateMipMap
  )
  {
    return true;
  }
  return false;
}

bool TextureInfo::isValidForCreation() const
{
  return !_mTexPath.empty();
}

const std::string TextureInfo::toString() const
{
  return "tex path = " + _mTexPath + 
    ", genMipMap = " + (_mGenerateMipMap ? "true" : "false");
}

// texture implementation
Texture::Texture()
{

}

Texture::~Texture()
{
  if (_mIsLoaded)
  {
    glDeleteTextures(1, &_mId);
  }
}

bool Texture::loadFromFile(std::string path, bool generateMipmap)
{
  if (_mIsLoaded)
  {
    Log.print<Severity::error>(
      "Already loaded a texture previously: ", _mPath, 
      ". Trying to load ", path, " to same texture object."
    );
    throw std::runtime_error("Double loading texture");
  }

  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(
    path.c_str(), 
    &_mWidth, 
    &_mHeight, 
    &_mNrChannels, 
    0
  );

  Log.print<Severity::info>(
    "Loading an image from \"", path, "\" with ", 
    _mNrChannels, " channels."
  );

  if (!data)
  {
    Log.print<Severity::warning>("Failed to load image: ", path);
    return false;
  }

  GLenum imageType = GL_RGB;
  if (_mNrChannels == 4)
  {
    imageType = GL_RGBA;
  }

  // generate texture
  glGenTextures(1, &_mId);
  glBindTexture(GL_TEXTURE_2D, _mId);

  // set the texture wrapping/filtering options... default options for now
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  if (generateMipmap)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }
  else
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // buffer the image into gl
  glTexImage2D(
    GL_TEXTURE_2D,      // buffer image to a 2D texture 
    0,                  // mipmap level, if this is a pre-generated mipmap
    GL_RGB,             // save as RGB format
    _mWidth,              // width of image
    _mHeight,             // height of image
    0,                  // always 0 (legacy stuff)
    imageType,          // input data format
    GL_UNSIGNED_BYTE,   // data primitive type
    data                // data buffer
  );

  // generate mipmap
  if (generateMipmap) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  // de-alloc data
  stbi_image_free(data);

  // remember the settings
  _mIsLoaded = true;
  _mPath = path;
  _mUseMipMap = generateMipmap;

  return true;
}

glm::ivec2 Texture::getDimension() const
{
  return glm::ivec2(_mWidth, _mHeight);
}

void Texture::bind(GLenum activeTarget) const 
{
  glActiveTexture(GL_TEXTURE0 + activeTarget);
  glBindTexture(GL_TEXTURE_2D, _mId);
}

// texture manager implementation
TextureManager::TextureManager()
{}

Texture* const TextureManager::create(const TextureInfo& key)
{
  if (!key.isValidForCreation())
  {
    throw std::runtime_error("Attempting to load invalid texture...");
  }

  Texture* tex = new Texture();
  bool success = tex->loadFromFile(key.getTexPath(), key.shouldGenMipMap());
  if (!success)
  {
    delete tex;
    throw std::runtime_error("Failed to load texture: " + key.getTexPath());
  }

  return tex;
}

void TextureManager::destroy(Texture* const value)
{
  delete value;
}