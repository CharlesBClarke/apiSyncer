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

int main() {
  crow::SimpleApp app;
  // build the tree
  std::vector<std::unique_ptr<ObjectNode>> roots;
  std::unordered_map<int, std::unique_ptr<ObjectNode>> nodes;
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
      obj["serial"] = res->getInt("serial");
      obj["name"] = res->getString("name");

      // Create a unique_ptr to ObjectNode and store it in the map
      nodes[id] = std::make_unique<ObjectNode>(
          id, obj.dump(), std::vector<std::unique_ptr<ObjectNode>>{});
    }

    // now build the tree using the relationships table
    res.reset(stmt->executeQuery("SELECT * FROM relationships"));
    while (res->next()) {
      int child = res->getInt("child_id");
      int parent = res->getInt("parent_id");
      nodes[parent]->pushChild(std::move(nodes[res->getInt(child)]));
      nodes.erase(child);
    }
    // now all nodes left in the map are roots
    for (auto &pair : nodes) {
      roots.push_back(std::move(pair.second));
    }
    nodes.clear();

  } catch (sql::SQLException &e) {
    std::cerr << "SQLException occurred:\n"
              << "Error Code: " << e.getErrorCode() << "\n"
              << "SQLState: " << e.getSQLState() << "\n"
              << "Message: " << e.what() << std::endl;
  }

  CROW_ROUTE(app, "/api/helloWord").methods(crow::HTTPMethod::GET)([]() {
    return crow::response(200, std::string("Hello World!"));
  });
  CROW_ROUTE(app, "/api/relationships").methods(crow::HTTPMethod::GET)([]() {
    try {
      sql::mysql::MySQL_Driver *driver;
      std::unique_ptr<sql::Connection> con;

      driver = sql::mysql::get_mysql_driver_instance();
      con.reset(
          driver->connect("tcp://" + DB_HOST + ":3306", DB_USER, DB_PASS));
      con->setSchema(DB_NAME);

      std::unique_ptr<sql::Statement> stmt(con->createStatement());
      std::unique_ptr<sql::ResultSet> res(
          stmt->executeQuery("SELECT parent_id, child_id FROM relationships"));

      crow::json::wvalue result;
      crow::json::wvalue::list relationships_list;

      // Iterate through the database results and construct the JSON list
      while (res->next()) {
        crow::json::wvalue obj;
        obj["parent_id"] = res->getInt("parent_id");
        obj["child_id"] = res->getInt("child_id");
        relationships_list.emplace_back(
            std::move(obj)); // Add each object to the list
      }

      result["relationships"] =
          std::move(relationships_list); // Move the list into the result

      return crow::response(200, result);
    } catch (sql::SQLException &e) {
      return crow::response(500, std::string("Error fetching relationships: ") +
                                     e.what());
    }
  });

  app.bindaddr("0.0.0.0").port(18080).run();
}
