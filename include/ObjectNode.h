#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include <memory>
#include <string>
#include <vector>

class ObjectNode {
public:
  // Constructor with parameters
  ObjectNode(int id, std::string name,
             std::vector<std::unique_ptr<ObjectNode>> children);

  // Rule of Five is not necessary since we're using smart pointers
  // Default destructor and copy/move semantics
  ~ObjectNode() = default;

  // Getters
  int getID() const;
  const std::string &getData() const;
  const std::vector<std::unique_ptr<ObjectNode>> &getChildren() const;

  // Setters
  void setID(int id);
  void setName(const std::string &name);
  void setChildren(std::vector<std::unique_ptr<ObjectNode>> children);
  void pushChild(std::unique_ptr<ObjectNode> child);

private:
  int id;
  std::string name;
  std::vector<std::unique_ptr<ObjectNode>> children;
};

#endif // OBJECTNODE_H
