#ifndef DATABASE_H
#define DATABASE_H

#include "ObjectNode.h"
#include <memory>
#include <unordered_map>
#include <vector>
// list of roots, and a map to every node
std::pair<std::vector<std::weak_ptr<ObjectNode>>,
          std::unordered_map<int, std::shared_ptr<ObjectNode>>>
buildTreeFromDatabase();

#endif // DATABASE_H
