#include "Node.h"

Node::~Node()
{
  // destroy all children
  for (Node* n : _mChildren)
  {
    delete n;
  }
  _mChildren.clear();
}

void Node::draw(const glm::mat4 PV, const glm::mat4& M) const
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
  _mChildren.push_back(n);
  n->_mParentIdx = _mChildren.size() - 1;
  n->_mParent = this;
}

void Node::removeChild(Node* n)
{
  if (_mParentIdx < 0 || _mParentIdx >= _mChildren.size())
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

void Node::setParent(Node* newParent)
{
  if (_mParent) 
  {
    _mParent->removeChild(this);
  }

  newParent->addChild(this);
}

const std::vector<Node*>& Node::getChildren() const
{
  return _mChildren;
}