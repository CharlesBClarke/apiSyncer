#include "ObjectNode.h"

// Constructor
ObjectNode::ObjectNode(int id, std::string name,
                       std::vector<ObjectNode *> children)
    : id(id), name(std::move(name)), children(std::move(children)) {}

// Getters
int ObjectNode::getID() const { return id; }

const std::string &ObjectNode::getName() const { return name; }

const std::vector<ObjectNode *> &ObjectNode::getChildren() const {
  return children;
}

// Setters
void ObjectNode::setID(int id) { this->id = id; }

void ObjectNode::setName(const std::string &name) { this->name = name; }

void ObjectNode::setChildren(std::vector<ObjectNode *> children) {
  this->children = std::move(children);
}

void ObjectNode::pushChild(ObjectNode *child) {
  children.push_back(std::move(child));
}
