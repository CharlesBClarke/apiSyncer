#include "../mysql/include/jdbc/cppconn/resultset.h"
#include "../mysql/include/jdbc/cppconn/statement.h"
#include "../mysql/include/jdbc/mysql_connection.h"
#include "../mysql/include/jdbc/mysql_driver.h"
#include <iostream>

int main() {
  try {
    // Create a connection to the MySQL server
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "your_username",
                          "your_password");

    // Connect to your database
    con->setSchema("your_database_name");

    // Create a statement
    sql::Statement *stmt;
    stmt = con->createStatement();

    // Execute a query
    sql::ResultSet *res;
    res = stmt->executeQuery("SELECT 'Hello, World!' AS _message");

    // Fetch and display the result
    while (res->next()) {
      std::cout << "MySQL says: " << res->getString("_message") << std::endl;
    }

    // Clean up
    delete res;
    delete stmt;
    delete con;
  } catch (sql::SQLException &e) {
    std::cerr << "SQL Error: " << e.what() << std::endl;
  }

  return 0;
}
