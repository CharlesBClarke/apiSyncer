#ifndef DATABASE_H
#define DATABASE_H

#include "ObjectNode.h"
#include <memory>
#include <unordered_map>
#include <vector>

std::pair<std::vector<ObjectNode *>,
          std::unordered_map<int, std::unique_ptr<ObjectNode>>>
buildTreeFromDatabase();

#endif // DATABASE_H
