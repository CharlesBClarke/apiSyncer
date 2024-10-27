#include "DataSyncMagnr.h"
#include <algorithm>
#include <unordered_set>

DataSyncMagnr::DataSyncMagnr(const std::string &host, const std::string &user,
                             const std::string &pass, const std::string &dbname)
    : db_connector(host, user, pass, dbname),
      connection(db_connector.connect()) {

  std::cout << "Building Tree ...\n";
  // sync other Data with database
  try {
    std::unique_ptr<sql::ResultSet> res(
        db_connector.executeQuery("SELECT * FROM objects"));

    // Populate nodes map
    while (res->next()) {
      int id = res->getInt("id");
      std::string name = res->getString("name");
      nodes[id] = std::make_shared<ObjectNode>(
          id, name, std::vector<std::weak_ptr<ObjectNode>>{});
    }

    // Build tree from relationships
    res = db_connector.executeQuery("SELECT * FROM relationships");
    std::unordered_set<int> children;
    std::cout << "setting relationships ...\n";
    while (res->next()) {
      int child = res->getInt("child_id");
      int parent = res->getInt("parent_id");
      std::cout << "Parent ID: " << parent << ", Child ID: " << child << "\n";
      if (nodes.find(parent) != nodes.end() &&
          nodes.find(child) != nodes.end()) {
        nodes[parent]->pushChild(std::weak_ptr<ObjectNode>(nodes[child]));
        children.insert(child);
      } else {
        std::cerr << "Error: Parent or Child node not found!" << std::endl;
      }
    }

    // Identify root nodes
    for (const auto &pair : nodes) {
      if (children.find(pair.first) == children.end()) {
        roots.push_back(std::weak_ptr<ObjectNode>(pair.second));
      }
    }
  } catch (sql::SQLException &e) {
    std::cerr << "SQLException occurred:\n"
              << "Error Code: " << e.getErrorCode() << "\n"
              << "SQLState: " << e.getSQLState() << "\n"
              << "Message: " << e.what() << std::endl;
  }
}

// maybe I should have an isConnectted but this is all for now;
bool DataSyncMagnr::hasConnected() { return connection; }

const std::vector<std::weak_ptr<ObjectNode>> &DataSyncMagnr::getRoots() const {
  return roots;
}
/*
 * TODO:
 * - Try Catch shit
 * - Check if AUTOCOMMIT is on
 */
int DataSyncMagnr::addNode(const std::string &name, int parent_id) {

  // make sure AUTOCOMMIT is off
  db_connector.executeUpdate("SET AUTOCOMMIT = 0");

  // Insert the node into the database
  std::string query = "INSERT INTO objects (name) VALUES ('" + name + "')";
  int result = db_connector.executeUpdate(query);
  if (result == -1) {
    db_connector.executeUpdate("ROLLBACK");
    return -1;
  }

  // Retrieve the ID of the new Object
  std::unique_ptr<sql::ResultSet> res =
      db_connector.executeQuery("SELECT LAST_INSERT_ID() AS id");
  if (!res || !res->next()) {
    return -1;
  }
  int new_id = res->getInt("id");

  // add to nodes map
  nodes[new_id] = std::make_shared<ObjectNode>(
      new_id, name, std::vector<std::weak_ptr<ObjectNode>>{});

  // Now update relationships if needed
  if (parent_id != -1) {
    // Insert into relationships table
    std::string query =
        "INSERT INTO relationships (child_id, parent_id) VALUES (" +
        std::to_string(new_id) + ", " + std::to_string(parent_id) + ")";
    int result = db_connector.executeUpdate(query);
    if (result == -1) {
      db_connector.executeUpdate("ROLLBACK");
      return -1;
    }
    // Check if parent node exists in nodes map
    auto parentNodeIt = nodes.find(parent_id);
    if (parentNodeIt != nodes.end() && parentNodeIt->second) {
      // Parent node exists, add new node as child
      parentNodeIt->second->pushChild(std::weak_ptr<ObjectNode>(nodes[new_id]));
    } else {
      // Parent node not found, rollback and return error
      db_connector.executeUpdate("ROLLBACK");
      return -1;
    }
  } else {
    // if no parent, add to roots
    roots.push_back(std::weak_ptr<ObjectNode>(nodes[new_id]));
  }

  // Re-enable auto-commit
  db_connector.executeUpdate("SET AUTOCOMMIT = 1");

  return new_id;
}

void DataSyncMagnr::removeNode(int id) {
  auto it = nodes.find(id);
  std::shared_ptr<ObjectNode> object;
  if (it != nodes.end()) {
    object = {it->second};
  } else {
    std::cerr << "Node Not found";
    return;
  }
  if (object->getChildren().size() != 0) {
    std::cerr << "Cannot remove node: it has child nodes.\n";
    return;
  }

  // make sure AUTOCOMMIT is off
  db_connector.executeUpdate("SET AUTOCOMMIT = 0");

  // delete from objects
  std::string query = "DELETE FROM objects WHERE id = " + std::to_string(id);
  int result = db_connector.executeUpdate(query);
  if (result == -1) {
    db_connector.executeUpdate("ROLLBACK");
    std::cerr << "DataBase Failure: delete from objects failed";
    return;
  }
  // Step 1: Construct the query to find the parent_id
  query = "SELECT parent_id FROM relationships WHERE child_id = " +
          std::to_string(id);

  // Step 2: Execute the query to get the parent_id
  std::unique_ptr<sql::ResultSet> res(db_connector.executeQuery(query));

  // Step 3: Check if a result exists
  if (res->next()) {
    // Step 4: Retrieve the parent_id from the result set
    int parent_id = res->getInt("parent_id");
    auto parent_it = nodes.find(parent_id);
    std::shared_ptr<ObjectNode> parent_object;
    if (parent_it != nodes.end()) {
      parent_object = parent_it->second;
    } else {
      std::cerr << "Parent Node Not found\n";
      db_connector.executeUpdate("ROLLBACK");
      db_connector.executeUpdate("SET AUTOCOMMIT = 1");
      return;
    }
    parent_object->removeChild(object);

    // delete from relationships
    query = "DELETE FROM relationships WHERE parent_id = " +
            std::to_string(parent_id);
    result = db_connector.executeUpdate(query);
    if (result == -1) {
      db_connector.executeUpdate("ROLLBACK");
      std::cerr << "DataBase Failure: delete from relationships failed";
      return;
    }
  } else {
    roots.erase(
        std::remove_if(
            roots.begin(), roots.end(),
            [&object](const std::weak_ptr<ObjectNode> &weak_ptr) {
              if (auto sp_wp = weak_ptr.lock()) {
                return sp_wp == object; // Remove if pointing to the same object
              }
              return false; // Do not remove if weak_ptr could not be locked
            }),
        roots.end());
  }

  // FInally remove form
  nodes.erase(id);

  // Commit the transaction
  db_connector.executeUpdate("COMMIT");
  db_connector.executeUpdate("SET AUTOCOMMIT = 1");
}
