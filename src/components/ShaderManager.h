#pragma once
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

#include "../utils/ResourceManager.h"

struct ShaderInfo {
  std::string shaderPath;
  GLenum shaderType;

  ShaderInfo(const ShaderInfo& other) = default;

  bool operator< (const ShaderInfo& other) const {
    if (shaderType != other.shaderType)
    {
      return shaderType < other.shaderType;
    }
    else
    {
      return shaderPath < other.shaderPath;
    }
  }

  bool operator== (const ShaderInfo& other) const {
    return shaderPath == other.shaderPath && shaderType == other.shaderType;
  }
};

class Shader {
private:
  friend class ShaderManager;

  ShaderInfo _mShaderInfo;
  unsigned int _mShaderId;
  bool _mIsShaderLoaded;

  void loadFromFile(const ShaderInfo& info);
  void deleteShader();

public:

  Shader();
  virtual ~Shader();

  unsigned int getShaderId() const;
  bool isShaderLoaded() const;
  const ShaderInfo& getShaderInfo() const;
};

// ShaderManager is an encapsulation of Shader
class ShaderManager : ResourceManager<ShaderInfo, Shader>
{
private:

public:
  ShaderManager();
  virtual ~ShaderManager();

  ShaderManager::Shader& getOrCreate(const ShaderInfo& key);
  void remove(const ShaderInfo& key);
  void removeAll();
};