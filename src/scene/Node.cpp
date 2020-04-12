#include "Node.h"

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

void Node::draw(const glm::mat4& PV, const glm::mat4& M) const
{
  for (Node* n : _mChildren)
  {
    n->draw(PV, M);
  }
}

void Node::update(float deltaT)
{
  for (Node* n : _mChildren)
  {
    n->update(deltaT);
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

Node::Node() {}

void Node::copyTo(Cloneable* cloned) const 
{
  Node* clonedNode = dynamic_cast<Node*>(cloned);

  if (!clonedNode)
  {
    Log.print<Severity::warning>("Failed to cast into Node*");
    return;
  }

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