#include "AssetImporter.h"
#include "../utils/Logger.h"

AssetImporter::AssetImporter(GameResources& resources, const std::string& path)
  : _mResources(resources), _mPath(path)
{
  _mDirectory = _mPath.substr(0, _mPath.find_last_of('/'));
}

AssetImporter::~AssetImporter()
{
  if (_mRoot)
  {
    delete _mRoot;
  }
}

void AssetImporter::load()
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
    _mPath, 
    aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords
  );

  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) 
  {
    Log.print<Severity::error>("!! Assimp failed to load file: ", _mPath);
    Log.print<Severity::error>("!! Assimp Error Message: ", importer.GetErrorString());
    return;
  }

  _mRoot = new Asset();
  processNode(scene->mRootNode, scene, _mRoot);
}

void AssetImporter::processNode(aiNode* node, const aiScene* scene, Asset* assetNode)
{
  // transformation
  aiVector3D position, scaling;
  aiQuaternion rotation;
  node->mTransformation.Decompose(scaling, rotation, position);
  assetNode->setPosition(glm::vec3(position.x, position.y, position.z));
  assetNode->setRotationQuaternion(glm::quat(rotation.x, rotation.y, rotation.z, rotation.w));
  assetNode->setScale(glm::vec3(scaling.x, scaling.y, scaling.z));

  // meshes
  for (unsigned int i = 0; i < node->mNumMeshes; i++) 
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    std::string primitiveName = processMesh(mesh, scene, assetNode);

    Model* model = new Model(_mPrimitives[primitiveName]);
    auto it = _mMaterials.find(primitiveName);
    if (it != _mMaterials.end())
    {
      model->material = (*it).second;
    }
    else
    {
      Material* blandMaterial = new Material(&_mResources.shaderProgramManager);
      model->material = blandMaterial;
      _mMaterials[primitiveName] = blandMaterial;
    }

    assetNode->addModel(primitiveName, model, true);
  }

  // children
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    Asset* newAsset = new Asset();
    assetNode->addChild(newAsset);
    processNode(node->mChildren[i], scene, newAsset);
  }
}


std::string AssetImporter::processMesh(aiMesh* mesh, const aiScene* scene, Asset* assetNode)
{
  std::string primitiveName(mesh->mName.C_Str());
  primitiveName = _mPath + "___" + primitiveName;

  Log.print<Severity::debug>("Processing mesh: ", primitiveName, " now!");

  auto it = _mPrimitives.find(primitiveName);
  if (it == _mPrimitives.end())
  {
    Primitive* primitive = _mResources.primitiveManager.find(primitiveName);
    if (!primitive)
    {
      PrimitiveData data;
      auto numChannels = mesh->GetNumUVChannels();
      numChannels = glm::min(numChannels, (unsigned int)Primitive::MAX_TEX_COORDINATE_SUPPORTED);
      std::vector< unsigned int* > numComponents;
      numComponents.push_back(&data.numComponents);
      numComponents.push_back(&data.numComponents_2);
      numComponents.push_back(&data.numComponents_3);
      for (int c = 0; c < numChannels; c++)
      {
        unsigned int nc = mesh->mNumUVComponents[c];
        *numComponents[c] = nc;
      }

      bool hasNormals = mesh->mNormals != NULL;
      bool hasTangents = mesh->mTangents != NULL;
      bool hasBitangents = mesh->mBitangents != NULL;
      for (unsigned i = 0; i < mesh->mNumVertices; i++)
      {
        aiVector3D vertex = mesh->mVertices[i];
        data.vertices.push_back(vertex.x);
        data.vertices.push_back(vertex.y);
        data.vertices.push_back(vertex.z);

        if (hasNormals)
        {
          aiVector3D normal = mesh->mNormals[i];
          data.normals.push_back(normal.x);
          data.normals.push_back(normal.y);
          data.normals.push_back(normal.z);
        }

        if (hasTangents)
        {
          aiVector3D tangent = mesh->mTangents[i];
          data.tangents.push_back(tangent.x);
          data.tangents.push_back(tangent.y);
          data.tangents.push_back(tangent.z);
        }

        if (hasBitangents)
        {
          aiVector3D bitangent = mesh->mBitangents[i];
          data.tangents.push_back(bitangent.x);
          data.tangents.push_back(bitangent.y);
          data.tangents.push_back(bitangent.z);
        }

        // process multi tex coords
        std::vector< std::vector<float>* > texCoords;
        texCoords.push_back(&data.texCoords);
        texCoords.push_back(&data.texCoords_2);
        texCoords.push_back(&data.texCoords_3);
        for (int c = 0; c < numChannels; c++)
        {
          aiVector3D tex = mesh->mTextureCoords[c][i];
          if (*numComponents[c] > 0)
          {
            texCoords[c]->push_back(tex.x);
          }
          if (*numComponents[c] > 1)
          {
            texCoords[c]->push_back(tex.y);
          }
          if (*numComponents[c] > 2)
          {
            texCoords[c]->push_back(tex.z);
          }
        }
      }

      // triangulated faces!
      for (int i = 0; i < mesh->mNumFaces; i++)
      {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
          data.indices.push_back(face.mIndices[j]);
        }
      }

      primitive = _mResources.primitiveManager.insert(primitiveName, data);
    }
    _mPrimitives[primitiveName] = primitive;
  }

  if (mesh->mMaterialIndex >= 0)
  {
    if (_mMaterials.find(primitiveName) == _mMaterials.end()) 
    {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
      auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
      auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, scene);
      auto ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, scene);

      PhongMaterial* phongMat = new PhongMaterial(&_mResources.shaderProgramManager);
      if (diffuseMaps.size() > 0)
      {
        phongMat->diffuseTex = diffuseMaps[diffuseMaps.size() - 1];
        phongMat->ambientTex = diffuseMaps[diffuseMaps.size() - 1];
        
        if (diffuseMaps.size() > 1)
          Log.print<Severity::warning>("Multiple diffuse maps is not supported yet!");
      }

      if (specularMaps.size() > 0)
      {
        phongMat->specularTex = specularMaps[0];
        if (specularMaps.size() > 1)
          Log.print <Severity::warning>("Multiple specular maps is not supported yet!");
      }

      if (ambientMaps.size() > 0)
      {
        phongMat->ambientTex = ambientMaps[0];
        if (ambientMaps.size() > 1)
          Log.print<Severity::warning>("Multiple ambient maps is not supported yet!");
      }

      aiString name;
      material->Get(AI_MATKEY_NAME, name);

      aiColor3D diffuse(0, 0, 0);
      material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

      aiColor3D specular(0, 0, 0);
      material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

      aiColor3D ambient(0, 0, 0);
      material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);

      float shininess = 0.f;
      material->Get(AI_MATKEY_SHININESS, shininess);

      float shininessStrength = 1.f;
      material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);

      phongMat->diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
      phongMat->specular = glm::vec3(specular.r, specular.g, specular.b) * shininessStrength;
      phongMat->ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
      phongMat->shininess = shininess;

      _mMaterials[primitiveName] = phongMat;
    }
  }

  return primitiveName;
}

std::vector<Texture*> AssetImporter::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const aiScene* scene)
{
  std::vector<Texture*> ret;
  unsigned int numTextures = mat->GetTextureCount(type);
  for (unsigned int i = 0; i < numTextures; i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);

    std::string texPath(str.C_Str());
    auto it = _mTextures.find(texPath);
    Texture* tex = nullptr;

    // can't find tex?
    if (it == _mTextures.end())
    {
      tex = _mResources.textureManager.find(texPath);
      if (!tex)
      {
        if (auto assimpTexture = scene->GetEmbeddedTexture(str.C_Str()))
        {
          //Log.print<Severity::warning>("Found an embedded texture! This is not supported yet!");
          //continue;
          texPath = _mPath + "-[Embedded]-" + texPath;
          TextureData texData(assimpTexture, true);
          tex = _mResources.textureManager.insert(texPath, texData);

          if (!tex)
            Log.print<Severity::warning>("Failed to create embedded texture: ", texPath);
        }
        else
        {
          texPath = _mDirectory + '/' + texPath;
          TextureData texData(texPath, true);
          tex = _mResources.textureManager.insert(texPath, texData);

          if (!tex)
            Log.print<Severity::warning>("Failed to create texture with path: ", texPath);
        }
      }
      _mTextures[texPath] = tex;
    }
    else
    {
      tex = (*it).second;
    }

    ret.push_back(tex);
  }

  return ret;
}

void AssetImporter::cleanupAllResources()
{
  for (auto pair : _mMaterials)
  {
    delete pair.second;
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
  if (!_mRoot) return nullptr;

  Asset* clone = _mRoot->clone();

  if (cloneMaterial) 
  {
    // clone material somehow..?
  }

  return clone;
}