#include "MySQLDB.h"
#include <iostream>
#include <jdbc/cppconn/exception.h>

// Constructor
MySQLDB::MySQLDB(const std::string &host, const std::string &user,
                 const std::string &password, const std::string &database)
    : host(host), user(user), password(password), database(database),
      driver(nullptr), connection(nullptr) {}

// Destructor to ensure the connection is closed
MySQLDB::~MySQLDB() { disconnect(); }

// Connect to the database
bool MySQLDB::connect() {
  try {
    driver = sql::mysql::get_mysql_driver_instance();
    connection = std::unique_ptr<sql::Connection>(
        driver->connect("tcp://" + host + ":3306", user, password));
    connection->setSchema(database); // Use the database
    std::cout << "Connected to MySQL database." << std::endl;
    return true;
  } catch (sql::SQLException &e) {
    std::cerr << "SQLException: " << e.what() << std::endl;
    std::cerr << "MySQL Error Code: " << e.getErrorCode() << std::endl;
    std::cerr << "SQLState: " << e.getSQLState() << std::endl;
    return false;
  }
}

// Disconnect from the database
void MySQLDB::disconnect() {
  if (connection) {
    connection->close();
    std::cout << "Disconnected from MySQL database." << std::endl;
  }
}

// Execute a query and return the result set (SELECT queries)
std::unique_ptr<sql::ResultSet>
MySQLDB::executeQuery(const std::string &query) {
  try {
    std::unique_ptr<sql::Statement> stmt(connection->createStatement());
    return std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));
  } catch (sql::SQLException &e) {
    std::cerr << "SQLException in executeQuery: " << e.what() << std::endl;
    return nullptr;
  }
}

// Execute an update (INSERT, UPDATE, DELETE) and return the affected rows
int MySQLDB::executeUpdate(const std::string &query) {
  try {
    std::unique_ptr<sql::Statement> stmt(connection->createStatement());
    return stmt->executeUpdate(query); // Returns the number of affected rows
  } catch (sql::SQLException &e) {
    std::cerr << "SQLException in executeUpdate: " << e.what() << std::endl;
    return -1;
  }
}
