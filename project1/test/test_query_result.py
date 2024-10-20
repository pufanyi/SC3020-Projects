import unittest

import pandas as pd


class TestQueryResult(unittest.TestCase):
    @classmethod
    def setUp(self) -> None:
        data_path = "../assets/games.txt"
        self.df = pd.read_csv(data_path, sep="\t")

    def test_FG_PCT_home_result(self):
        data = self.df.loc[(self.df["FG_PCT_home"] >= 0.5) & (self.df["FG_PCT_home"] <= 0.8)]
        self.assertEqual(len(data), 6902)
        self.assertEqual(data["FG_PCT_home"].mean().round(2), 0.53)
        self.assertEqual(data["FG3_PCT_home"].mean().round(2), 0.42)
