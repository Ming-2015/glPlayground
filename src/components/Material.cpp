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
PhongMaterial::PhongMaterial()
  : diffuse(1, 1, 1), specular(0, 0, 0), ambient(0, 0, 0)
{}

PhongMaterial::PhongMaterial(ShaderProgramManager* manager)
  : diffuse(1, 1, 1), specular(0, 0, 0), ambient(0, 0, 0)
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
  modelMatUniform = _mProgram->getUniformByName("modelMat");
  normalMatUniform = _mProgram->getUniformByName("normalMat");
  projViewModelMatUniform = _mProgram->getUniformByName("projViewModelMat");

  diffuseTexUniform = _mProgram->getUniformByName("phongMaterial.diffuseTex");
  specularTexUniform = _mProgram->getUniformByName("phongMaterial.specularTex");
  ambientTexUniform = _mProgram->getUniformByName("phongMaterial.ambientTex");
  diffuseUniform = _mProgram->getUniformByName("phongMaterial.diffuse");
  specularUniform = _mProgram->getUniformByName("phongMaterial.specular");
  ambientUniform = _mProgram->getUniformByName("phongMaterial.ambient");
  shininessUniform = _mProgram->getUniformByName("phongMaterial.shininess");
}

PhongMaterial::PhongMaterial(const PhongMaterial& other)
  : Material(other), 
  diffuse(other.diffuse), 
  specular(other.specular), 
  ambient(other.ambient)
{
  diffuseTexUniform = other.diffuseTexUniform;
  specularTexUniform = other.specularTexUniform;
  ambientTexUniform = other.ambientTexUniform;
  diffuseUniform = other.diffuseUniform;
  specularUniform = other.specularUniform;
  ambientUniform = other.ambientUniform;
  shininessUniform = other.shininessUniform;
}

PhongMaterial::~PhongMaterial()
{}

void bindTexUniform(Uniform* texUniform, Uniform* colorUniform, Texture* tex, glm::vec3 color, int texIdx)
{
  if (texUniform)
  {
    if (tex)
    {
      texUniform->setUniform(texIdx);
      tex->bind(texIdx);
    }
    else
    {
      texUniform->setUniform(texIdx);
      glActiveTexture(GL_TEXTURE0 + texIdx);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }

  if (colorUniform)
  {
    if (tex)
      colorUniform->setUniform(glm::vec3(0, 0, 0));
    else
      colorUniform->setUniform(color);
  }
}

void PhongMaterial::preRender()
{
  Material::preRender();

  bindTexUniform(diffuseTexUniform, diffuseUniform, diffuseTex, diffuse, DIFFUSE_TEX_IDX);
  bindTexUniform(specularTexUniform, specularUniform, specularTex, specular, SPECULAR_TEX_IDX);
  bindTexUniform(ambientTexUniform, ambientUniform, ambientTex, ambient, AMBIENT_TEX_IDX);

  if (shininessUniform)
  {
    shininessUniform->setUniform(shininess);
  }
}