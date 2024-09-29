#include <crow.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <memory>

// Database credentials
const std::string DB_HOST = "149.28.89.172";
const std::string DB_USER = "Charles";
const std::string DB_PASS =
    "mdeBiINwyCqdjImyhpBivbawJNDcCA"; // hard coded password
const std::string DB_NAME = "invmangBase";

int main() {
  crow::SimpleApp app;

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
