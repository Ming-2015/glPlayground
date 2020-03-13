#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../utils/ResourceManager.hpp"
#include "../utils/Logger.h"

// information needed to initialize a texture
class TextureInfo : public ResourceInfo<TextureInfo>
{
protected:
  std::string _mTexPath;
  bool _mGenerateMipMap;

public:

  TextureInfo();
  TextureInfo(const std::string& texPath, bool generateMipMap);
  TextureInfo(const TextureInfo& other);
  ~TextureInfo() = default;

  bool operator< (const TextureInfo& other) const;
  bool operator== (const TextureInfo& other) const;
  bool isValidForCreation() const;

  const std::string toString() const;
  const std::string& getTexPath() const { return _mTexPath; }
  const bool shouldGenMipMap() const { return _mGenerateMipMap; }
};


// internal implementation of texture
class Texture
{
  friend class TextureManager;

protected:

  int _mWidth = 0;
  int _mHeight = 0;
  int _mNrChannels = 0;
  unsigned int _mId = 0;
  bool _mIsLoaded = false;

  std::string _mPath;
  bool _mUseMipMap = false;

  virtual bool loadFromFile(std::string path, bool generateMipmap = false);

public:

  Texture();
  virtual ~Texture();
  glm::ivec2 getDimension() const;
  unsigned int getId() const { return _mId; }

  // bind texture to an active target
  virtual void bind(GLenum activeTarget) const;
};


// manages creation and deletion of textures
class TextureManager : public ResourceManager<TextureInfo, Texture>
{
protected:
  Texture* const create(const TextureInfo& key);
  void destroy(Texture* const value);

public:
  TextureManager();
  virtual ~TextureManager() { clear(); }
};