#include "database.h"
#include "routes.h"
#include <crow.h>

int main() {
  crow::SimpleApp app;

  // Initialize database and build the tree
  auto [roots, nodes] = buildTreeFromDatabase();

  // Register routes
  setupRoutes(app, roots);

  // Start the server
  app.bindaddr("0.0.0.0").port(18080).run();
}
