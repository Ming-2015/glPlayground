#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/scene.h>
#include "../utils/ResourceManager.hpp"
#include "../utils/Logger.h"

// information needed to initialize a texture
class TextureData
{
public:
  enum class TextureDataType {
    path,
    assimpBuffer
  };

  TextureDataType type = TextureDataType::path;

  // for texture type buffer
  const aiTexture* assimpTexture = nullptr;

  // for texture type path
  std::string texPath;

  // generate mip map?
  bool generateMipMap;

public:
  TextureData(const std::string& texPath, bool generateMipMap = true);
  TextureData(const aiTexture* tex, bool generateMipMip = true);

  TextureData(const TextureData& other) = default;
  ~TextureData() = default;
};


// internal implementation of texture
class Texture
{
protected:

  int _mWidth = 0;
  int _mHeight = 0;
  int _mNrChannels = 0;
  unsigned int _mId = 0;
  bool _mIsLoaded = false;

  std::string _mPath;
  bool _mUseMipMap = false;

  bool processStbiData(unsigned char* data);

public:

  Texture();
  virtual ~Texture();
  virtual bool loadFromFile(std::string path, bool generateMipmap = false);
  virtual bool loadFromAssimpTexture(const aiTexture* tex, bool generateMipMap = false);

  // getters
  glm::ivec2 getDimension() const;
  unsigned int getId() const { return _mId; }

  // bind texture to an active target
  virtual void bind(GLenum activeTarget) const;
};


// manages creation and deletion of textures
class TextureManager : public ResourceManager<std::string, TextureData, Texture>
{
protected:
  Texture* const create(const std::string& key, const TextureData& data) override;
  void destroy(Texture* const value) override;

public:
  TextureManager();
  virtual ~TextureManager() { clear(); }
};