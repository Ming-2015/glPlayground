#include "Material.h"

TestMaterial::TestMaterial(
  ShaderProgramManager& manager,
  ShaderProgram& program
)
{
  if (!program.isLoaded()) {
    Log.print<Severity::error>("Trying to initialize material with unloaded program!");
    throw std::runtime_error("Program is not loaded!");
  }

  linkToProgram(&manager, &program);
  diffuseTexUniform = _mProgram->getUniformByName("diffuseTex");
  specularTexUniform = _mProgram->getUniformByName("specularTex");
}

TestMaterial::TestMaterial(const TestMaterial& other)
{
  diffuseTex = other.diffuseTex;
  linkToProgram(other._mProgramManager, other._mProgram);
  diffuseTexUniform = other.diffuseTexUniform;
  specularTexUniform = other.specularTexUniform;
}

TestMaterial::~TestMaterial()
{}

void TestMaterial::preRender()
{
  _mProgramManager->useProgram(_mProgram);

  // setting a diffuse texture 
  if (diffuseTex && diffuseTexUniform)
  {
    diffuseTexUniform->setUniform(DIFFUSE_TEX_IDX);
    diffuseTex->bind(DIFFUSE_TEX_IDX);
  }

  if (specularTex && specularTexUniform) 
  {
    specularTexUniform->setUniform(SPECULAR_TEX_IDX);
    specularTex->bind(SPECULAR_TEX_IDX);
  }
}

