#include <iostream>

#include "database_manager.h"
#include "util.h"

int main() {
  std::string file_name = "../assets/games.txt";
  std::string db_file_name = "games.db";
  DatabaseManager db(db_file_name, true);
  db.load_from_txt("../assets/games.txt", "games",
                   "DATE, VARCHAR(10), INT, FLOAT32, FLOAT32, "
                   "FLOAT32, INT, INT, BOOLEAN",
                   "\t");
  // std::cout << db.get_records()[0].toString() << std::endl;
  std::string index_name = "FG_PCT_home.idx";
  std::cout << "IO Times Before Building Index: " << IO_TIMES << std::endl;
  auto b_tree = db.build_index(index_name, "FG_PCT_home", IndexType::FLOAT32);
  std::cout << "IO Times After Building Index: " << IO_TIMES << std::endl;
  std::shared_ptr<Index> lower_bound_index =
      std::make_shared<Float32Index>(0.5 - 1e-6);
  std::shared_ptr<Index> upper_bound_index =
      std::make_shared<Float32Index>(0.8 + 1e-6);
  // auto result = b_tree->range_query(lower_bound_index, upper_bound_index);
  std::shared_ptr<BPlusTree> b_tree_load =
      db.load_index(index_name, "FG_PCT_home", IndexType::FLOAT32);
  auto result = b_tree_load->range_query(lower_bound_index, upper_bound_index);
  std::cout << "IO Times After Range Query: " << IO_TIMES << std::endl;
  std::cout << "Result size: " << result.size() << std::endl;
  // for (const auto &record : result) {
  //   std::cout << record.toString() << std::endl;
  // }
  // std::vector<Record> records = db.linear_scan(
  //     "FG_PCT_home",
  //     "GAME_DATE_EST DATE, TEAM_ID_home VARCHAR(10), PTS_home INT,"
  //     "FG_PCT_home FLOAT32, FT_PCT_home FLOAT32,"
  //     "FG3_PCT_home FLOAT32, AST_home INT, REB_home INT, HOME_TEAM_WINS "
  //     "BOOLEAN",
  //     0.5, 0.8);
  // std::cout << "Linear scan result size: " << records.size() << std::endl;
  // for (const auto &record : records) {
  //   std::cout << record.toString() << std::endl;
  // }
  return 0;
}
