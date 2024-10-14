cmake_minimum_required(VERSION 3.10)
project(invmanr_api)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/mysql_connector_cpp/include
)

# Library directories
link_directories(
    ${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64
)

# Source files
set(SOURCES
    src/main.cpp
    src/routes.cpp
    include/ObjectNode.cpp
    include/MySQLDB.cpp
    include/DataSyncMagnr.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    mysqlcppconn
    pthread
)

# Set RPATH to find shared libraries at runtime
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64"
)
