#include "database.h"
#include "MySQLDB.h"
#include "ObjectNode.h"
#include <iostream>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/mysql_driver.h>
#include <unordered_set>

// database shit
extern MySQLDB db_connector;
extern std::unordered_map<int, std::shared_ptr<ObjectNode>> nodes;
extern std::vector<std::weak_ptr<ObjectNode>> roots;

void buildTreeFromDatabase() {
  std::cout << "Building Tree ...\n";

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
