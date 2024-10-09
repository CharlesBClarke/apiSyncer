#include "routes.h"

// Convert node to JSON
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

void setupRoutes(crow::SimpleApp &app,
                 const std::vector<std::weak_ptr<ObjectNode>> &roots) {
  CROW_ROUTE(app, "/api/tree").methods(crow::HTTPMethod::GET)([roots]() {
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

  CROW_ROUTE(app, "/api/rootsCount").methods(crow::HTTPMethod::GET)([&roots]() {
    int roots_size = roots.size();
    return crow::response(roots_size, "bad idea");
  });
  CROW_ROUTE(app, "/api/helloWorld").methods(crow::HTTPMethod::GET)([]() {
    return crow::response(200, "Hello World!");
  });
}
