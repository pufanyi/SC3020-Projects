#include <gtest/gtest.h>

#include <chrono>

#include "managers.h"

TEST(ManagerTest, DBManagerLoadFromTxtTest) {
  DatabaseManager db_manager("test.db", true);
  std::vector<Record> records =
      db_manager.load_from_txt("../assets/games.txt", "games",
                               "DATE, VARCHAR(10), INT, FLOAT32, FLOAT32, "
                               "FLOAT32, INT, INT, BOOLEAN",
                               "\t");
  EXPECT_EQ(records.size(), 26651);
  EXPECT_EQ(records[0].toString(),
            "22/12/2022 1610612740 126 0.484000 0.926000 0.382000 25 46 1 ");
  EXPECT_EQ(records[1].toString(),
            "22/12/2022 1610612762 120 0.488000 0.952000 0.457000 16 40 1 ");
  EXPECT_EQ(records[2].toString(),
            "21/12/2022 1610612739 114 0.482000 0.786000 0.313000 22 37 1 ");
}

TEST(ManagerTest, DBManagerLoadFromDbTest) {
  DatabaseManager db_manager("test.db", false);
  std::vector<Record> records = db_manager.load_from_db(
      "games",
      "GAME_DATE_EST DATE, TEAM_ID_home VARCHAR(10), PTS_home INT,"
      "FG_PCT_home FLOAT32, FT_PCT_home FLOAT32,"
      "FG3_PCT_home FLOAT32, AST_home INT, REB_home INT, HOME_TEAM_WINS "
      "BOOLEAN",
      26651);
  EXPECT_EQ(records.size(), 26651);
  EXPECT_EQ(records[0].toString(),
            "22/12/2022 1610612740 126 0.484000 0.926000 0.382000 25 46 1 ");
  EXPECT_EQ(records[1].toString(),
            "22/12/2022 1610612762 120 0.488000 0.952000 0.457000 16 40 1 ");
  EXPECT_EQ(records[2].toString(),
            "21/12/2022 1610612739 114 0.482000 0.786000 0.313000 22 37 1 ");
}

TEST(ManagerTest, LinearScan) {
  DatabaseManager db_manager("test.db", false);
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<Record> records = db_manager.linear_scan(
      "FG_PCT_home",
      "GAME_DATE_EST DATE, TEAM_ID_home VARCHAR(10), PTS_home INT,"
      "FG_PCT_home FLOAT32, FT_PCT_home FLOAT32,"
      "FG3_PCT_home FLOAT32, AST_home INT, REB_home INT, HOME_TEAM_WINS "
      "BOOLEAN",
      0.5, 0.8);
  auto stop = std::chrono::high_resolution_clock::now();
  EXPECT_EQ(records.size(), 6902);
  float sum = 0;
  for (auto record : records) {
    std::string value = record.toString();
    std::vector<std::string> splitted_values = split(value, ' ');
    // 4th value is FG_PCT_home
    float float_value = std::stof(splitted_values[3]);
    sum += float_value;
  }
  EXPECT_NEAR(sum / records.size(), 0.53, 0.01);
  GTEST_COUT << "Average FG_PCT_home: " << sum / records.size() << std::endl;
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  auto micro_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

  // To get the value of duration use the count()
  // member function on the duration object
  GTEST_COUT << "Linear Scan time in Milliseconds : " << duration.count()
             << std::endl;
  GTEST_COUT << "Linear Scan time in Milliseconds : " << micro_duration.count()
             << std::endl;
}
