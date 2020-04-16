#pragma once
#include "Node.h"
#include "../components/ShaderProgram.h"

class LightBase
{
public:
  // set the light's uniforms in the shader
  virtual void setProgramUniform(ShaderProgram& shaderProgram, int index) = 0;
  virtual std::string getUniformName() const = 0;
};

class Light : public LightBase, public Node {
protected:
  virtual void copyTo(Cloneable* cloned) const override
  {
    Node::copyTo(cloned);
  }
public:
  virtual Light* clone() const override = 0;
  Light() : Node() {}
};