#include "ObjectNode.h"

// Constructor
ObjectNode::ObjectNode(int id, std::string name,
                       std::vector<std::unique_ptr<ObjectNode>> children)
    : id(id), name(std::move(name)), children(std::move(children)) {}

// Getters
int ObjectNode::getID() const { return id; }

const std::string &ObjectNode::getData() const { return name; }

const std::vector<std::unique_ptr<ObjectNode>> &
ObjectNode::getChildren() const {
  return children;
}

// Setters
void ObjectNode::setID(int id) { this->id = id; }

void ObjectNode::setName(const std::string &name) { this->name = name; }

void ObjectNode::setChildren(
    std::vector<std::unique_ptr<ObjectNode>> children) {
  this->children = std::move(children);
}

void ObjectNode::pushChild(std::unique_ptr<ObjectNode> child) {
  children.push_back(std::move(child));
}
