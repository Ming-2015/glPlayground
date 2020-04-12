#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "../utils/ResourceManager.hpp"
#include "Uniform.h"

// Contains all the info needed to initialize a shader program
class ShaderProgramInfo : ResourceInfo<ShaderProgramInfo>
{
public:
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
  bool isValidForCreation() const;
  const std::string toString() const;

  bool hasGeometryShader() const;
};

// a wrapper around an opengl shader program
// TODO: update it to support Shader Program Pipeline
class ShaderProgram
{
protected:
  unsigned int _mId;
  bool _mIsLoaded;

  // NOTE: contains a copy of each uniform created, in the order the uniforms are queried
  std::vector<Uniform*> _mUniforms;

  // NOTE: map may contain multiple same uniforms with different names
  std::map<std::string, Uniform*> _mUniformMap;

  // called to initiate uniforms
  void parseProgramInfo();

public:

  ShaderProgram();
  virtual ~ShaderProgram();

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
  
  // SHOULD ONLY BE CALLED BY ShaderProgramManager
  void use() const;

  // get uniform by its name
  Uniform* getUniformByName(const std::string& name) const;
  
  // get uniform by its index - arrays only take 1 space in the vector!
  Uniform* getUniformByIndex(unsigned int index) const;

  unsigned int getShaderProgramId() const;
  bool isLoaded() const;
};

// encapsulate and manages shader program instances
class ShaderProgramManager : public ResourceManager<ShaderProgramInfo, ShaderProgram>
{
private:
  ShaderManager& _mShaderManager;
  ShaderProgram* _mProgramInUse;

protected:
  ShaderProgram* const create(const ShaderProgramInfo& key);
  void destroy(ShaderProgram* const value);

public:
  ShaderProgramManager(ShaderManager& shaderManager);
  virtual ~ShaderProgramManager() { clear(); }

  void useProgram(ShaderProgram* program);
  ShaderProgram* getProgramInUse() const;
};