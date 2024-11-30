#pragma once

#include <memory>
#include <string>
#include <vector>

class ObjectNode {
public:
  // Constructor with parameters
  ObjectNode(uint id, std::string name,
             std::vector<std::weak_ptr<ObjectNode>> children);

  ~ObjectNode() = default;

  // Getters
  int getID() const;
  const std::string &getName() const;
  const std::weak_ptr<ObjectNode> &getParent() const;
  const std::vector<std::weak_ptr<ObjectNode>> &getChildren() const;

  // Setters
  void setID(uint id);
  void setName(const std::string &name);
  void setChildren(std::vector<std::weak_ptr<ObjectNode>> children);
  void setParent(std::weak_ptr<ObjectNode> parent);
  void pushChild(std::weak_ptr<ObjectNode> child);
  void removeChild(std::shared_ptr<ObjectNode> target);

private:
  uint id;
  std::string name;
  std::weak_ptr<ObjectNode> parent;
  std::vector<std::weak_ptr<ObjectNode>> children;
};
