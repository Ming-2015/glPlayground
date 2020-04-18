#pragma once
#include "../components/ShaderProgram.h"
#include "../components/Texture.h"
#include "../utils/Cloneable.hpp"

// an interface for all materials
class MaterialBase : public Cloneable
{
protected:
  ShaderProgram* _mProgram = nullptr;
  ShaderProgramManager* _mProgramManager = nullptr;

  // this should prep a program to be used before a draw call
  virtual void preRender() = 0;
  virtual void copyTo(Cloneable* cloned) const override;

public:
  void use();
  virtual MaterialBase* clone() const override = 0;

  std::string name;
};

class Material : public MaterialBase
{
protected:
  Uniform* modelMatUniform = nullptr;
  Uniform* normalMatUniform = nullptr;
  Uniform* projViewModelMatUniform = nullptr;
  Uniform* alphaCutoffUniform = nullptr;

protected:
  // not public: has to generated with a factory method!
  Material();
  virtual void preRender() override;
  virtual void copyTo(Cloneable* cloned) const override;

public:
  Material(ShaderProgramManager* manager);
  Material(const Material& other);
  virtual ~Material();

  // these matrices need to be set before a model can be rendered!
  void setModelMatrix(const glm::mat4& model);
  void setProjViewModelMatrix(const glm::mat4& projViewModel);
  void setNormalMatrix(const glm::mat3& normal);

  // alpha cutoff of the material
  float alphaCutoff = 0.f;
  bool useAlphaBlending = false;

  virtual Material* clone() const override;
};

class ScreenShader : public Material {
protected:
  Uniform* _mScreenTextureUniform = nullptr;

protected:
  virtual void preRender() override;
  virtual void copyTo(Cloneable* cloned) const override;

public:
  GLuint screenTextureId = 0;

public:
  ScreenShader(ShaderProgramManager* manager);
  ScreenShader(const ScreenShader& other);
  virtual ~ScreenShader();

  virtual ScreenShader* clone() const override;
};

class PhongMaterial : public Material 
{
private:
  // uniforms
  Uniform* ambientTexUniform = nullptr;
  static const int AMBIENT_TEX_IDX = 2;

  Uniform* specularTexUniform = nullptr;
  static const int SPECULAR_TEX_IDX = 1;

  Uniform* diffuseTexUniform = nullptr;
  static const int DIFFUSE_TEX_IDX = 0;

  Uniform* diffuseUVIndexUniform = nullptr;
  Uniform* diffuseUniform = nullptr;

  Uniform* specularUVIndexUniform = nullptr;
  Uniform* specularUniform = nullptr;

  Uniform* ambientUVIndexUniform = nullptr;
  Uniform* ambientUniform = nullptr;

  Uniform* shininessUniform = nullptr;

protected:
  PhongMaterial();
  virtual void preRender();
  virtual void copyTo(Cloneable* cloned) const override;

public:
  // diffuse texture of the material
  int diffuseUVIndex = 0;
  Texture* diffuseTex = nullptr;
  
  int specularUVIndex = 0;
  Texture* specularTex = nullptr;

  int ambientUVIndex = 0;
  Texture* ambientTex = nullptr;

  glm::vec4 diffuse;
  glm::vec4 specular;
  glm::vec4 ambient;

  int shininess = 32;

public:
  PhongMaterial(ShaderProgramManager* manager);
  PhongMaterial(const PhongMaterial& other);
  virtual ~PhongMaterial();

  virtual PhongMaterial* clone() const override;
};