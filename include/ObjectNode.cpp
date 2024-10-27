#include "ObjectNode.h"
#include <algorithm>
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

// Setters
void ObjectNode::setID(int id) { this->id = id; }

void ObjectNode::setName(const std::string &name) { this->name = name; }

void ObjectNode::setChildren(std::vector<std::weak_ptr<ObjectNode>> children) {
  this->children = std::move(children);
}

void ObjectNode::pushChild(std::weak_ptr<ObjectNode> child) {
  children.push_back(std::move(child));
}

void ObjectNode::removeChild(std::weak_ptr<ObjectNode> target) {
  children.erase(
      std::remove_if(
          children.begin(), children.end(),
          [&target](const std::weak_ptr<ObjectNode> &wp) {
            if (auto sp_wp = wp.lock()) {
              if (auto sp_target = target.lock()) {
                return sp_wp ==
                       sp_target; // Remove if pointing to the same object
              }
            }
            // why not
            return wp.expired();
          }),
      children.end());
}
