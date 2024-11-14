import os
from pathlib import Path
from typing import Dict, List, Union

import psycopg
from datasets import Dataset, get_dataset_config_names, load_dataset
from tqdm import tqdm

# Get the full path of current file
PATH_FILE = os.path.dirname(os.path.abspath(__file__))
path = Path(PATH_FILE)


class TPCHDataset(object):
    def __init__(self, hf_path: str = "pufanyi/TPC-H"):
        self.subsets: List[str] = get_dataset_config_names(hf_path)
        self.data: Dict[str, Dataset] = {}
        for subset in tqdm(self.subsets, desc="Loading subsets"):
            self.data[subset] = load_dataset(hf_path, subset, split="train")
        self.host_status = False

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def close(self):
        if self.host_status:
            self.conn.close()
        return self

    def host(
        self,
        *,
        dbname="tpch",
        user="postgres",
    ):
        self.host_status = True
        self.conn = psycopg.connect(f"dbname={dbname} user={user}")
        self.cursor = self.conn.cursor()

        # Create tables for each subset
        for subset in self.subsets:
            # Get column names from dataset
            columns = self.data[subset].column_names
            columns_sql = ", ".join([f"{col} TEXT" for col in columns])

            # Create table
            self.cursor.execute(f"DROP TABLE IF EXISTS {subset}")
            self.cursor.execute(f"CREATE TABLE {subset} ({columns_sql})")

            # Insert data
            for row in self.data[subset]:
                values = [str(row[col]) for col in columns]
                values_sql = ", ".join([f"'{val}'" for val in values])
                self.cursor.execute(f"INSERT INTO {subset} VALUES ({values_sql})")

        self.conn.commit()
        return self

    def execute(self, query: str):
        self.cursor.execute(query)
        return self.cursor.fetchall()

    def save(self, path: Union[str, Path]):
        if isinstance(path, str):
            path = Path(path)
        path.mkdir(parents=True, exist_ok=True)
        for subset in tqdm(self.subsets, desc="Saving subsets"):
            self.data[subset].to_csv(path / f"{subset}.csv", index=False)


if __name__ == "__main__":
    tpch = TPCHDataset()
    tpch.save(path.parent / "sc3020" / "assets" / "data")
