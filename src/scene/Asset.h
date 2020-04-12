#pragma once
#include "./Model.h"

class Asset : public GameObject {
protected:
  std::map<std::string, Model*> _mModels;
  void copyTo(Cloneable* cloned) const override;

public:

  void addModel(const std::string& name, Model* model, bool addAsChild=false);
  Model* getModel(const std::string& name) const;

  Asset* clone() const override;
};