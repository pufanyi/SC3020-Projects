#include <gtest/gtest.h>

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
