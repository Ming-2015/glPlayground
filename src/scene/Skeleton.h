#pragma once
#include "Node.h"
#include <map>
#include <unordered_map>

// a Bone node... ASSUMES that parent and children are all bones too
class Bone : public Node{
protected:

  virtual void copyTo(Cloneable* cloneable) const override;

public:
  // for transforming to the bone space - a.k.a. offsetMatrix or skinning transform
  glm::mat4 inverseBindPoseTransform;

  // the original bone matrix for a bind pose
  glm::mat4 bindPoseTransform;

  // index in the skeleton
  unsigned int boneIndex;

  // clone implementation
  virtual Bone* clone() const override;

  Bone();
  virtual ~Bone();
};


class AnimationBoneData {
public:
  static int getIndexFromTimeArray(double time, const std::vector<double>& allTimes);

public:
  // note that all times are in ticks...
  std::vector<glm::vec3> translations;
  std::vector<double> translationTimes;

  std::vector<glm::quat> rotations;
  std::vector<double> rotationTimes;

  std::vector<glm::vec3> scalings;
  std::vector<double> scalingTimes;

  glm::vec3 getTranslation(double time);
  glm::quat getRotation(double time);
  glm::vec3 getScale(double time);
};


// each animation has a list of animation data, each corresponds to a bone
class Animation {
public:
  std::string name;

  // number of ticks in this animation
  double totalTicks = 0;

  // ticks per second in this animation
  double ticksPerSecond = 0;

  double getDurationInSeconds() { return totalTicks / ticksPerSecond; }
  double convertMillisecondsToTicks(double ms);
  double convertSecondsToTicks(double s);

  // all the animation data...
  std::unordered_map<std::string, AnimationBoneData *> animationData;

  Animation();
  virtual ~Animation();
};


class Skeleton {
protected:
  // each node acts as a bone...
  std::vector<Bone*> bones;
  std::map<std::string, unsigned int> boneMapping;
  Bone* root = nullptr;

  std::vector<Animation*> animations;
  std::map<std::string, unsigned int> animationMapping;

  void parseBone(Bone* bone);

public:
  Skeleton();
  virtual ~Skeleton();

  void setBoneTree(Bone* root);
  unsigned int getNumBones() const { return bones.size(); }

  // create a new animation for this skeleton...
  void addAnimation(Animation* anim);

  glm::mat4 inverseGlobalTransform;
  std::vector<glm::mat4> calcBoneMatrices(const std::string& animName, double timeInTicks);
  std::vector<glm::mat4> calcBoneMatrices(unsigned int idx, double timeInTicks);

  Bone* getBone(const std::string& name) const;
  Bone* getBone(unsigned int idx) const;
  int getBoneIdx(const std::string& name) const;

  Animation* getAnimation(const std::string& name) const;
  Animation* getAnimation(unsigned int idx);
  int getAnimationIdx(const std::string& name) const;
};