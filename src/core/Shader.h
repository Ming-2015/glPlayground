#pragma once
#include <string>

class Shader
{
private:

public:
  Shader();
  virtual ~Shader();

  void loadFromFile(std::string shaderPath);
};