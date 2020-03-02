#pragma once
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

#include "../utils/ResourceManager.h"

class ShaderInfo : ResourceInfo<ShaderInfo> {
protected:
  friend class Shader;
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

class Shader {
protected:
  friend class ShaderManager;
  ShaderInfo _mShaderInfo;
  unsigned int _mShaderId;
  bool _mIsShaderLoaded;

  void load(const ShaderInfo& info);
  void deleteShader();

public:
  Shader();
  virtual ~Shader();

  unsigned int getShaderId() const;
  bool isLoaded() const;
  const ShaderInfo& getShaderInfo() const;
};

// ShaderManager is an encapsulation of Shader
class ShaderManager : public ResourceManager<ShaderInfo, Shader>
{
protected:
  Shader* const create(const ShaderInfo& key);
  void destroy(Shader* const value);

public:
  ShaderManager();
  virtual ~ShaderManager() { clear(); }
};