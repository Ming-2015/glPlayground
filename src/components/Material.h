#pragma once
#include "../components/ShaderProgramManager.h"
#include "../components/Texture.h"

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
private:
  // texture settings
  Uniform* specularTexUniform = nullptr;
  static const int SPECULAR_TEX_IDX = 1;

  Uniform* diffuseTexUniform = nullptr;
  static const int DIFFUSE_TEX_IDX = 0;

protected:
  virtual void preRender();

public:
  // diffuse texture of the material
  Texture* diffuseTex = nullptr;
  Texture* specularTex = nullptr;

public:
  TestMaterial(
    ShaderProgramManager& manager,
    ShaderProgram& program
  );
  TestMaterial(const TestMaterial& other);
  virtual ~TestMaterial();
};