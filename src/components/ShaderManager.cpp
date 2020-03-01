#include "ShaderManager.h"

// ShaderManager
ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{

}

Shader& ShaderManager::getOrCreate(const ShaderInfo& info)
{
  auto shaderIt = resources.find(info);
  bool found = shaderIt != resources.end();

  if (found)
  {
    return (*shaderIt).second;
  }
  else {
    auto it = resources.emplace(info, Shader());
    resources[info].loadFromFile(info);
    return resources[info];
  }
}

void ShaderManager::remove(const ShaderInfo& info)
{
  auto shaderIt = resources.find(info);
  bool found = shaderIt != resources.end();

  if (found)
  {
    resources.erase(info);
  }
  else {
    std::cerr << "Could not find shader to be deleted: " + info.shaderPath << std::endl;
  }
}

void ShaderManager::removeAll()
{
  resources.clear();
}

// Shader

// non static
Shader::Shader()
  : _mShaderInfo({}), _mShaderId(0), _mIsShaderLoaded(false)
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
    std::cout << "Shader " << _mShaderInfo.shaderPath << " successfully removed!" << std::endl;
  }
}

void Shader::loadFromFile(const ShaderInfo& info)
{
  if (_mIsShaderLoaded)
  {
    throw "Shader is already loaded: " + info.shaderPath;
  }

  _mShaderInfo = info;
  _mShaderId = glCreateShader(_mShaderInfo.shaderType);
  
  std::ifstream fileStream;
  fileStream.open(_mShaderInfo.shaderPath);

  if (!fileStream.is_open())
  {
    throw "Cannot open shader file: " + _mShaderInfo.shaderPath;
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
    std::cerr << "ERROR: Compilation of shader from path " << _mShaderInfo.shaderPath << " failed!" << std::endl;
    std::cerr << infoLog << std::endl;
    throw "Failed to compile shader: " + _mShaderInfo.shaderPath;
  }

  _mIsShaderLoaded = true;
  std::cout << "Shader " << _mShaderInfo.shaderPath << " successfully loaded!" << std::endl;
}

unsigned int Shader::getShaderId() const
{
  return _mShaderId;
}

bool Shader::isShaderLoaded() const
{
  return _mIsShaderLoaded;
}

const ShaderInfo& Shader::getShaderInfo() const
{
  return _mShaderInfo;
}