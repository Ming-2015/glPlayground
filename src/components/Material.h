#pragma once
#include "../components/ShaderProgram.h"
#include "../components/Texture.h"

// an interface for all materials
class MaterialBase
{
protected:
  ShaderProgram* _mProgram = nullptr;
  ShaderProgramManager* _mProgramManager = nullptr;

  // this should prep a program to be used before a draw call
  virtual void preRender() = 0;

public:
  void use() { preRender(); }
};


class Material : public MaterialBase
{
protected:
  Uniform* modelMatUniform = nullptr;
  Uniform* normalMatUniform = nullptr;
  Uniform* projViewModelMatUniform = nullptr;

protected:
  // not public: has to generated with a factory method!
  Material();
  virtual void preRender();

public:
  Material(ShaderProgramManager* manager);
  Material(const Material& other);
  virtual ~Material();

  // these matrices need to be set before a model can be rendered!
  void setModelMatrix(const glm::mat4& model);
  void setProjViewModelMatrix(const glm::mat4& projViewModel);
  void setNormalMatrix(const glm::mat3& normal);
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

  Uniform* diffuseUniform = nullptr;
  Uniform* specularUniform = nullptr;
  Uniform* ambientUniform = nullptr;

  Uniform* shininessUniform = nullptr;

protected:
  PhongMaterial();
  virtual void preRender();

public:
  // diffuse texture of the material
  Texture* diffuseTex = nullptr;
  Texture* specularTex = nullptr;
  Texture* ambientTex = nullptr;

  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 ambient;

  int shininess = 32;

public:
  PhongMaterial(ShaderProgramManager* manager);
  PhongMaterial(const PhongMaterial& other);
  virtual ~PhongMaterial();
};