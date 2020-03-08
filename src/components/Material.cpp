#include "Material.h"
// normal material...
Material::Material()
{}

Material::Material(ShaderProgramManager* manager)
{
  _mProgramManager = manager;

  // initialize the material
  ShaderInfo vertexShaderInfo(
    "./shaders/VertexShader.glsl",
    GL_VERTEX_SHADER
  );

  ShaderInfo fragmentShaderInfo(
    "./shaders/FragmentShader.glsl",
    GL_FRAGMENT_SHADER
  );

  ShaderProgramInfo programInfo(
    vertexShaderInfo,
    fragmentShaderInfo
  );

  Log.print<Severity::debug>("Creating a normal material...");

  // get the shader program
  ShaderProgram* program = _mProgramManager->getOrCreate(programInfo);

  if (!program->isLoaded()) {
    Log.print<Severity::error>("Trying to initialize material with unloaded program!");
    throw std::runtime_error("Program is not loaded!");
  }
  
  _mProgram = program;
  modelMatUniform = _mProgram->getUniformByName("modelMat");
  normalMatUniform = _mProgram->getUniformByName("normalMat");
  projViewModelMatUniform = _mProgram->getUniformByName("projViewModelMat");
}

Material::Material(const Material& other)
{
  _mProgramManager = other._mProgramManager;
  _mProgram = other._mProgram;
  modelMatUniform = other.modelMatUniform;
  normalMatUniform = other.normalMatUniform;
  projViewModelMatUniform = other.projViewModelMatUniform;
}

Material::~Material()
{}

void Material::preRender()
{
  _mProgramManager->useProgram(_mProgram);
}

void Material::setModelMatrix(const glm::mat4& model)
{
  if (modelMatUniform)
    modelMatUniform->setUniform(model);
}

void Material::setProjViewModelMatrix(const glm::mat4& projViewModel)
{
  if (projViewModelMatUniform)
    projViewModelMatUniform->setUniform(projViewModel);
}

void Material::setNormalMatrix(const glm::mat3& normal)
{
  if (normalMatUniform)
    normalMatUniform->setUniform(normal);
}

// ----------- phoon material ---------------
PhoonMaterial::PhoonMaterial()
{}

PhoonMaterial::PhoonMaterial(ShaderProgramManager* manager)
{
  _mProgramManager = manager;
  if (!_mProgramManager)
  {
    Log.print<Severity::warning>("Trying to create material with null manager");
    return;
  }

  // initialize the material
  ShaderInfo vertexShaderInfo(
    "./shaders/VertexShader.glsl",
    GL_VERTEX_SHADER
  );

  ShaderInfo fragmentShaderInfo(
    "./shaders/FragmentShader.glsl",
    GL_FRAGMENT_SHADER
  );

  ShaderProgramInfo programInfo(
    vertexShaderInfo,
    fragmentShaderInfo
  );

  Log.print<Severity::debug>("Creating a phoon material...");

  // get the shader program
  ShaderProgram* program = manager->getOrCreate(programInfo);

  if (!program->isLoaded()) {
    Log.print<Severity::error>("Trying to initialize material with unloaded program!");
    throw std::runtime_error("Program is not loaded!");
  }

  _mProgram = program;
  diffuseTexUniform = _mProgram->getUniformByName("diffuseTex");
  specularTexUniform = _mProgram->getUniformByName("specularTex");
  modelMatUniform = _mProgram->getUniformByName("modelMat");
  normalMatUniform = _mProgram->getUniformByName("normalMat");
  projViewModelMatUniform = _mProgram->getUniformByName("projViewModelMat");
}

PhoonMaterial::PhoonMaterial(const PhoonMaterial& other)
  : Material(other)
{
  diffuseTexUniform = other.diffuseTexUniform;
  specularTexUniform = other.specularTexUniform;
}

PhoonMaterial::~PhoonMaterial()
{}

void PhoonMaterial::preRender()
{
  Material::preRender();

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