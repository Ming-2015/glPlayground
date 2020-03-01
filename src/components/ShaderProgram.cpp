#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
  : _mShaderProgram(0), _mIsLoaded(false)
{

}

ShaderProgram::~ShaderProgram()
{
  deleteShaderProgram();
}

void ShaderProgram::initShaderProgram(
  const Shader& vertexShader,
  const Shader& fragmentShader
)
{
  _mShaderProgram = glCreateProgram();
  glAttachShader(_mShaderProgram, vertexShader.getShaderId());
  glAttachShader(_mShaderProgram, fragmentShader.getShaderId());
  glLinkProgram(_mShaderProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(_mShaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(_mShaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR: Linking of shader program failed!" << std::endl;
    std::cerr << infoLog << std::endl;
    throw "Failed to link shader program";
  }

  _mIsLoaded = true;
  std::cout << "Shader Program successfully loaded!" << std::endl;
}

void ShaderProgram::deleteShaderProgram()
{
  if (_mIsLoaded)
  {
    glDeleteProgram(_mShaderProgram);
    _mShaderProgram = 0;
    _mIsLoaded = false;
    std::cout << "Shader Program successfully deleted!" << std::endl;
  }
  else
  {
    std::cerr << "Cannot delete shader program before it is loaded!" << std::endl;
  }
}

unsigned int ShaderProgram::getShaderProgramId() const
{
  return _mShaderProgram;
}

bool ShaderProgram::isLoaded() const
{
  return _mIsLoaded;
}

void ShaderProgram::useProgram() const
{
  glUseProgram(_mShaderProgram);
}