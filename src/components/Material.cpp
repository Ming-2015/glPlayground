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

void MaterialBase::use() {
  if (_mProgramManager && _mProgram) {
    _mProgramManager->useProgram(_mProgram);
    preRender();
  }
  else {
    Log.print<Severity::warning>("Failed to render material since program is not initialized!");
  }
}

ShaderProgram* getShaderProgram(
  ShaderProgramManager& programManager,
  const std::string& programKey, 
  const std::string& vsPath, 
  const std::string& fsPath
) {
  ShaderProgram* program = programManager.find(programKey);
  if (!program)
  {
    ShaderManager& shaderManager = programManager.getShaderManager();
    Shader* vs = shaderManager.find(vsPath);
    Shader* fs = shaderManager.find(fsPath);

    if (!vs)
    {
      ShaderData vsData(
        vsPath,
        GL_VERTEX_SHADER
      );

      vs = shaderManager.insert(vsPath, vsData);
    }

    if (!fs)
    {
      ShaderData fsData(
        fsPath,
        GL_FRAGMENT_SHADER
      );

      fs = shaderManager.insert(fsPath, fsData);
    }

    ShaderProgramData programData(
      vsPath,
      fsPath
    );

    program = programManager.insert(programKey, programData);
  }

  if (!program)
  {
    Log.print<Severity::error>("Failed to retrieve shader program!");
    throw std::runtime_error("Program is not found!");
  }

  if (!program->isLoaded()) {
    Log.print<Severity::error>("Trying to initialize material with unloaded program!");
    throw std::runtime_error("Program is not loaded!");
  }

  return program;
}

Material::Material() {}

Material::Material(ShaderProgramManager* manager)
{
  _mProgramManager = manager;
  static std::string vsPath = "./shaders/Phong.vs";
  static std::string fsPath = "./shaders/Phong.fs";
  static std::string programKey = vsPath + "___" + fsPath;
  _mProgram = getShaderProgram(*_mProgramManager, programKey, vsPath, fsPath);

  modelMatUniform = _mProgram->getUniformByName("modelMat");
  normalMatUniform = _mProgram->getUniformByName("normalMat");
  projViewModelMatUniform = _mProgram->getUniformByName("projViewModelMat");
  alphaCutoffUniform = _mProgram->getUniformByName("alphaCutoff");
}

Material::Material(const Material& other)
{
  _mProgramManager = other._mProgramManager;
  _mProgram = other._mProgram;
  modelMatUniform = other.modelMatUniform;
  normalMatUniform = other.normalMatUniform;
  projViewModelMatUniform = other.projViewModelMatUniform;
  alphaCutoff = other.alphaCutoff;
  alphaCutoffUniform = other.alphaCutoffUniform;
}

Material::~Material()
{}

void Material::preRender()
{
  if (alphaCutoffUniform)
    alphaCutoffUniform->setUniform(.5f);
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
  : diffuse(1), specular(1), ambient(1)
{}

PhongMaterial::PhongMaterial(ShaderProgramManager* manager)
  : diffuse(1), specular(1), ambient(1)
{
  _mProgramManager = manager;
  static std::string vsPath = "./shaders/Phong.vs";
  static std::string fsPath = "./shaders/Phong.fs";
  static std::string programKey = vsPath + "___" + fsPath;
  _mProgram = getShaderProgram(*_mProgramManager, programKey, vsPath, fsPath);

  alphaCutoffUniform = _mProgram->getUniformByName("alphaCutoff");
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

  diffuseUVIndexUniform = _mProgram->getUniformByName("phongMaterial.diffuseUVIndex");
  specularUVIndexUniform = _mProgram->getUniformByName("phongMaterial.specularUVIndex");
  ambientUVIndexUniform = _mProgram->getUniformByName("phongMaterial.ambientUVIndex");
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
  diffuseUVIndexUniform = other.diffuseUVIndexUniform;
  specularUVIndexUniform = other.specularUVIndexUniform;
  ambientUVIndexUniform = other.ambientUVIndexUniform;
}

PhongMaterial::~PhongMaterial()
{}

void bindTexUniform(Uniform* texUniform, Uniform* colorUniform, Texture* tex, glm::vec4 color, int texIdx)
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
    colorUniform->setUniform(color);
  }
}

void PhongMaterial::preRender()
{
  Material::preRender();

  bindTexUniform(diffuseTexUniform, diffuseUniform, diffuseTex, diffuse, DIFFUSE_TEX_IDX);
  bindTexUniform(specularTexUniform, specularUniform, specularTex, specular, SPECULAR_TEX_IDX);
  bindTexUniform(ambientTexUniform, ambientUniform, ambientTex, ambient, AMBIENT_TEX_IDX);

  if (diffuseUVIndexUniform) {
    if (!diffuseTex)
      diffuseUVIndexUniform->setUniform(-1);
    else
      diffuseUVIndexUniform->setUniform(diffuseUVIndex);
  }

  if (specularUVIndexUniform) {
    if (!specularTex)
      specularUVIndexUniform->setUniform(-1);
    else
      specularUVIndexUniform->setUniform(specularUVIndex);
  }

  if (ambientUVIndexUniform) 
  {
    if (!ambientTex)
      ambientUVIndexUniform->setUniform(-1);
    else
      ambientUVIndexUniform->setUniform(ambientUVIndex);
  }

  if (shininessUniform)
    shininessUniform->setUniform(shininess);
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

// ----------------- Screen shader -----------------------
ScreenShader::ScreenShader(ShaderProgramManager* manager)
{
  _mProgramManager = manager;
  static std::string vsPath = "./shaders/ScreenShader.vs";
  static std::string fsPath = "./shaders/ScreenShader.fs";
  static std::string programKey = vsPath + "___" + fsPath;
  _mProgram = getShaderProgram(*_mProgramManager, programKey, vsPath, fsPath);

  _mScreenTextureUniform = _mProgram->getUniformByName("screenTexture");
}

ScreenShader::ScreenShader(const ScreenShader& other)
  : Material(other)
{
  _mProgramManager = other._mProgramManager;
  _mProgram = other._mProgram;
  _mScreenTextureUniform = other._mScreenTextureUniform;
  screenTextureId = other.screenTextureId;
}

ScreenShader::~ScreenShader()
{}

void ScreenShader::copyTo(Cloneable* cloned) const
{
  Material::copyTo(cloned);

  ScreenShader* clonedMaterial = dynamic_cast<ScreenShader*>(cloned);
  if (!clonedMaterial)
  {
    Log.print<Severity::warning>("Failed to cast to PhongMaterial in clone");
    return;
  }

  clonedMaterial->_mScreenTextureUniform = _mScreenTextureUniform;
}

ScreenShader* ScreenShader::clone() const
{
  ScreenShader* material = new ScreenShader(_mProgramManager);
  copyTo(material);
  return material;
}

void ScreenShader::preRender()
{
  if (_mScreenTextureUniform) {
    _mScreenTextureUniform->setUniform(0);
    glBindTextureUnit(0, screenTextureId);
  }
}