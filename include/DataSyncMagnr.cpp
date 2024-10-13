#include "DataSyncMagnr.h"
#include <unordered_set>

DataSyncMagnr::DataSyncMagnr(const std::string &host, const std::string &user,
                             const std::string &pass, const std::string &dbname)
    : db_connector(host, user, pass, dbname) {
  std::cout << "Building Tree ...\n";
  // sync other Data with database
  try {
    std::unique_ptr<sql::ResultSet> res(
        db_connector.executeQuery("SELECT * FROM objects"));

    // Populate nodes map
    std::cout << "Building nods :";
    int number = 0;
    while (res->next()) {
      ++number;
      int id = res->getInt("id");
      std::string name = res->getString("name");
      nodes[id] = std::make_shared<ObjectNode>(
          id, name, std::vector<std::weak_ptr<ObjectNode>>{});
    }
    std::cout << number << "\n";

    // Build tree from relationships
    res = db_connector.executeQuery("SELECT * FROM relationships");
    std::unordered_set<int> children;
    std::cout << "setting relationships ...\n";
    while (res->next()) {
      int child = res->getInt("child_id");
      int parent = res->getInt("parent_id");
      std::cout << "Parent ID: " << parent << ", Child ID: " << child << "\n";
      if (nodes.find(parent) != nodes.end() &&
          nodes.find(child) != nodes.end()) {
        nodes[parent]->pushChild(std::weak_ptr<ObjectNode>(nodes[child]));
        children.insert(child);
      } else {
        std::cerr << "Error: Parent or Child node not found!" << std::endl;
      }
    }

    // Identify root nodes
    for (const auto &pair : nodes) {
      if (children.find(pair.first) == children.end()) {
        roots.push_back(std::weak_ptr<ObjectNode>(pair.second));
      }
    }
  } catch (sql::SQLException &e) {
    std::cerr << "SQLException occurred:\n"
              << "Error Code: " << e.getErrorCode() << "\n"
              << "SQLState: " << e.getSQLState() << "\n"
              << "Message: " << e.what() << std::endl;
  }
}

/*
 * TODO:
 * - Try Catch shit
 * - Check if AUTOCOMMIT is on
 */
bool DataSyncMagnr::addNode(const std::string &name, int parent_id) {

  // make sure AUTOCOMMIT is off
  db_connector.executeUpdate("SET AUTOCOMMIT = 0");

  // Insert the node into the database
  std::string query = "INSERT INTO objects (name) VALUES ('" + name + "')";
  int result = db_connector.executeUpdate(query);
  if (result == -1) {
    db_connector.executeUpdate("ROLLBACK");
    return -1;
  }

  // Retrieve the ID of the new Object
  std::unique_ptr<sql::ResultSet> res =
      db_connector.executeQuery("SELECT LAST_INSERT_ID() AS id");
  if (!res || !res->next()) {
    return -1;
  }
  int new_id = res->getInt("id");

  // add to nodes map
  nodes[new_id] = std::make_shared<ObjectNode>(
      new_id, name, std::vector<std::weak_ptr<ObjectNode>>{});

  // Now update relationships if needed
  if (parent_id != -1) {
    // Insert into relationships table
    std::string query =
        "INSERT INTO relationships (child_id, parent_id) VALUES (" +
        std::to_string(new_id) + ", " + std::to_string(parent_id) + ")";
    int result = db_connector.executeUpdate(query);
    if (result == -1) {
      db_connector.executeUpdate("ROLLBACK");
      return -1;
    }
    // Check if parent node exists in nodes map
    auto parentNodeIt = nodes.find(parent_id);
    if (parentNodeIt != nodes.end() && parentNodeIt->second) {
      // Parent node exists, add new node as child
      parentNodeIt->second->pushChild(std::weak_ptr<ObjectNode>(nodes[new_id]));
    } else {
      // Parent node not found, rollback and return error
      db_connector.executeUpdate("ROLLBACK");
      return -1;
    }
  } else {
    // if no parent, add to roots
    roots.push_back(std::weak_ptr<ObjectNode>(nodes[new_id]));
  }

  // Re-enable auto-commit
  db_connector.executeUpdate("SET AUTOCOMMIT = 1");

  return new_id;
}
