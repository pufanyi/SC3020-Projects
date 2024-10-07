#include <chrono>
#include <iostream>

#include "database_manager.h"
#include "utils.h"

int main() {
  std::string file_name = "../assets/games.txt";
  std::string db_file_name = "games.db";
  DatabaseManager db(db_file_name, true);
  db.load_from_txt("../assets/games.txt", "games",
                   "DATE, VARCHAR(10), INT, FLOAT32, FLOAT32, "
                   "FLOAT32, INT, INT, BOOLEAN",
                   "\t");
  std::cout << db.get_records()[0] << std::endl;
  std::string index_name = "FG_PCT_home.idx";
  size_t before;
  size_t after;
  before = IO_TIMES;
  auto b_tree = db.build_index(index_name, "FG_PCT_home", IndexType::FLOAT32);
  after = IO_TIMES;
  std::cout << "IO Times From Building Index: " << after - before << std::endl;

  std::shared_ptr<Index> lower_bound_index =
      std::make_shared<Float32Index>(0.5 - 1e-6);
  std::shared_ptr<Index> upper_bound_index =
      std::make_shared<Float32Index>(0.8 + 1e-6);

  auto start = std::chrono::high_resolution_clock::now();
  before = IO_TIMES;
  auto result = b_tree->range_query(lower_bound_index, upper_bound_index);
  after = IO_TIMES;
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  auto micro_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

  size_t io_from_range_query = after - before;
  std::cout << "Result size: " << result.size() << std::endl;

  float sum = 0;
  float FG3_PCT_home_sum = 0;
  before = IO_TIMES;
  for (auto record : result) {
    std::string value = record.toString();
    std::vector<std::string> splitted_values = split(value, ' ');
    // 4th value is FG_PCT_home
    float float_value = std::stof(splitted_values[3]);
    sum += float_value;
    float FG3_PCT_home_value = std::stof(splitted_values[5]);
    FG3_PCT_home_sum += FG3_PCT_home_value;
  }
  after = IO_TIMES;
  size_t io_from_accessing_result = after - before;
  std::cout << "IO TImes From Range Query: "
            << io_from_range_query - io_from_accessing_result << std::endl;
  std::cout << "IO Times From Accessing Result: " << io_from_accessing_result
            << std::endl;

  std::cout << "Average FG_PCT_home: " << sum / result.size() << std::endl;
  std::cout << "Average FG3_PCT_home: " << FG3_PCT_home_sum / result.size()
            << std::endl;

  // To get the value of duration use the count()
  // member function on the duration object
  std::cout << "Range Query time in Milliseconds : " << duration.count()
            << std::endl;
  std::cout << "Range Query time in Milliseconds : " << micro_duration.count()
            << std::endl;

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
