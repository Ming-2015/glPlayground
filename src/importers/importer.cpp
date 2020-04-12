#include "importer.h"
#include "../utils/Logger.h"

AssetImporter::AssetImporter(GameResources& resources, const std::string& path)
  : _mResources(resources), _mPath(path)
{
  _mDirectory = _mPath.substr(0, _mPath.find_last_of('/'));
}

AssetImporter::~AssetImporter()
{}

void AssetImporter::load()
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
    _mPath, 
    aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
  );

  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) 
  {
    Log.print<Severity::error>("!! Assimp failed to load file: ", _mPath);
    Log.print<Severity::error>("!! Assimp Error Message: ", importer.GetErrorString());
    return;
  }

  processNode(scene->mRootNode, scene);
}


void AssetImporter::processNode(aiNode* node, const aiScene* scene)
{

}

void AssetImporter::processMesh(aiMesh* mesh, const aiScene* scene)
{

}

void AssetImporter::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{

}

void AssetImporter::cleanupResources()
{
  if (_mRoot) 
  {
    delete _mRoot;
  }
  _mRoot = nullptr;

  for (auto material : _mMaterials)
  {
    delete material;
  }

  _mMaterials.clear();

  for (auto primitivePair : _mPrimitives)
  {
    _mResources.primitiveManager.erase(primitivePair.first);
  }
  _mPrimitives.clear();

  for (auto texturePair : _mTextures)
  {
    _mResources.textureManager.erase(texturePair.first);
  }
  _mTextures.clear();
}

Asset* AssetImporter::createInstance(bool cloneMaterial) const
{
  return _mRoot;
}