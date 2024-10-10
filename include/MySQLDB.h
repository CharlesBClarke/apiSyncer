#ifndef MYSQLDB_H
#define MYSQLDB_H

#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <memory>
#include <string>

class MySQLDB {
public:
  // Constructor to initialize with database credentials
  MySQLDB(const std::string &host, const std::string &user,
          const std::string &password, const std::string &database);

  // Destructor to close the connection
  ~MySQLDB();

  // Connect to the MySQL database
  bool connect();

  // Disconnect from the MySQL database
  void disconnect();

  // Execute a query that returns results (e.g., SELECT)
  std::unique_ptr<sql::ResultSet> executeQuery(const std::string &query);

  // Execute an update (e.g., INSERT, UPDATE, DELETE)
  int executeUpdate(const std::string &query);

private:
  std::string host;
  std::string user;
  std::string password;
  std::string database;

  sql::mysql::MySQL_Driver *driver;
  std::unique_ptr<sql::Connection> connection;
};

#endif // MYSQLDB_H
