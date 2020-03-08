#pragma once
#include "../utils/Logger.h"
#include <glm/glm.hpp>
#include <vector>

class Node
{
private:
  // parent of the node
  Node* _mParent = nullptr;

  // which index am I in the parent? -1 means no parent
  int _mParentIdx = -1;

  // all my own children
  std::vector<Node*> _mChildren;

public:

  virtual ~Node();

  // these should be inherited AND called from super class
  virtual void draw(const glm::mat4& PV, const glm::mat4& M) const;
  virtual void update(float deltaT);

  // these should not be modified by super class
  void addChild(Node* n);
  void removeChild(Node* n);

  Node* getParent() const;
  void setParent(Node * n);

  const std::vector<Node*>& getChildren() const;
};