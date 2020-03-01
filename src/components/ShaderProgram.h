#pragma once
#include <string>
#include <glad/glad.h>
#include <iostream>
#include "Shader.h"

class ShaderProgram
{
private:
  unsigned int _mShaderProgram;
  bool _mIsLoaded;

public:

  ShaderProgram();
  virtual ~ShaderProgram();

  void initShaderProgram(
    std::string vertexShaderPath,
    std::string fragmentShaderPath,
    std::string geometryShaderPath = ""
  );
  void deleteShaderProgram();

  unsigned int getShaderProgramId() const;
  bool isLoaded() const;

  void useProgram() const;
};