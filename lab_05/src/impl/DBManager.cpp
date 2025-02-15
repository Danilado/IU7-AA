#include "DBManager.hpp"
#include <format>

DatabaseManager::DatabaseManager(const std::string &dbName) {
  if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    throw std::runtime_error(
        std::format("Can't open database: {}", sqlite3_errmsg(db)));

  if (!createTables())
    throw std::runtime_error("Could not reset table");
}

DatabaseManager::~DatabaseManager() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;
  }
}

bool DatabaseManager::createTables() {
  const char *createTableSQL = R"(
            DROP TABLE IF EXISTS recipes;
            CREATE TABLE IF NOT EXISTS recipes (
                id INTEGER PRIMARY KEY,
                issue_id INTEGER NOT NULL DEFAULT 9185,
                title TEXT,
                ingredients TEXT,
                steps TEXT,
                image_url TEXT
            );
        )";

  char *errMsg = nullptr;
  if (sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg) !=
      SQLITE_OK) {
    std::cerr << "SQL error: " << errMsg << std::endl;
    sqlite3_free(errMsg);
    return false;
  }

  return true;
}

bool DatabaseManager::insertRecipe(int id, const std::string &title,
                                   const std::string &ingredients,
                                   const std::vector<std::string> &steps,
                                   const std::string &imageUrl) {
  const std::string insertSQL =
      "INSERT INTO recipes (id, title, ingredients, steps, image_url) "
      "VALUES (?, ?, ?, ?, ?);";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr) !=
      SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db)
              << std::endl;
    return false;
  }

  std::string steps_text = "";
  int offset = 0;
  for (int i = 0; i < steps.size(); ++i) {
    if (steps[i].size())
      steps_text.append(std::format("{}: {}\n", i + 1 - offset, steps[i]));
    else
      offset++;
  }

  sqlite3_bind_int(stmt, 1, id);
  sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, ingredients.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, steps_text.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, imageUrl.c_str(), -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db)
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

std::string deserializeWString(const std::string &serialized) {
  std::string result;
  std::stringstream ss(serialized);
  int num;
  while (ss >> num) {
    result.push_back(static_cast<wchar_t>(num));
    if (ss.peek() == ',')
      ss.ignore();
  }
  return result;
}

void DatabaseManager::dump() {
  sqlite3_stmt *stmt;
  std::string query =
      "SELECT id, issue_id, title, ingredients, steps, image_url FROM recipes";
  int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

  if (rc != SQLITE_OK) {
    std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return;
  }

  // json recipes = json::array();

  // while (sqlite3_step(stmt) == SQLITE_ROW) {
  //   json recipe;
  //   recipe["id"] = sqlite3_column_int(stmt, 0);
  //   recipe["issue_id"] = sqlite3_column_int(stmt, 1);
  //   recipe["title"] =
  //       reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
  //   recipe["ingredients"] =
  //       reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
  //   recipe["steps"] =
  //       reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
  //   recipe["image_url"] =
  //       reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
  //   recipes.push_back(recipe);
  // }

  sqlite3_finalize(stmt);
  // sqlite3_close(db);
}