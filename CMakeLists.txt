cmake_minimum_required(VERSION 3.10)
project(invmanr_api)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include             # Your project's include directory
    ${PROJECT_SOURCE_DIR}/mysql_connector_cpp/include  # Connector's include directory
)

# Library directories
link_directories(
    ${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64
)

# Source files
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/*.cpp"
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    mysqlcppconn        # Link against libmysqlcppconn.so
)

# Set RPATH to find shared libraries at runtime
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64"
)

# Optionally, if you need to include the 'private' libraries
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     BUILD_RPATH "${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64;${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64/private"
# )
