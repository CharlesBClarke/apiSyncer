#include "database.h"
#include <iostream>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/mysql_driver.h>
#include <unordered_set>
// Database credentials
const char *DB_HOST = std::getenv("DB_HOST");
const char *DB_USER = std::getenv("DB_USER");
const char *DB_PASS = std::getenv("DB_PASSWORD");
const char *DB_NAME = std::getenv("DB_NAME");

std::pair<std::vector<std::weak_ptr<ObjectNode>>,
          std::unordered_map<int, std::shared_ptr<ObjectNode>>>
buildTreeFromDatabase() {
  std::unordered_map<int, std::shared_ptr<ObjectNode>> nodes;
  std::vector<std::weak_ptr<ObjectNode>> roots;

  try {
    sql::mysql::MySQL_Driver *driver;
    std::unique_ptr<sql::Connection> con;

    driver = sql::mysql::get_mysql_driver_instance();
    con.reset(driver->connect("tcp://" + std::string(DB_HOST) + ":3306",
                              std::string(DB_USER), std::string(DB_PASS)));
    con->setSchema(DB_NAME);

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT * FROM objects"));

    // Populate nodes map
    while (res->next()) {
      int id = res->getInt("id");
      std::string name = res->getString("name");
      nodes[id] = std::make_shared<ObjectNode>(
          id, name, std::vector<std::weak_ptr<ObjectNode>>{});
    }

    // Build tree from relationships
    res.reset(stmt->executeQuery("SELECT * FROM relationships"));
    std::unordered_set<int> children;
    while (res->next()) {
      int child = res->getInt("child_id");
      int parent = res->getInt("parent_id");

      if (nodes.find(parent) != nodes.end() &&
          nodes.find(child) != nodes.end()) {
        nodes[parent]->pushChild(std::weak_ptr<ObjectNode>(nodes[child]));
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

  return {roots, std::move(nodes)};
}
