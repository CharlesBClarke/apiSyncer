cmake_minimum_required(VERSION 3.10)
project(MySQLCrowApp)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the path to Crow and MySQL Connector/C++
set(MYSQL_CONN_DIR "${PROJECT_SOURCE_DIR}/mysql")
set(CROW_DIR "${PROJECT_SOURCE_DIR}/Crow")  # Added this line

# Include directories for Crow and MySQL
include_directories(${CROW_DIR}/include)
include_directories(${MYSQL_CONN_DIR}/include)
include_directories(${MYSQL_CONN_DIR}/include/jdbc)

# Link library directories for MySQL Connector/C++
link_directories(${MYSQL_CONN_DIR}/lib64)

# Add the executable
add_executable(MySQLCrowApp main.cpp)

# Link libraries
target_link_libraries(MySQLCrowApp mysqlcppconn)
