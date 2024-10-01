#include <gtest/gtest.h>

#include "managers.h"

TEST(ManagerTest, DBManagerTest) {
  DatabaseManager db_manager("test.db", true);
  std::vector<Record> records = db_manager.load_from_txt(
      "../assets/games.txt", "games",
      "DATE, VARCHAR(10), INT, FLOAT32, FLOAT32, FLOAT32, INT, INT, BOOLEAN",
      "\t");
}
