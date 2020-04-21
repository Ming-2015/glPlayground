#include "AssetImporter.h"
#include "../utils/Logger.h"
#include "VertexBoneData.h"
#include <unordered_set>

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

void AssetImporter::load(unsigned int flags)
{
  Log.print<Severity::info>("Start parsing asset ", _mPath);

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
    _mPath, 
    aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | flags
  );

  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) 
  {
    Log.print<Severity::error>("!! Assimp failed to load file: ", _mPath);
    Log.print<Severity::error>("!! Assimp Error Message: ", importer.GetErrorString());
    return;
  }

  _mRoot = new Asset();
  processBones(scene);
  processNode(scene->mRootNode, scene, _mRoot);

  if (_mSkeleton)
  {
    _mRoot->skeleton = _mSkeleton;
    for (auto& it : _mMaterials)
    {
      _mRoot->allMaterials.push_back(it.second);
    }
  }
}

glm::mat4 aiMatrixToGlm(aiMatrix4x4 mat)
{
  glm::mat4 ret;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      ret[j][i] = mat[i][j];
    }
  }

  return ret;
}

void processBoneNode(aiNode* node, Bone* parent, const std::unordered_map<std::string, Bone*>& allBones)
{
  auto& it = allBones.find(node->mName.C_Str());
  Bone* current = parent;
  if (it != allBones.end())
  {
    current = it->second;
    current->setParent(parent);
    current->bindPoseTransform = aiMatrixToGlm(node->mTransformation);
  }

  for (int i = 0; i < node->mNumChildren; i++)
  {
    processBoneNode(node->mChildren[i], current, allBones);
  }
}

void AssetImporter::processBones(const aiScene* scene)
{
  std::unordered_map<std::string, Bone*> allBones;
  for (int i = 0; i < scene->mNumMeshes; i++)
  {
    auto& mesh = scene->mMeshes[i];
    for (int b = 0; b < mesh->mNumBones; b++)
    {
      auto& bone = mesh->mBones[b];
      auto it = allBones.find(bone->mName.C_Str());
      if (it == allBones.end())
      {
        Bone* myBone = new Bone();
        myBone->inverseBindPoseTransform = aiMatrixToGlm(bone->mOffsetMatrix);
        myBone->name = bone->mName.C_Str();
        allBones[myBone->name] = myBone;
      }
    }
  }

  if (allBones.size() == 0) return;

  processBoneNode(scene->mRootNode, nullptr, allBones);
  std::vector<Bone*> rootBones;
  for (auto& it : allBones)
  {
    if (it.second->getParent() == nullptr) rootBones.push_back(it.second);
  }

  Bone* root = nullptr;
  if (rootBones.size() > 1) 
  {
    Log.print<Severity::warning>("Found skeleton with multiple root bones!");

    root = new Bone();
    root->name = _mPath + ":joint_root";
    root->inverseBindPoseTransform = glm::mat4(1.f);
    root->bindPoseTransform = glm::mat4(1.f);

    for (auto& b : rootBones) root->addChild(b);
  }
  
  else if (rootBones.size() == 1)
  {
    root = rootBones[0];
  }
  else
  {
    Log.print<Severity::warning>("Failed to build bone tree!");
    return;
  }

  _mSkeleton = new Skeleton();
  _mSkeleton->setBoneTree(root);
  _mSkeleton->inverseGlobalTransform = glm::inverse(aiMatrixToGlm( scene->mRootNode->mTransformation ));

  processAnimations(scene);
}

void AssetImporter::processAnimations(const aiScene* scene)
{
  for (int i = 0; i < scene->mNumAnimations; i++)
  {
    aiAnimation* assimpAnimation = scene->mAnimations[i];
    Animation* anim = new Animation();
    _mSkeleton->addAnimation(anim);

    anim->totalTicks = assimpAnimation->mDuration;
    anim->ticksPerSecond = assimpAnimation->mTicksPerSecond;

    for (int j = 0; j < assimpAnimation->mNumChannels; j++)
    {
      aiNodeAnim* animNode = assimpAnimation->mChannels[j];

      AnimationBoneData* boneData = new AnimationBoneData();
      anim->animationData[animNode->mNodeName.C_Str()] = boneData;

      for (int np = 0; np < animNode->mNumPositionKeys; np++)
      {
        auto posKey = animNode->mPositionKeys[np];
        boneData->translations.push_back(glm::vec3(posKey.mValue.x, posKey.mValue.y, posKey.mValue.z));
        boneData->translationTimes.push_back(posKey.mTime);
      }

      for (int nr = 0; nr < animNode->mNumRotationKeys; nr++)
      {
        auto rotKey = animNode->mRotationKeys[nr];
        boneData->rotations.push_back(glm::quat(rotKey.mValue.w, rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z));
        boneData->rotationTimes.push_back(rotKey.mTime);
      }

      for (int ns = 0; ns < animNode->mNumScalingKeys; ns++)
      {
        auto scaKey = animNode->mScalingKeys[ns];
        boneData->scalings.push_back(glm::vec3(scaKey.mValue.x, scaKey.mValue.y, scaKey.mValue.z));
        boneData->scalingTimes.push_back(scaKey.mTime);
      }
    }
  }
}

void AssetImporter::processNode(aiNode* node, const aiScene* scene, Asset* assetNode)
{
  // transformation
  aiVector3D position, scaling;
  aiQuaternion rotation;
  node->mTransformation.Decompose(scaling, rotation, position);
  assetNode->setPosition(glm::vec3(position.x, position.y, position.z));
  assetNode->setRotationQuaternion(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
  assetNode->setScale(glm::vec3(scaling.x, scaling.y, scaling.z));
  assetNode->name = node->mName.C_Str();

  Log.print<Severity::debug>("Processing node: ", assetNode->name);

  // meshes
  for (unsigned int i = 0; i < node->mNumMeshes; i++) 
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    std::string primitiveName = processMesh(mesh, scene, assetNode);

    Model* model = new Model(_mPrimitives[primitiveName]);
    auto it = _mMaterials.find(primitiveName);
    if (it != _mMaterials.end())
      model->material = (*it).second;
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
    if (!primitive) primitive = createPrimitiveFromAiMesh(primitiveName, mesh, scene);
    _mPrimitives[primitiveName] = primitive;
  }

  if (mesh->mMaterialIndex >= 0)
  {
    if (_mMaterials.find(primitiveName) == _mMaterials.end())
    {
      Material* mat = createMaterialFromAiMesh(mesh, scene);
      _mMaterials[primitiveName] = mat;
    }
  }

  return primitiveName;
}

Primitive* AssetImporter::createPrimitiveFromAiMesh(const std::string& name, aiMesh* mesh, const aiScene* scene)
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

  bool hasNormals = mesh->HasNormals();
  bool hasTangents = mesh->HasTangentsAndBitangents();
  bool hasBitangents = hasTangents;
  bool hasBones = mesh->HasBones();

  // process multi tex coords
  std::vector< std::vector<float>* > texCoords;
  texCoords.push_back(&data.texCoords);
  texCoords.push_back(&data.texCoords_2);
  texCoords.push_back(&data.texCoords_3);

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
      data.bitangents.push_back(bitangent.x);
      data.bitangents.push_back(bitangent.y);
      data.bitangents.push_back(bitangent.z);
    }

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

  if (hasBones && _mSkeleton != nullptr)
  {
    std::vector<VertexBoneData> boneData(mesh->mNumVertices, VertexBoneData());

    for (int i = 0; i < mesh->mNumBones; i++)
    {
      aiBone* bone = mesh->mBones[i];
      std::string name = bone->mName.C_Str();

      int boneId = _mSkeleton->getBoneIdx(name);
      if (boneId < 0) 
      {
        Log.print<Severity::warning>("Failed to retrieve bone ID from the skeleton");
        continue;
      }

      // for each vertex weight...
      for (int j = 0; j < bone->mNumWeights; j++)
      {
        aiVertexWeight vw = bone->mWeights[j];
        boneData[vw.mVertexId].addBoneData(boneId, vw.mWeight);
      }
    }

    for (int i = 0; i < boneData.size(); i++)
    {
      boneData[i].normalizeBoneWeights();

      glm::uvec4 boneIds = boneData[i].getBoneIds();
      glm::vec4 weights = boneData[i].getWeights();
      for (int j = 0; j < 4; j++) 
      {
        data.joints.push_back(boneIds[j]);
        data.weights.push_back(weights[j]);
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

  return _mResources.primitiveManager.insert(name, data);
}

Material* AssetImporter::createMaterialFromAiMesh(aiMesh* mesh, const aiScene* scene)
{
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
  auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, scene);
  auto ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, scene);

  PhongMaterial* phongMat = new PhongMaterial(&_mResources.shaderProgramManager);
  if (diffuseMaps.size() > 0)
  {
    phongMat->diffuseTex = diffuseMaps[0];
    phongMat->ambientTex = diffuseMaps[0];

    if (diffuseMaps.size() > 1)
      Log.print<Severity::warning>("Multiple diffuse maps is not supported!");
  }

  if (specularMaps.size() > 0)
  {
    phongMat->specularTex = specularMaps[0];
    if (specularMaps.size() > 1)
      Log.print <Severity::warning>("Multiple specular maps is not supported!");
  }

  if (ambientMaps.size() > 0)
  {
    phongMat->ambientTex = ambientMaps[0];
    if (ambientMaps.size() > 1)
      Log.print<Severity::warning>("Multiple ambient maps is not supported!");
  }

  aiString name;
  material->Get(AI_MATKEY_NAME, name);

  aiColor3D diffuse(1, 1, 1);
  material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

  // not all materials have specular and ambient!
  aiColor3D specular(1, 1, 1);
  material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

  aiColor3D ambient(1, 1, 1);
  material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
  if (ambient.r == 0 && ambient.g == 0 && ambient.b == 0) {
    ambient = diffuse;
  }

  float shininess = 0.f;
  material->Get(AI_MATKEY_SHININESS, shininess);

  float shininessStrength = 1.f;
  material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);

  int blendMode;
  material->Get(AI_MATKEY_BLEND_FUNC, blendMode);

  phongMat->diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, 1.f);
  phongMat->specular = glm::vec4(specular.r, specular.g, specular.b, 0) * shininessStrength;
  phongMat->specular.a = 1.f;
  phongMat->ambient = glm::vec4(ambient.r, ambient.g, ambient.b, 1.f);
  phongMat->shininess = shininess;
  phongMat->name = name.C_Str();

  if (blendMode == aiBlendMode::aiBlendMode_Default) {
    phongMat->alphaCutoff = 0;
    phongMat->useAlphaBlending = true;
  }
  else {
    phongMat->alphaCutoff = 0.5f;
    phongMat->useAlphaBlending = false;
  }

  return phongMat;
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