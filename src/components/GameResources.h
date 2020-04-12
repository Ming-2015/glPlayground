#pragma once
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Primitive.h"
#include "Window.h"

struct GameResources {
  ShaderManager& shaderManager;
  ShaderProgramManager& shaderProgramManager;
  TextureManager& textureManager;
  PrimitiveManager& primitiveManager;
  Window& window;

  GameResources(
    ShaderManager& shaderManager,
    ShaderProgramManager& shaderProgramManager,
    TextureManager& textureManager,
    PrimitiveManager& primitiveManager,
    Window& window
  ) : shaderManager(shaderManager),
    shaderProgramManager(shaderProgramManager),
    textureManager(textureManager),
    primitiveManager(primitiveManager),
    window(window)
  {}

  GameResources(const GameResources& other)
    : shaderManager(other.shaderManager),
    shaderProgramManager(other.shaderProgramManager),
    textureManager(other.textureManager),
    primitiveManager(other.primitiveManager),
    window(other.window)
  {}
};