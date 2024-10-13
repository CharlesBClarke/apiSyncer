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

public:
  DataSyncMagnr(const std::string &host, const std::string &user,
                const std::string &pass, const std::string &dbname);
  ~DataSyncMagnr();
  //
  bool addNode(const std::string &name, int parent_id = -1);
};

#endif // DATASYNCMAGNR_H
