#pragma once
#include <glm/glm.hpp>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../components/GameResources.h"
#include "../components/Material.h"
#include "../scene/Model.h"
#include "../scene/Asset.h"

class AssetImporter {
private:
  GameResources& _mResources;
  std::string _mPath;
  std::string _mDirectory;

  std::map<PrimitiveInfo, Primitive*> _mPrimitives;
  std::map<TextureInfo, Texture*> _mTextures;

  // all materials allocated
  std::vector<Material*> _mMaterials;

  Asset* _mRoot = nullptr;

private:
  void processNode(aiNode* node, const aiScene* scene);
  void processMesh(aiMesh* mesh, const aiScene* scene);
  void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
  AssetImporter(GameResources& resources, const std::string& path);
  virtual ~AssetImporter();
  void load();

  // create a brand new asset instance (will not be managed internally, caller responsible for deleting it)
  Asset* createInstance(bool cloneMaterial = false) const;

  // note: this will remove the asset from memory, so don't call it until done using it
  void cleanupResources();
};