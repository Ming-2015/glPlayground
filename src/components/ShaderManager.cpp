#include "ShaderManager.h"

// ShaderInfo
bool ShaderInfo::operator< (const ShaderInfo& other) const 
{
  if (shaderType != other.shaderType)
  {
    return shaderType < other.shaderType;
  }
  else
  {
    return shaderPath < other.shaderPath;
  }
}

bool ShaderInfo::operator== (const ShaderInfo& other) const 
{
  return shaderPath == other.shaderPath && shaderType == other.shaderType;
}

bool ShaderInfo::isValid() const 
{
  return !shaderPath.empty() && shaderType;
}

const std::string ShaderInfo::toString() const 
{
  return shaderPath + " " + std::to_string(shaderType);
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
    std::cout << "Shader " << _mShaderInfo.shaderPath << " successfully removed!" << std::endl;

    _mShaderId = 0;
    _mIsShaderLoaded = false;
    _mShaderInfo = ShaderInfo();
  }
}

void Shader::load(const ShaderInfo& info)
{
  if (_mIsShaderLoaded)
  {
    std::cerr << "Shader is already loaded: " << info.toString() << std::endl;
    throw "Shader is already loaded: " + info.shaderPath;
  }
  
  // read the file
  std::ifstream fileStream;
  fileStream.open(info.shaderPath);
  if (!fileStream.is_open())
  {
    std::cerr << "Cannot open shader file: " << info.toString() << std::endl;
    throw "Cannot open shader file: " + info.shaderPath;
  }
  std::stringstream buffer;
  buffer << fileStream.rdbuf();
  const std::string& shaderCode = buffer.str();
  const char* shaderCodeCstr = shaderCode.c_str();

  // initialize variables
  _mShaderId = glCreateShader(info.shaderType);

  // compile the shader
  glShaderSource(_mShaderId, 1, &shaderCodeCstr, NULL);
  glCompileShader(_mShaderId);

  // error checking
  int  success;
  char infoLog[512];
  glGetShaderiv(_mShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glDeleteShader(_mShaderId);
    _mShaderId = 0;

    glGetShaderInfoLog(_mShaderId, 512, NULL, infoLog);
    std::cerr << "ERROR: Compilation of shader from path " << info.shaderPath << " failed!" << std::endl;
    std::cerr << infoLog << std::endl;
    throw "Failed to compile shader: " + info.shaderPath;
  }

  _mShaderInfo = info;
  _mIsShaderLoaded = true;
  std::cout << "Shader " << _mShaderInfo.shaderPath << " successfully loaded!" << std::endl;
}

unsigned int Shader::getShaderId() const
{
  return _mShaderId;
}

bool Shader::isLoaded() const
{
  return _mIsShaderLoaded;
}

const ShaderInfo& Shader::getShaderInfo() const
{
  return _mShaderInfo;
}

// ShaderManager
ShaderManager::ShaderManager() {}

Shader* const ShaderManager::create(const ShaderInfo& info)
{
  Shader* shader = new Shader();
  shader->load(info);
  return shader;
}

void ShaderManager::destroy(Shader* const value)
{
  delete value;
}