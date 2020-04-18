#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureData::TextureData(const std::string& texPath, bool generateMipMap)
  : type(TextureDataType::path), texPath(texPath), generateMipMap(generateMipMap)
{}

TextureData::TextureData(const aiTexture* tex, bool generateMipMip)
  : type(TextureDataType::assimpBuffer), assimpTexture(tex), generateMipMap(generateMipMip)
{}

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

bool Texture::processStbiData(unsigned char* data)
{
  if (!data)
  {
    Log.print<Severity::warning>("Failed to load image: ", _mPath);
    return false;
  }

  GLenum imageType = GL_RGB;
  if (_mNrChannels == 4)
    imageType = GL_RGBA;

  // generate texture
  glCreateTextures(GL_TEXTURE_2D, 1, &_mId);

  // set the texture wrapping/filtering options... default options for now
  glTextureParameteri(_mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(_mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
  if (_mUseMipMap)
  {
    glTextureParameteri(_mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }
  else
  {
    glTextureParameteri(_mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  glTextureParameteri(_mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTextureStorage2D(_mId, 1, GL_RGBA8, _mWidth, _mHeight);
  glTextureSubImage2D(_mId, 0, 0, 0, _mWidth, _mHeight, imageType, GL_UNSIGNED_BYTE, data);

  // generate mipmap
  if (_mUseMipMap) 
    glGenerateTextureMipmap(_mId);

  // de-alloc data
  stbi_image_free(data);

  // remember the settings
  _mIsLoaded = true;
  return true;
}

bool Texture::loadFromAssimpTexture(const aiTexture* tex, bool generateMipMap)
{
  if (!tex)
  {
    Log.print<Severity::error>("trying to load texture from null pointer");
    throw std::runtime_error("Invalid texture pointer");
  }

  if (_mIsLoaded)
  {
    Log.print<Severity::error>(
      "Already loaded a texture previously: ", _mPath,
      ". Trying to load ", tex->mFilename.C_Str(), " to same texture object."
      );
    throw std::runtime_error("Double loading texture");
  }

  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = nullptr;
  if (tex->mHeight == 0)
  {
    data = stbi_load_from_memory(
      reinterpret_cast<unsigned char*>(tex->pcData),
      tex->mWidth,
      &_mWidth,
      &_mHeight,
      &_mNrChannels,
      0
    );
  }
  else
  {
    data = stbi_load_from_memory(
      reinterpret_cast<unsigned char*>(tex->pcData),
      tex->mWidth * tex->mHeight * 4,
      &_mWidth,
      &_mHeight,
      &_mNrChannels,
      0
    );
  }

  _mPath = tex->mFilename.C_Str();
  _mUseMipMap = generateMipMap;
  return processStbiData(data);
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

  _mPath = path;
  _mUseMipMap = generateMipmap;

  return processStbiData(data);
}

glm::ivec2 Texture::getDimension() const
{
  return glm::ivec2(_mWidth, _mHeight);
}

void Texture::bind(GLenum activeTarget) const 
{
  glBindTextureUnit(activeTarget, _mId);
  //glActiveTexture(GL_TEXTURE0 + activeTarget);
  //glBindTexture(GL_TEXTURE_2D, _mId);
}

// texture manager implementation
TextureManager::TextureManager()
{}

Texture* const TextureManager::create(const std::string& key, const TextureData& data)
{
  Texture* tex = new Texture();
  bool success = false;
  
  if (data.type == TextureData::TextureDataType::path) 
  {
    success = tex->loadFromFile(data.texPath, data.generateMipMap);
    Log.print<Severity::debug>("Loading file texture from: ", data.texPath);
  }

  if (data.type == TextureData::TextureDataType::assimpBuffer)
  {
    success = tex->loadFromAssimpTexture(data.assimpTexture, data.generateMipMap);
    Log.print<Severity::debug>("Loading assimp texture");
  }
  
  if (!success)
  {
    delete tex;
    throw std::runtime_error("Failed to load texture: " + data.texPath);
  }

  return tex;
}

void TextureManager::destroy(Texture* const value)
{
  delete value;
}