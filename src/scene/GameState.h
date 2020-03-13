#pragma once
#include "../components/Shader.h"
#include "../components/ShaderProgram.h"
#include "../components/Texture.h"
#include "../components/Primitive.h"

class GameState
{
protected:

  ShaderManager& _mShaderManager;
  ShaderProgramManager& _mShaderProgramManager;
  TextureManager& _mTextureManager;
  PrimitiveManager& _mPrimitiveManager;

public:

  GameState(
    ShaderManager& shaderManager,
    ShaderProgramManager& shaderProgramManager,
    TextureManager& textureManager,
    PrimitiveManager& primitiveManager
  ) : _mShaderManager(shaderManager),
    _mShaderProgramManager(shaderProgramManager),
    _mTextureManager(textureManager),
    _mPrimitiveManager(primitiveManager)
  {}

  virtual ~GameState() {}
};