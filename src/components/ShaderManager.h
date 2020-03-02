#pragma once
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

#include "../utils/ResourceManager.hpp"
#include "../utils/Logger.h"

// Shader - has to be instantiated by ShaderManager
class Shader {
protected:
  friend class ShaderManager;

  unsigned int _mShaderId;
  bool _mIsShaderLoaded;
  std::string _mPath;
  GLenum _mType;

  Shader();
  virtual ~Shader();

  void load(const std::string& shaderPath, const GLenum& shaderType);
  void deleteShader();

public:

  unsigned int getShaderId() const { return _mShaderId; }
  bool isLoaded() const { return _mShaderId; }
  const std::string& getShaderPath() const { return _mPath; }
  const GLenum& getShaderType() const { return _mType; }
};

// Shader Info - everything necessary to instantiate a Shader
class ShaderInfo : ResourceInfo<ShaderInfo> {
protected:
  friend class ShaderManager;
  std::string shaderPath;
  GLenum shaderType;

public:
  ShaderInfo() : shaderPath(""), shaderType(0) {}
  ShaderInfo(std::string path, GLenum type) : shaderPath(path), shaderType(type) {}
  ShaderInfo(const ShaderInfo& other) = default;
  virtual ~ShaderInfo() = default;

  bool operator< (const ShaderInfo& other) const;
  bool operator== (const ShaderInfo& other) const;
  bool isValid() const;

  const std::string toString() const;
};

// ShaderManager is an encapsulation of Shader - allows the creation and lookup of shader
class ShaderManager : public ResourceManager<ShaderInfo, Shader>
{
protected:
  Shader* const create(const ShaderInfo& key);
  void destroy(Shader* const value);

public:
  ShaderManager();
  virtual ~ShaderManager() { clear(); }
};