#include "DataSyncMagnr.h"
#include "routes.h"
#include <crow.h>
#include <iostream>

// Database credentials
const char *DB_HOST = std::getenv("DB_HOST");
const char *DB_USER = std::getenv("DB_USER");
const char *DB_PASS = std::getenv("DB_PASSWORD");
const char *DB_NAME = std::getenv("DB_NAME");

DataSyncMagnr data{std::string(DB_HOST), std::string(DB_USER),
                   std::string(DB_PASS), std::string(DB_NAME)};
int main() {
  crow::SimpleApp app;
  if (data.hasConnected()) {
    // Register routes
    setupRoutes(app);

    // Start the server
    app.bindaddr("0.0.0.0").port(18080).run();
  } else {
    std::cerr << "server error";
  }
}
