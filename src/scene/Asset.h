#pragma once
#include "./Model.h"

class Asset : public Node {
protected:
  std::map<std::string, Model*> _mModels;
  void copyTo(Cloneable* cloned) const override;

public:

  void addModel(const std::string& name, Model* model, bool addAsChild=false);
  Model* getModel(const std::string& name) const;
  void removeModel(const std::string& name);
  std::map<std::string, Model*> getAllModels() const;

  Skeleton* skeleton = nullptr;
  std::vector<Material*> allMaterials;

  // animation state...
  double currentAnimationMs = 0;
  int currentAnimationIdx = -1;
  bool isAnimationStarted = false;

  Asset* clone() const override;

  virtual void update(float deltaT) override;
  virtual void draw(const glm::mat4& PV) override;
};