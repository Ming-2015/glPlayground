#include "Shader.h"

// static
std::map<std::string, Shader> Shader::LoadedShaders = {};
const Shader& Shader::GetOrCreateShader(GLenum type, std::string shaderPath)
{
  std::string shaderKey = ConstructShaderKey(type, shaderPath);
  auto shaderIt = LoadedShaders.find(shaderKey);
  bool found = shaderIt != LoadedShaders.end();

  if (found)
  {
    Shader& shader = (*shaderIt).second;
    return shader;
  }
  else {
    LoadedShaders[shaderKey] = Shader();
    LoadedShaders[shaderKey].loadFromFile(type, shaderPath);
    return LoadedShaders[shaderKey];
  }
}

std::string Shader::ConstructShaderKey(GLenum type, std::string shaderPath)
{
  return shaderPath + "-" + std::to_string(type);
}

void Shader::DeleteShader(GLenum type, std::string shaderPath)
{
  std::string shaderKey = ConstructShaderKey(type, shaderPath);
  auto shaderIt = LoadedShaders.find(shaderKey);
  bool found = shaderIt != LoadedShaders.end();

  if (found)
  {
    LoadedShaders.erase(shaderKey);
  }
  else {
    std::cerr << "Could not find shader to be deleted: " + shaderPath << std::endl;
  }
}

// non static
Shader::Shader()
  : _mType(0), _mShaderId(0), _mIsShaderLoaded(false), _mShaderPath()
{

}

Shader::~Shader() 
{
  deleteShader();
}

void Shader::deleteShader()
{
  if (_mIsShaderLoaded)
  {
    glDeleteShader(_mShaderId);
    _mShaderId = 0;
    _mIsShaderLoaded = false;
    std::cout << "Shader " << _mShaderPath << " successfully removed!" << std::endl;
  }
}

void Shader::loadFromFile(GLenum type, std::string shaderPath)
{
  if (_mIsShaderLoaded)
  {
    throw "Shader is already loaded: " + _mShaderPath;
  }

  _mType = type;
  _mShaderPath = shaderPath;
  _mShaderId = glCreateShader(type);
  
  std::ifstream fileStream;
  fileStream.open(shaderPath);

  if (!fileStream.is_open())
  {
    throw "Cannot open shader file: " + shaderPath;
  }

  std::stringstream buffer;
  buffer << fileStream.rdbuf();

  const std::string& shaderCode = buffer.str();
  const char* shaderCodeCstr = shaderCode.c_str();

  glShaderSource(_mShaderId, 1, &shaderCodeCstr, NULL);
  glCompileShader(_mShaderId);

  // error checking
  int  success;
  char infoLog[512];
  glGetShaderiv(_mShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(_mShaderId, 512, NULL, infoLog);
    std::cerr << "ERROR: Compilation of shader from path " << shaderPath << " failed!" << std::endl;
    std::cerr << infoLog << std::endl;
    throw "Failed to compile shader: " + shaderPath;
  }

  _mIsShaderLoaded = true;
  std::cout << "Shader " << _mShaderPath << " successfully loaded!" << std::endl;
}

unsigned int Shader::getShaderId() const
{
  return _mShaderId;
}

bool Shader::isShaderLoaded() const
{
  return _mIsShaderLoaded;
}

std::string Shader::getShaderKey() const
{
  return Shader::ConstructShaderKey(_mType, _mShaderPath);
}