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

  std::map<std::string, Material*> _mMaterials; // primitiveName to material
  std::map<std::string, Primitive*> _mPrimitives; // primitiveName to primitive
  std::map<std::string, Texture*> _mTextures; // texturePath to texture

  Asset* _mRoot = nullptr;

private:
  void processNode(aiNode* node, const aiScene* scene, Asset* assetNode);

  // process mesh, then returns the name of the mesh, which must be inserted into _mPrimitives and _mMaterials
  std::string processMesh(aiMesh* mesh, const aiScene* scene, Asset* assetNode);
  std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene);

public:
  AssetImporter(GameResources& resources, const std::string& path);
  AssetImporter(const AssetImporter& other) = delete;
  virtual ~AssetImporter();
  void load();

  // create a brand new asset instance (will not be managed internally, caller responsible for deleting it)
  Asset* createInstance(bool cloneMaterial = false) const;

  Asset* getOriginal() const { return _mRoot; }

  // Note: this will remove the asset from memory, so don't call it until done using it.
  // If there's duplicate asset, the cleanup will also clean the resources shared by the other asset, don't use this if possible.
  // Instead, opt to manually remove the assets through the getters
  void cleanupAllResources();

  // Alternatives, remove resources manually via the getters
  std::map<std::string, Material*>& getMaterials() { return _mMaterials; }
  std::map<std::string, Primitive*>& getPrimitives() { return _mPrimitives; };
  std::map<std::string, Texture*>& getTextures() { return _mTextures; };
};