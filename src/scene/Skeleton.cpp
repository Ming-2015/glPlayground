#include "Skeleton.h"


Bone::Bone()
  : Node(), inverseBindPoseTransform(1.f), bindPoseTransform(1.f), boneIndex(0)
{}

Bone::~Bone()
{}

// clone implementation
Bone* Bone::clone() const
{
  Bone* b = new Bone();
  copyTo(b);
  return b;
}

void Bone::copyTo(Cloneable* clone) const
{
  Bone* other = dynamic_cast<Bone*>(clone);
  if (!other)
  {
    Log.print<Severity::warning>("Failed to cast clone to Bone* in clone");
    return;
  }

  other->inverseBindPoseTransform = inverseBindPoseTransform;
  other->bindPoseTransform = bindPoseTransform;
  other->boneIndex = boneIndex;
}

static int binarySearch(double time, const std::vector<double>& allTimes, unsigned int start, unsigned int end)
{
  if (start == end) return start; // no range... only happens if allTimes is size 1
  if (start + 1 == end) return start; // only 1 range!

  // at this point there must be 2 or greater range!
  unsigned int mid = (start + end) / 2;
  double valAtMid = allTimes[mid];

  if (time > valAtMid)
  {
    if (time < allTimes[mid + 1]) return mid;
    return binarySearch(time, allTimes, mid + 1, end);
  }
  
  if (time < valAtMid)
  {
    if (time > allTimes[mid - 1]) return mid - 1;
    return binarySearch(time, allTimes, start, mid - 1);
  }

  return mid;
}

int AnimationBoneData::getIndexFromTimeArray(double time, const std::vector<double>& allTimes)
{
  if (allTimes.size() <= 0) return -1;
  if (time < allTimes[0]) return -1;
  if (time >= allTimes[allTimes.size()-1]) return allTimes.size() - 1;
  return binarySearch(time, allTimes, 0, allTimes.size() - 1);
}

glm::vec3 AnimationBoneData::getTranslation(double time)
{
  if (translations.size() == 0) return glm::vec3(0);

  int idx = getIndexFromTimeArray(time, translationTimes);
  if (idx == -1)
  {
    return translations[0];
  }
  if (idx == translationTimes.size() - 1)
  {
    return translations[idx];
  }

  double startTime = translationTimes[idx];
  double endTime = translationTimes[idx + 1];
  glm::vec3 startPos = translations[idx];
  glm::vec3 endPos = translations[idx + 1];

  float factor = (time - startTime) / (endTime - startTime);
  return startPos * factor + endPos * (1 - factor);
}

glm::quat AnimationBoneData::getRotation(double time)
{
  if (rotations.size() == 0) return glm::quat(0,0,0,1);

  int idx = getIndexFromTimeArray(time, rotationTimes);
  if (idx == -1)
  {
    return rotations[0];
  }
  if (idx == rotationTimes.size() - 1)
  {
    return rotations[idx];
  }

  double startTime = rotationTimes[idx];
  double endTime = rotationTimes[idx + 1];
  glm::quat startRotation = rotations[idx];
  glm::quat endRotation = rotations[idx + 1];

  float factor = (time - startTime) / (endTime - startTime);
  return glm::slerp(startRotation, endRotation, factor);
}

glm::vec3 AnimationBoneData::getScale(double time)
{
  if (scalings.size() == 0) return glm::vec3(1);

  int idx = getIndexFromTimeArray(time, scalingTimes);
  if (idx == -1)
  {
    return scalings[0];
  }
  if (idx == scalingTimes.size() - 1)
  {
    return scalings[idx];
  }

  double startTime = scalingTimes[idx];
  double endTime = scalingTimes[idx + 1];
  glm::vec3 startScaling = scalings[idx];
  glm::vec3 endScaling = scalings[idx + 1];

  float factor = (time - startTime) / (endTime - startTime);
  return startScaling * factor + endScaling * (1 - factor);
}


Animation::Animation()
{}


Animation::~Animation()
{
  for (auto& a : animationData) 
  {
    delete a.second;
  }
  animationData.clear();
}

double Animation::convertSecondsToTicks(double s)
{
  return s * ticksPerSecond;
}

double Animation::convertMillisecondsToTicks(double ms)
{
  return convertSecondsToTicks(ms / 1000.0);
}

Skeleton::Skeleton()
  : inverseGlobalTransform(1.f)
{}

Skeleton::~Skeleton()
{
  delete root;
  root = nullptr;
  bones.clear();

  for (auto& a : animations)
  {
    delete a;
  }
  animations.clear();
}

void Skeleton::setBoneTree(Bone* r)
{
  root = r;
  bones.clear();
  boneMapping.clear();
  parseBone(r);
}

void Skeleton::parseBone(Bone* bone)
{
  bones.push_back(bone);
  boneMapping[bone->name] = bones.size() - 1;
  for (auto& child : bone->getChildren())
  {
    Bone* b = dynamic_cast<Bone*>(child);
    if (!b) {
      Log.print<Severity::warning>("Found a bone child that is not a bone!");
      continue;
    }

    parseBone(b);
  }
}

void Skeleton::addAnimation(Animation* anim)
{
  animations.push_back(anim);
  animationMapping[anim->name] = animations.size() - 1;
}

std::vector<glm::mat4> Skeleton::calcBoneMatrices(const std::string& animName, double time)
{
  int idx = getAnimationIdx(animName);
  if (idx < 0) return std::vector<glm::mat4>();
  return calcBoneMatrices(idx, time);
}

std::vector<glm::mat4> Skeleton::calcBoneMatrices(unsigned int idx, double timeInTicks)
{
  std::vector<glm::mat4> ret;

  Animation* anim = getAnimation(idx);
  if (!anim)
  {
    Log.print<Severity::warning>("Failed to get the animation from index in calcBoneMatrices!");
    throw std::runtime_error("Animation Idx out of bound");
  }

  double boundedTime = timeInTicks - floor(timeInTicks / anim->totalTicks) * anim->totalTicks;

  for (unsigned int i = 0; i < bones.size(); i++) 
  {
    const std::string& name = bones[i]->name;
    auto& it = anim->animationData.find(name);
    if (it == anim->animationData.end())
    {
      bones[i]->setPosition(glm::vec3(0));
      bones[i]->setRotationQuaternion(glm::quat(0, 0, 0, 1));
      bones[i]->setScale(glm::vec3(1));
      continue;
    }

    auto& animData = it->second;
    bones[i]->setPosition(animData->getTranslation(boundedTime));
    bones[i]->setRotationQuaternion(animData->getRotation(boundedTime));
    bones[i]->setScale(animData->getScale(boundedTime));
  }

  root->update(0);
  for (unsigned int i = 0; i < bones.size(); i++)
  {
    glm::mat4 boneMatrix = inverseGlobalTransform
      * bones[i]->getGlobalTransform()
      * bones[i]->inverseBindPoseTransform;

    ret.push_back(boneMatrix);
  }

  return ret;
}

Bone* Skeleton::getBone(const std::string& name) const
{
  int idx = getBoneIdx(name);
  if (idx < 0) return nullptr;
  return bones[idx];
}

Bone* Skeleton::getBone(unsigned int idx) const
{
  if (idx < bones.size()) return bones[idx];
  return nullptr;
}

int Skeleton::getBoneIdx(const std::string& name) const
{
  auto& it = boneMapping.find(name);
  if (it == boneMapping.end())
  {
    return -1;
  }
  return int(it->second);
}

Animation* Skeleton::getAnimation(const std::string& name) const
{
  int idx = getAnimationIdx(name);
  if (idx < 0) return nullptr;
  return animations[idx];
}

Animation* Skeleton::getAnimation(unsigned int idx)
{
  if (idx < animations.size()) return animations[idx];
  return nullptr;
}

int Skeleton::getAnimationIdx(const std::string& name) const
{
  auto& it = animationMapping.find(name);
  if (it == animationMapping.end())
  {
    return -1;
  }
  return int(it->second);
}