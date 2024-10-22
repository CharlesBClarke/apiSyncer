#ifndef DATASYNCMAGNR_H
#define DATASYNCMAGNR_H

#include "MySQLDB.h"
#include "ObjectNode.h"
#include <unordered_map>

// Template class DataSyncMagnr declaration
class DataSyncMagnr {
private:
  // Data Structs
  std::unordered_map<int, std::shared_ptr<ObjectNode>> nodes;
  std::vector<std::weak_ptr<ObjectNode>> roots;
  MySQLDB db_connector;
  bool connection;

public:
  DataSyncMagnr(const std::string &host, const std::string &user,
                const std::string &pass, const std::string &dbname);
  // Accessors
  bool hasConnected();
  // Add this accessor
  const std::vector<std::weak_ptr<ObjectNode>> &getRoots() const;
  int addNode(const std::string &name, int parent_id = -1);
  void removeNode(int id);
};

#endif // DATASYNCMAGNR_H
