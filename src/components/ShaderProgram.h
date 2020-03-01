#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include "ShaderManager.h"

class ShaderProgram
{
private:
  unsigned int _mShaderProgram;
  bool _mIsLoaded;

public:

  ShaderProgram();
  virtual ~ShaderProgram();

  void initShaderProgram(
    const Shader& vertexShader,
    const Shader& fragmentShader
  );
  void deleteShaderProgram();

  unsigned int getShaderProgramId() const;
  bool isLoaded() const;

  void useProgram() const;
};