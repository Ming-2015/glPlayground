#include "Node.h"
#include "../components/ShaderProgram.h"

#pragma once
class Light : public Node
{
public:
  // set the light's uniforms in the shader
  virtual void setProgramUniform(ShaderProgram& shaderProgram, int index) = 0;
  virtual std::string getUniformName() const = 0;
};