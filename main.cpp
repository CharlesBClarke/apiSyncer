#include "ObjectNode.h"
#include <crow.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <memory>
#include <string>
#include <unordered_map>

// Database credentials
const std::string DB_HOST = "149.28.89.172";
const std::string DB_USER = "Charles";
const std::string DB_PASS =
    "mdeBiINwyCqdjImyhpBivbawJNDcCA"; // hard coded password
const std::string DB_NAME = "invmangBase";

// convert node to json;
crow::json::wvalue nodeToJson(const ObjectNode &node) {
  crow::json::wvalue jsonNode;
  jsonNode["id"] = node.getID();
  jsonNode["name"] = node.getName();
  // Recursively add children
  crow::json::wvalue::list children = crow::json::wvalue::list();
  for (const auto &childPtr : node.getChildren()) {
    children.push_back(nodeToJson(*childPtr));
  }
  jsonNode["children"] = std::move(children);

  return jsonNode;
}

int main() {
  crow::SimpleApp app;

  // build the tree
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
    crow::json::wvalue result;

    // Iterate through the database results and add to hashmap
    while (res->next()) {
      crow::json::wvalue obj;
      int id = res->getInt("id");
      std::string name = res->getString("name");

      // Create a unique_ptr to ObjectNode and store it in the map
      nodes[id] =
          std::make_unique<ObjectNode>(id, name, std::vector<ObjectNode *>{});
    }

    // Build tree from relationships
    res.reset(stmt->executeQuery("SELECT * FROM relationships"));
    std::unordered_map<int, bool> children;
    while (res->next()) {
      int child = res->getInt("child_id");
      int parent = res->getInt("parent_id");

      // Ensure both parent and child exist before accessing or modifying them
      if (nodes.find(parent) != nodes.end() &&
          nodes.find(child) != nodes.end()) {
        // Move the child to the parent's children vector
        nodes[parent]->pushChild(nodes[child].get());
        children[child] = true;

      } else {
        std::cerr << "Error: Parent or Child node not found!" << std::endl;
      }
    }

    for (const auto &pair : nodes) {
      if (children.find(pair.first) == children.end()) {
        roots.push_back(pair.second.get()); // Get raw pointer
      }
    }

  } catch (sql::SQLException &e) {
    std::cerr << "SQLException occurred:\n"
              << "Error Code: " << e.getErrorCode() << "\n"
              << "SQLState: " << e.getSQLState() << "\n"
              << "Message: " << e.what() << std::endl;
  }

  CROW_ROUTE(app, "/api/tree").methods(crow::HTTPMethod::GET)([roots]() {
    crow::json::wvalue::list json_tree;
    for (auto &root : roots)
      json_tree.push_back(nodeToJson(*root));
    crow::json::wvalue result;
    result["tree"] = std::move(json_tree);
    return crow::response(200, result);
  });

  CROW_ROUTE(app, "/api/helloWord").methods(crow::HTTPMethod::GET)([]() {
    return crow::response(200, std::string("Hello World!"));
  });

  app.bindaddr("0.0.0.0").port(18080).run();
}
