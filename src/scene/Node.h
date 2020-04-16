#pragma once
#include "../utils/Cloneable.hpp"
#include "../utils/Logger.h"
#include "./GameObject.h"
#include <glm/glm.hpp>
#include <vector>

class Node : public Cloneable, public GameObjectBase
{
private:
  // parent of the node
  Node* _mParent = nullptr;

  // which index am I in the parent? -1 means no parent
  int _mParentIdx = -1;

  // all my own children
  std::vector<Node*> _mChildren;

protected:
  virtual void copyTo(Cloneable* cloned) const override;

  // a cache of the parent's transform in the previous draw call
  glm::mat4 _mParentGlobalTransform;
  glm::mat4 _mGlobalTransformCache;
  bool _mIsGlobalTransformDirty = true;

  void _setParentGlobalTransform(const glm::mat4& transform);

public:
  // an optional, public name, for convenience
  std::string name;

public:
  Node();
  virtual ~Node();

  // these should be inherited AND called from super class
  virtual void draw(const glm::mat4& PV);
  virtual void update(float deltaT);

  // these should not be modified by super class
  void addChild(Node* n);
  void removeChild(Node* n);
  Node* getParent() const;
  void setParent(Node * n);
  Node* getRoot();
  const std::vector<Node*>& getChildren() const;

  // find a node based on its name
  Node* findByName(const std::string& name) const;

  // returns the indices of each parent in the order of child access when node is found. Empty vector if not found
  // NOTE: does not check against itself, so that has to be done manually
  std::vector<int> breadthFirstSearch(const Node* node) const;

  // get the descendent by keep accessing the child of node
  Node* getDescendentByIndices(std::vector<int> indices) const;

  // get a direct child
  Node* getChildByIndex(int idx) const;

  // clone implementation
  virtual Node* clone() const override;

  // get the absolute position of the node. Not guaranteed to be sync-ed to current frame unless forceComputeTransform is called.
  const glm::mat4& getGlobalTransform();

  // force recompute current transform - as well as parents
  void forceComputeTransform();

  // decompose global transform
  void decomposeGlobalMatrix(glm::vec3& position, glm::quat& rotation, glm::vec3& scale);
  glm::vec3 getAbsolutePosition();
};