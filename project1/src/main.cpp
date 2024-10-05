#include <iostream>

#include "database_manager.h"

int main() {
  std::string file_name = "../assets/games.txt";
  std::string db_file_name = "games.db";
  DatabaseManager db(db_file_name, true);
  db.load_from_txt("../assets/games.txt", "games",
                   "DATE, VARCHAR(10), INT, FLOAT32, FLOAT32, "
                   "FLOAT32, INT, INT, BOOLEAN",
                   "\t");
  // std::cout << db.get_records()[0].toString() << std::endl;
  std::string index_name = "FT_PCT_home.idx";
  db.build_index(index_name, "FT_PCT_home", IndexType::FLOAT32);
  return 0;
}
