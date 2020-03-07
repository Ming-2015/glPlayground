#include "Material.h"

TestMaterial::TestMaterial(
  ShaderProgramManager& manager,
  ShaderProgram& program
) : diffuseTex(nullptr)
{
  linkToProgram(&manager, &program);
}

TestMaterial::TestMaterial(const TestMaterial& other)
{
  diffuseTex = other.diffuseTex;
  linkToProgram(other._mProgramManager, other._mProgram);
}

TestMaterial::~TestMaterial()
{}

void TestMaterial::preRender()
{
  _mProgramManager->useProgram(_mProgram);
  if (diffuseTex) diffuseTex->bindTexture();
}