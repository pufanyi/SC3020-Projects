import io
import os
from pathlib import Path
from typing import Dict, List, Union

import gradio as gr
import psycopg
import psycopg.crdb
from huggingface_hub import snapshot_download
from tqdm import tqdm

PATH_FILE = os.path.dirname(os.path.abspath(__file__))
SETUP_SQL = Path(PATH_FILE) / "setup.sql"
DEFAULT_CACHE_DIR = Path(PATH_FILE).parent / "assets" / "cache"


class TPCHDataset(object):
    def __init__(
        self,
        hf_path: str = "pufanyi/TPC-H",
    ):
        super().__init__()
        self.data_path = (
            Path(
                snapshot_download(
                    repo_id=hf_path, allow_patterns="*.tbl", repo_type="dataset"
                )
            )
            / "raw_data"
        )
        self.subsets = [x.stem for x in self.data_path.glob("*.tbl")]
        self.host_status = False

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def close(self):
        if self.host_status:
            self.conn.close()
            self.host_status = False
        return self

    def host(
        self,
        *,
        host: str = "localhost",
        port: int = 5432,
        dbname: str = "tpch",
        user: str = "postgres",
        password,
    ):
        if self.host_status:
            self.close()

        self.conn = psycopg.connect(
            dbname=dbname, user=user, password=password, host=host, port=port
        )
        self.cursor = self.conn.cursor()
        self.host_status = True

        return self

    def setup(self, **kwargs):
        if not self.host_status:
            self.host(**kwargs)
            print("Connected to database")

        print(self.host_status)

        for subset in self.subsets:
            self.cursor.execute(f"DROP TABLE IF EXISTS {subset};")

        with open(SETUP_SQL, "r") as f:
            setup_sql = f.read()
        self.cursor.execute(setup_sql)

        for subset in tqdm(self.subsets, desc="Inserting subsets"):
            dataset_path = (self.data_path / f"{subset}.tbl").resolve()
            print(f"Inserting {subset} from {dataset_path}")
            self.cursor.execute(
                f"COPY {subset} FROM '{dataset_path}' DELIMITER '|' CSV;"
            )

        self.conn.commit()

    def execute(self, query: str):
        try:
            self.cursor.execute(query)
            results = self.cursor.fetchall()
            return results
        except psycopg.errors.SyntaxError as e:
            return f"Syntax Error: {e}"


if __name__ == "__main__":
    tpch = TPCHDataset()
    password = os.getenv("POSTGRES_PASSWORD")
    with tpch.host(
        host="localhost",
        port=5432,
        dbname="tpch",
        user="postgres",
        password=password,
    ):
        tpch.setup()
