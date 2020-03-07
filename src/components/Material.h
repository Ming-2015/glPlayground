#pragma once
#include "ShaderProgramManager.h"
#include "Texture.h"

// an interface for all materials
class Material
{
protected:
  ShaderProgram* _mProgram;
  ShaderProgramManager* _mProgramManager;

  virtual void linkToProgram(
    ShaderProgramManager* manager,
    ShaderProgram* program
  )
  {
    _mProgramManager = manager;
    _mProgram = program;
  }
  virtual void preRender() = 0;

public:
  virtual void use() {
    preRender();
  }
};

class TestMaterial : public Material
{
protected:
  virtual void preRender();

public:
  // diffuse texture of the material
  Texture* diffuseTex;

public:
  TestMaterial(
    ShaderProgramManager& manager,
    ShaderProgram& program
  );
  TestMaterial(const TestMaterial& other);
  virtual ~TestMaterial();
};