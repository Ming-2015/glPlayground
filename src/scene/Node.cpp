#include "Node.h"
#include <glm/gtx/matrix_decompose.hpp>

Node::Node(): GameObjectBase(), _mParentGlobalTransform(1.0f), _mGlobalTransformCache(1.0f) {}

Node::~Node()
{
  // make a copy as children will modify _mChildren
  std::vector<Node*> childrenCopy = _mChildren;

  // destroy all children
  for (Node* n : childrenCopy)
  {
    delete n;
  }

  // remove from parent
  if (_mParent) {
    _mParent->removeChild(this);
  }
}

void Node::addChild(Node* n)
{
  if (!n)
  {
    Log.print<Severity::warning>("Trying to add a null child!");
    return;
  }

  if (n->_mParent == this) return;

  if (n->_mParent) 
  {
    n->_mParent->removeChild(n);
  }

  _mChildren.push_back(n);
  n->_mParentIdx = _mChildren.size() - 1;
  n->_mParent = this;
  n->_setParentGlobalTransform(getGlobalTransform());
}

void Node::removeChild(Node* n)
{
  if (!n)
  {
    Log.print<Severity::warning>("Trying to remove a null child!");
    return;
  }

  if (n->_mParentIdx < 0 || n->_mParentIdx >= _mChildren.size())
  {
    Log.print<Severity::warning>("Node's parentIdx is out of bound!");
    return;
  }

  if (_mChildren[n->_mParentIdx] != n)
  {
    Log.print<Severity::warning>("Node's parentIdx is not matching the Node itself!");
    return;
  }

  int idx = n->_mParentIdx;
  _mChildren.erase(_mChildren.begin() + idx);
  n->_mParent = nullptr;
  n->_mParentIdx = -1;
  n->_setParentGlobalTransform(glm::mat4(1.f));

  for (auto it = _mChildren.begin() + idx; it != _mChildren.end(); it++)
  {
    (*it)->_mParentIdx--;
  }
}

Node* Node::getParent() const
{
  return _mParent;
}

Node* Node::getRoot() 
{
  Node* current = this;
  while (current->_mParent) {
    current = current->_mParent;
  }
  return current;
}

void Node::setParent(Node* newParent)
{
  if (newParent == nullptr)
  {
    if (_mParent)
      _mParent->removeChild(this);
    
    return;
  }

  newParent->addChild(this);
}

const std::vector<Node*>& Node::getChildren() const
{
  return _mChildren;
}

void Node::copyTo(Cloneable* cloned) const 
{
  Node* clonedNode = dynamic_cast<Node*>(cloned);
  if (!clonedNode)
  {
    Log.print<Severity::warning>("Failed to cast into Node*");
    return;
  }

  GameObjectBase::_copyTo(clonedNode);
  for (auto child : _mChildren)
  {
    clonedNode->addChild(child->clone());
  }
}

Node* Node::clone() const
{
  // note that the node won't have a parent by default
  Node* node = new Node();
  copyTo(node);
  return node;
}

bool bfs(const Node* current, const Node* target, std::vector<int>& vec)
{
  auto children = current->getChildren();
  for (int i = 0; i < children.size(); i++)
  {
    auto child = children[i];
    if (child == target) 
    {
      vec.push_back(i);
      return true;
    }
  }

  for (int i = 0; i < children.size(); i++)
  {
    auto child = children[i];
    if (bfs(child, target, vec)) 
    {
      vec.push_back(i);
      return true;
    }
  }

  return false;
}

// returns the indices of each parent in the order of child access when node is found. Empty vector if not found
std::vector<int> Node::breadthFirstSearch(const Node* node) const
{
  std::vector<int> ret;
  bfs( const_cast<const Node*>(this), node, ret);
  std::reverse(ret.begin(), ret.end());
  return ret;
}

// get the descendent by keep accessing the child of node
Node* Node::getDescendentByIndices(std::vector<int> indices) const
{
  const Node* current = this;
  for (int depth = 0; depth < indices.size(); depth++)
  {
    current = getChildByIndex(indices[depth]);
    if (!current) break;
  }

  return const_cast<Node *>(current);
}

// get a direct child
Node* Node::getChildByIndex(int idx) const
{
  if (idx < 0 || idx >= _mChildren.size()) return nullptr;
  return _mChildren[idx];
}

Node* _findByName(const std::string& name, Node* current)
{
  if (current->name == name) return current;
  auto children = current->getChildren();
  for (auto child : children)
  {
    Node* found = _findByName(name, child);
    if (found) return found;
  }
  return nullptr;
}

Node* Node::findByName(const std::string& name) const
{
  return _findByName(name, const_cast<Node *>(this));
}

void Node::draw(const glm::mat4& PV)
{
  for (Node* n : _mChildren)
  {
    n->draw(PV);
  }
}

void Node::update(float deltaT)
{
  forceComputeTransform();
  for (auto& n : _mChildren)
    n->update(deltaT);
}

void Node::forceComputeTransform()
{
  if (_mShouldUpdateModelMatrix)
  {
    _updateWorldMatrix();
    _mIsGlobalTransformDirty = true;
    _mShouldUpdateModelMatrix = false;
  }

  if (_mIsGlobalTransformDirty)
  {
    for (auto& child : _mChildren)
      child->_setParentGlobalTransform(getGlobalTransform());
  }
}

const glm::mat4& Node::getGlobalTransform() 
{
  if (_mIsGlobalTransformDirty) 
    _mGlobalTransformCache = _mParentGlobalTransform * _mModelMatrix;
  
  return _mGlobalTransformCache;
}

void Node::_setParentGlobalTransform(const glm::mat4& transform)
{
  _mParentGlobalTransform = transform;
  _mIsGlobalTransformDirty = true;
}

void Node::decomposeGlobalMatrix(glm::vec3& position, glm::quat& rotation, glm::vec3& scale)
{
  auto& matrix = getGlobalTransform();
  glm::vec3 skew;
  glm::vec4 perspective;
  glm::decompose(matrix, scale, rotation, position, skew, perspective);
}

glm::vec3 Node::getAbsolutePosition() 
{
  auto& matrix = getGlobalTransform();
  glm::vec4 pos = matrix * glm::vec4(0, 0, 0, 1.f);
  return glm::vec3(pos);
}