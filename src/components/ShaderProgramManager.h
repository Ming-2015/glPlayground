#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "ShaderManager.h"
#include "../utils/ResourceManager.hpp"

// Contains all the info needed to initialize a shader program
class ShaderProgramInfo : ResourceInfo<ShaderProgramInfo>
{
protected:
  friend class ShaderProgramManager;
  ShaderInfo vertexShaderInfo;
  ShaderInfo fragmentShaderInfo;
  ShaderInfo geometryShaderInfo;

public:
  ShaderProgramInfo();
  ShaderProgramInfo(
    const ShaderInfo& pVertexShaderInfo,
    const ShaderInfo& pFragmentShader
  );
  ShaderProgramInfo(
    const ShaderInfo& pVertexShader,
    const ShaderInfo& pFragmentShader,
    const ShaderInfo& pGeometryShader
  );
  ShaderProgramInfo(const ShaderProgramInfo& other) = default;

  bool operator< (const ShaderProgramInfo& other) const;
  bool operator== (const ShaderProgramInfo& other) const;
  bool isValid() const;
  const std::string toString() const;

  bool hasGeometryShader() const;
};

// a wrapper around an opengl shader program
// TODO: update it to support Shader Program Pipeline
class ShaderProgram
{
private:
  friend class ShaderProgramManager;
  ShaderProgramInfo _mShaderProgramInfo;
  unsigned int _mShaderProgramId;
  bool _mIsLoaded;

  void initShaderProgram(
    const Shader& vertexShader,
    const Shader& fragmentShader
  );

  void initShaderProgram(
    const Shader& vertexShader,
    const Shader& fragmentShader,
    const Shader& geometryShader
  );

  void deleteShaderProgram();

public:

  ShaderProgram();
  virtual ~ShaderProgram();

  unsigned int getShaderProgramId() const;
  bool isLoaded() const;
  void useProgram() const;
  const ShaderProgramInfo& getShaderProgramInfo() const;
};

// encapsulate and manages shader program instances
class ShaderProgramManager : public ResourceManager<ShaderProgramInfo, ShaderProgram>
{
private:
  ShaderManager& _mShaderManager;
protected:
  ShaderProgram* const create(const ShaderProgramInfo& key);
  void destroy(ShaderProgram* const value);
public:
  ShaderProgramManager(ShaderManager& shaderManager);
  virtual ~ShaderProgramManager() { clear(); }
};