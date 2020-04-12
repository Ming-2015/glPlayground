#include "Material.h"

void MaterialBase::copyTo(Cloneable* cloned) const
{
  MaterialBase* mat = dynamic_cast<MaterialBase*>(cloned);
  if (!cloned)
  {
    Log.print<Severity::warning>("Failed to cast Material Base in clone");
    return;
  }

  mat->_mProgram = _mProgram;
  mat->_mProgramManager = _mProgramManager;
}

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

void Material::copyTo(Cloneable* cloned) const
{
  MaterialBase::copyTo(cloned);
  Material* clonedMaterial = dynamic_cast<Material*>(cloned);
  if (!clonedMaterial)
  {
    Log.print<Severity::warning>("Failed to cast to Material in clone");
    return;
  }
}

Material* Material::clone() const
{
  Material* material = new Material(_mProgramManager);
  copyTo(material);
  return material;
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

void PhongMaterial::copyTo(Cloneable* cloned) const
{
  Material::copyTo(cloned);
  PhongMaterial* clonedMaterial = dynamic_cast<PhongMaterial*>(cloned);
  if (!clonedMaterial)
  {
    Log.print<Severity::warning>("Failed to cast to PhongMaterial in clone");
    return;
  }

  clonedMaterial->ambient = ambient;
  clonedMaterial->diffuseTex = diffuseTex;
  clonedMaterial->specularTex = specularTex;
  clonedMaterial->ambientTex = ambientTex;
  clonedMaterial->diffuse = diffuse;
  clonedMaterial->specular = specular;
  clonedMaterial->ambient = ambient;
  clonedMaterial->shininess = shininess;
}

PhongMaterial* PhongMaterial::clone() const
{
  PhongMaterial* material = new PhongMaterial(_mProgramManager);
  copyTo(material);
  return material;
}