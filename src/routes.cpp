#include "routes.h"

// Convert node to JSON
crow::json::wvalue nodeToJson(const ObjectNode &node) {
  crow::json::wvalue jsonNode;
  jsonNode["id"] = node.getID();
  jsonNode["name"] = node.getName();

  // Recursively add children
  crow::json::wvalue::list children;
  for (const auto &childPtr : node.getChildren()) {
    children.push_back(nodeToJson(*childPtr));
  }
  jsonNode["children"] = std::move(children);
  return jsonNode;
}

void setupRoutes(crow::SimpleApp &app, const std::vector<ObjectNode *> &roots) {
  CROW_ROUTE(app, "/api/tree").methods(crow::HTTPMethod::GET)([roots]() {
    crow::json::wvalue::list json_tree;
    for (auto &root : roots)
      json_tree.push_back(nodeToJson(*root));
    crow::json::wvalue result;
    result["tree"] = std::move(json_tree);
    return crow::response(200, result);
  });

  CROW_ROUTE(app, "/api/helloWorld").methods(crow::HTTPMethod::GET)([]() {
    return crow::response(200, "Hello World!");
  });
}
