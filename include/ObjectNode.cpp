#include "ObjectNode.h"

// Constructor
ObjectNode::ObjectNode(int id, std::string name,
                       std::vector<std::weak_ptr<ObjectNode>> children)
    : id(id), name(std::move(name)), children(std::move(children)) {}

// Getters
int ObjectNode::getID() const { return id; }

const std::string &ObjectNode::getName() const { return name; }

const std::vector<std::weak_ptr<ObjectNode>> &ObjectNode::getChildren() const {
  return children;
}

const std::weak_ptr<ObjectNode> &ObjectNode::getParent() const {
  return parent;
}

// Setters
void ObjectNode::setID(int id) { this->id = id; }

void ObjectNode::setName(const std::string &name) { this->name = name; }

void ObjectNode::setChildren(std::vector<std::weak_ptr<ObjectNode>> children) {
  this->children = std::move(children);
}

void ObjectNode::pushChild(std::weak_ptr<ObjectNode> child) {
  children.push_back(std::move(child));
}
void ObjectNode::setParent(std::weak_ptr<ObjectNode> parent) {
  this->parent = parent;
}
