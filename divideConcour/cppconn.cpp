#include <iostream>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <memory>

int main() {
  try {
    // Initialize the driver
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();

    // Create a connection
    std::unique_ptr<sql::Connection> conn(driver->connect(
        "tcp://127.0.0.1:3306", // Replace with your server address if different
        "your_username",        // Replace with your MySQL username
        "your_password"         // Replace with your MySQL password
        ));

    // Set the active database schema
    conn->setSchema("test"); // Replace with your database name if different

    // Create a new statement
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());

    // Execute a simple query
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT 'Hello, World!' AS message"));

    // Fetch and display the result
    while (res->next()) {
      std::cout << "Message: " << res->getString("message") << std::endl;
    }
  } catch (sql::SQLException &e) {
    std::cerr << "Error occurred:\n";
    std::cerr << "Message: " << e.what() << std::endl;
    std::cerr << "SQLState: " << e.getSQLState() << std::endl;
    std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
