#pragma once
#include <string>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

class Shader
{
private:
  GLenum _mType;
  std::string _mShaderPath;

  unsigned int _mShaderId;
  bool _mIsShaderLoaded;


  void loadFromFile(GLenum type, std::string shaderPath);
  void deleteShader();

  static std::map<std::string, Shader> LoadedShaders;

protected:

public:
  Shader();
  virtual ~Shader();

  unsigned int getShaderId() const;
  bool isShaderLoaded() const;
  std::string getShaderKey() const;

  static const Shader& GetOrCreateShader(GLenum type, std::string shaderPath);
  static std::string ConstructShaderKey(GLenum type, std::string shaderPath);
  static void DeleteShader(GLenum type, std::string shaderPath);
};