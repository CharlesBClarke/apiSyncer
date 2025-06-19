cmake_minimum_required(VERSION 3.10)

project(invmanr_api)

# ────────────────────────────────────────────────────────────
# Build settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # for clangd

# ────────────────────────────────────────────────────────────
# Stand-alone Asio (NO Boost)
#   • install:  pacman -S asio openssl zlib   (Arch)
#               apt  install libasio-dev libssl-dev zlib1g-dev   (Ubuntu)
add_definitions(-Dasio_standalone)

# ────────────────────────────────────────────────────────────
# Include / link paths
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/mysql_connector_cpp/include
)

link_directories(
    ${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64
)

# ────────────────────────────────────────────────────────────
# Sources
set(SOURCES
    src/main.cpp
    src/routes.cpp
    include/ObjectNode.cpp
    include/MySQLDB.cpp
    include/DataSyncMagnr.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# ────────────────────────────────────────────────────────────
# Libraries
target_link_libraries(${PROJECT_NAME}
    mysqlcppconn
    pthread
    ssl
    crypto
    z
)

# ────────────────────────────────────────────────────────────
# Runtime library path (so the MySQL connector .so is found)
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "${PROJECT_SOURCE_DIR}/mysql_connector_cpp/lib64"
)
