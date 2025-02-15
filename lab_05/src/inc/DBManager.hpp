#pragma once

#include <chrono>
#include <exception>
#include <iostream>
#include <sqlite3.h>
#include <vector>

class DatabaseManager {
private:
  sqlite3 *db;

  bool createTables();

public:
  DatabaseManager(const std::string &dbName);
  ~DatabaseManager();

  bool insertRecipe(int id, const std::string &title,
                    const std::string &ingredients,
                    const std::vector<std::string> &steps,
                    const std::string &imageUrl);

  void dump();
};
