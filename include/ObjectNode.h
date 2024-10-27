#pragma once

#include <memory>
#include <string>
#include <vector>

class ObjectNode {
public:
  // Constructor with parameters
  ObjectNode(int id, std::string name,
             std::vector<std::weak_ptr<ObjectNode>> children);

  ~ObjectNode() = default;

  // Getters
  int getID() const;
  const std::string &getName() const;
  const std::vector<std::weak_ptr<ObjectNode>> &getChildren() const;

  // Setters
  void setID(int id);
  void setName(const std::string &name);
  void setChildren(std::vector<std::weak_ptr<ObjectNode>> children);
  void pushChild(std::weak_ptr<ObjectNode> child);
  void removeChild(std::weak_ptr<ObjectNode> target);

private:
  int id;
  std::string name;
  std::vector<std::weak_ptr<ObjectNode>> children;
};
