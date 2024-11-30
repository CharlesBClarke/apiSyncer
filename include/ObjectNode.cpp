#include "ObjectNode.h"
#include <algorithm>
#include <iostream>
// Constructor
ObjectNode::ObjectNode(uint id, std::string name,
                       std::vector<std::weak_ptr<ObjectNode>> children)
    : id(id), name(std::move(name)), children(std::move(children)) {}

// Getters
int ObjectNode::getID() const { return id; }

const std::string &ObjectNode::getName() const { return name; }

const std::weak_ptr<ObjectNode> &ObjectNode::getParent() const {
  return parent;
}

const std::vector<std::weak_ptr<ObjectNode>> &ObjectNode::getChildren() const {
  return children;
}

// Setters
void ObjectNode::setID(uint id) { this->id = id; }

void ObjectNode::setName(const std::string &name) { this->name = name; }

void ObjectNode::setChildren(std::vector<std::weak_ptr<ObjectNode>> children) {
  this->children = std::move(children);
}

void ObjectNode::setParent(std::weak_ptr<ObjectNode> parent) {
  this->parent = std::move(parent);
}

void ObjectNode::pushChild(std::weak_ptr<ObjectNode> child) {
  children.push_back(std::move(child));
}

void ObjectNode::removeChild(std::shared_ptr<ObjectNode> target) {
  children.erase(
      std::remove_if(children.begin(), children.end(),
                     [&target](const std::weak_ptr<ObjectNode> &wp) {
                       if (auto sp_wp = wp.lock()) {
                         return sp_wp.get() ==
                                target.get(); // Compare raw pointers
                       }
                       // Remove expired weak_ptrs
                       std::cerr << "expired child found in tree and removed";
                       return true;
                     }),
      children.end());
}
