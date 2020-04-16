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

  Asset* clone() const override;
};