#include "routes.h"
#include "DataSyncMagnr.h"
#include "ObjectNode.h"
#include <iostream>
#include <string>

extern DataSyncMagnr data;

crow::json::wvalue nodeToJson(const ObjectNode &node) {
  std::cout << "Node To Json\n";
  crow::json::wvalue jsonNode;
  jsonNode["id"] = node.getID();
  jsonNode["name"] = node.getName();

  // Recursively add children
  crow::json::wvalue::list children;
  for (const auto &childPtr : node.getChildren()) {
    if (auto lock = childPtr.lock()) {
      children.push_back(nodeToJson(*lock));
    } else {
      std::cerr << "Expired child in tree\n";
    }
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
   * - Try Catch shit
   */
  std::cout << "buildind";

  CROW_ROUTE(app, "/api/add_node")
      .methods(crow::HTTPMethod::POST)([](const crow::request &req) {
        std::cout << "Starting Node to json \n";
        // Parse JSON request body
        auto body = crow::json::load(req.body);

        // Check if the JSON body is valid
        if (!body) {
          return crow::response(400, "Invalid JSON data");
        }

        // Get info
        std::string name = "";
        if (body.has("name") && body["name"].t() != crow::json::type::Null) {
          name = body["name"].s();
        } else {
          return crow::response(500, "Name not provided");
        }

        // Declare parent_id, check if it exists and is not null
        int parent_id = -1;
        if (body.has("parent_id") &&
            body["parent_id"].t() != crow::json::type::Null) {
          parent_id = body["parent_id"].i();
        }

        int new_id = data.addNode(name, parent_id);

        // Return success response with node details
        crow::json::wvalue response;
        response["message"] = "Node added successfully";
        response["id"] = new_id;
        response["name"] = name;

        return crow::response(201, response);
      });

  CROW_ROUTE(app, "/api/remove_node")
      .methods(crow::HTTPMethod::POST)([](const crow::request &req) {
        // Parse JSON request body
        auto body = crow::json::load(req.body);

        // Check if the JSON body is valid
        if (!body) {
          return crow::response(400, "Invalid JSON data");
        }

        // Get info
        int id = std::intmax_t();
        if (body.has("id") && body["id"].t() != crow::json::type::Null) {
          id = body["id"].i();
        } else {
          return crow::response(500, "Name not provided");
        }

        data.removeNode(id);

        // Return success response with node details
        crow::json::wvalue response;
        response["message"] = "Node Remvoed successfully";
        response["id"] = id;

        return crow::response(201, response);
      });

  // Build Tree
  CROW_ROUTE(app, "/api/tree").methods(crow::HTTPMethod::GET)([]() {
    crow::json::wvalue::list json_tree;

    // Lock weak_ptr and check if it's valid
    if (auto lock = data.getSuperRoot().lock()) {
      try {
        json_tree.push_back(nodeToJson(*lock)); // Convert ObjectNode to JSON
      } catch (const std::exception &e) {
        // Handle conversion failure
        return crow::response(500, "Failed to convert node to JSON");
      }
    } else {
      // Handle weak_ptr lock failure
      return crow::response(404, "SuperRoot not found");
    }

    // Build final JSON response
    crow::json::wvalue result;
    result["tree"] = crow::json::wvalue(json_tree);
    return crow::response(200, result);
  });

  CROW_ROUTE(app, "/api/helloWorld").methods(crow::HTTPMethod::GET)([]() {
    return crow::response(200, "Hello World!");
  });
  CROW_ROUTE(app, "/api/dev/verify").methods(crow::HTTPMethod::GET)([]() {
    if (data.validate_tree()) {
      return crow::response(200, "tree good");
    } else {
      return crow::response(200, "tree bad");
    }
  });
}
