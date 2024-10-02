#include "ObjectNode.h"

// Constructor
ObjectNode::ObjectNode(int id, std::string data,
                       std::vector<std::unique_ptr<ObjectNode>> children)
    : id(id), data(std::move(data)), children(std::move(children)) {}

// Getters
int ObjectNode::getID() const { return id; }

const std::string &ObjectNode::getData() const { return data; }

const std::vector<std::unique_ptr<ObjectNode>> &
ObjectNode::getChildren() const {
  return children;
}

// Setters
void ObjectNode::setID(int id) { this->id = id; }

void ObjectNode::setData(const std::string &data) { this->data = data; }

void ObjectNode::setChildren(
    std::vector<std::unique_ptr<ObjectNode>> children) {
  this->children = std::move(children);
}

void ObjectNode::pushChild(std::unique_ptr<ObjectNode> child) {
  children.push_back(std::move(child));
}
