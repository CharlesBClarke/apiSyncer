#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include <memory>
#include <string>
#include <vector>

class ObjectNode {
public:
  // Constructor with parameters
  ObjectNode(int id, std::string name, std::vector<ObjectNode *> children);

  // Rule of Five is not necessary since we're using smart pointers
  // Default destructor and copy/move semantics
  ~ObjectNode() = default;

  // Getters
  int getID() const;
  const std::string &getName() const;
  const std::vector<ObjectNode *> &getChildren() const;

  // Setters
  void setID(int id);
  void setName(const std::string &name);
  void setChildren(std::vector<ObjectNode *> children);
  void pushChild(ObjectNode *child);

private:
  int id;
  std::string name;
  std::vector<ObjectNode *> children;
};

#endif // OBJECTNODE_H
