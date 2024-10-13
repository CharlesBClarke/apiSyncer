// DataSyncMagnr.cpp
#ifndef DATASYNCMAGNR_CPP
#define DATASYNCMAGNR_CPP

#include "DataSyncMagnr.h"

// Add data to all structures
template <typename T> void DataSyncMagnr<T>::addData(int key, const T &value) {
  dataMap[key] = value;
  dataTree[key] = value;
  database[key] = value;
}

// Update data in all structures
template <typename T>
void DataSyncMagnr<T>::updateData(int key, const T &newValue) {
  if (dataMap.find(key) != dataMap.end()) {
    dataMap[key] = newValue;
    dataTree[key] = newValue;
    database[key] = newValue;
  } else {
    throw std::runtime_error("Key not found");
  }
}

// Remove data from all structures
template <typename T> void DataSyncMagnr<T>::removeData(int key) {
  dataMap.erase(key);
  dataTree.erase(key);
  database.erase(key);
}

// Retrieve data (from map as example)
template <typename T> T DataSyncMagnr<T>::getData(int key) const {
  auto it = dataMap.find(key);
  if (it != dataMap.end()) {
    return it->second;
  } else {
    throw std::runtime_error("Key not found");
  }
}

// Print data from all structures (for debugging)
template <typename T> void DataSyncMagnr<T>::printData() const {
  std::cout << "Data from Map:" << std::endl;
  for (const auto &pair : dataMap) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }

  std::cout << "Data from Tree:" << std::endl;
  for (const auto &pair : dataTree) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }

  std::cout << "Data from Database:" << std::endl;
  for (const auto &pair : database) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }
}

#endif // DATASYNCMAGNR_CPP
