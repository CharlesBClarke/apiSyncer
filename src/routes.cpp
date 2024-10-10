#include "routes.h"
#include "MySQLDB.h"
#include "ObjectNode.h"

extern std::unordered_map<int, std::shared_ptr<ObjectNode>> nodes;
extern std::vector<std::weak_ptr<ObjectNode>> roots;
extern MySQLDB db_connector;

crow::json::wvalue nodeToJson(const ObjectNode &node) {
  crow::json::wvalue jsonNode;
  jsonNode["id"] = node.getID();
  jsonNode["name"] = node.getName();

  // Recursively add children
  crow::json::wvalue::list children;
  for (const auto &childPtr : node.getChildren()) {
    std::shared_ptr lock{childPtr.lock()};
    children.push_back(nodeToJson(*lock));
  }
  jsonNode["children"] = std::move(children);
  return jsonNode;
}

void setupRoutes(crow::SimpleApp &app) {
  /*
   * This implementation uses a tree structure combined with an
   * `std::unordered_map<int, std::shared_ptr<ObjectNode>>` for managing nodes.
   *
   * - The map allows fast lookups by node ID.
   * - The tree structure handles parent-child relationships.
   *
   * While functional, this dual approach introduces redundancy and complexity.
   *
   * TODO:
   * - Refactor to integrate the map and tree more efficiently.
   * - Consider a database-backed structure for scalability and better hierarchy
   * management.
   */
  CROW_ROUTE(app, "/api/add_node")
      .methods(crow::HTTPMethod::POST)([](const crow::request &req) {
        // Parse JSON request body
        auto body = crow::json::load(req.body);

        // Check if the JSON body is valid
        if (!body) {
          return crow::response(400, "Invalid JSON data");
        }

        // Get info
        std::string serial = body["serial"].s();
        std::string name = body["name"].s();

        // Insert the node into the database
        std::string query = "INSERT INTO objects (serial, name) VALUES ('" +
                            serial + "', '" + name + "')";
        int result = db_connector.executeUpdate(query);

        if (result == -1) {
          return crow::response(500, "Database error: failed to insert node");
        }

        // Retrieve the auto-incremented ID
        std::unique_ptr<sql::ResultSet> res =
            db_connector.executeQuery("SELECT LAST_INSERT_ID() AS id");
        if (!res || !res->next()) {
          return crow::response(
              500, "Database error: failed to retrieve new node ID");
        }
        int new_id = res->getInt("id");

        // I am going to clean this shit up in the futre
        nodes[new_id] = std::make_shared<ObjectNode>(
            new_id, name, std::vector<std::weak_ptr<ObjectNode>>{});

        // Return success response with node details
        crow::json::wvalue response;
        response["message"] = "Node added successfully";
        response["id"] = new_id;
        response["serial"] = serial;
        response["name"] = name;

        return crow::response(201, response); // 201: Created
      });

  // Build Tree
  CROW_ROUTE(app, "/api/tree").methods(crow::HTTPMethod::GET)([]() {
    crow::json::wvalue::list json_tree;

    for (const auto &root : roots) {
      if (auto lock = root.lock()) { // Lock weak_ptr and check if it's valid
        json_tree.push_back(nodeToJson(
            *lock)); // Assuming nodeToJson converts ObjectNode to JSON
      }
    }

    crow::json::wvalue result;
    result["tree"] = std::move(json_tree);
    return crow::response(200, result);
  });

  CROW_ROUTE(app, "/api/helloWorld").methods(crow::HTTPMethod::GET)([]() {
    return crow::response(200, "Hello World!");
  });
}
