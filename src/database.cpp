#include "database.h"
#include <iostream>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/mysql_driver.h>

// Database credentials
const std::string DB_HOST = "149.28.89.172";
const std::string DB_USER = "Charles";
const std::string DB_PASS = "mdeBiINwyCqdjImyhpBivbawJNDcCA";
const std::string DB_NAME = "invmangBase";

std::pair<std::vector<ObjectNode *>,
          std::unordered_map<int, std::unique_ptr<ObjectNode>>>
buildTreeFromDatabase() {
  std::unordered_map<int, std::unique_ptr<ObjectNode>> nodes;
  std::vector<ObjectNode *> roots;

  try {
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> con;

    driver = sql::mysql::get_mysql_driver_instance();
    con.reset(driver->connect("tcp://" + DB_HOST + ":3306", DB_USER, DB_PASS));
    con->setSchema(DB_NAME);

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT * FROM objects"));

    // Populate nodes map
    while (res->next()) {
      int id = res->getInt("id");
      std::string name = res->getString("name");
      nodes[id] =
          std::make_unique<ObjectNode>(id, name, std::vector<ObjectNode *>{});
    }

    // Build tree from relationships
    res.reset(stmt->executeQuery("SELECT * FROM relationships"));
    std::unordered_map<int, bool> children;
    while (res->next()) {
      int child = res->getInt("child_id");
      int parent = res->getInt("parent_id");

      if (nodes.find(parent) != nodes.end() &&
          nodes.find(child) != nodes.end()) {
        nodes[parent]->pushChild(nodes[child].get());
        children[child] = true;
      } else {
        std::cerr << "Error: Parent or Child node not found!" << std::endl;
      }
    }

    // Identify root nodes
    for (const auto &pair : nodes) {
      if (children.find(pair.first) == children.end()) {
        roots.push_back(pair.second.get());
      }
    }
  } catch (sql::SQLException &e) {
    std::cerr << "SQLException occurred:\n"
              << "Error Code: " << e.getErrorCode() << "\n"
              << "SQLState: " << e.getSQLState() << "\n"
              << "Message: " << e.what() << std::endl;
  }

  return {roots, std::move(nodes)};
}
