import io
import os
import traceback
from pathlib import Path
from typing import Dict, List, Union

import gradio as gr
import psycopg
import psycopg.crdb
from huggingface_hub import snapshot_download
from sc3020.database.visualizer import Visualizer
from sc3020.whatif import prepare_join_command, prepare_scan_command
from tqdm import tqdm

from .ExecutionTree import (
    ExecutionTree,
    ExecutionTreeNode,
    parse_query_explanation_to_tree,
)

PATH_FILE = os.path.dirname(os.path.abspath(__file__))
SETUP_SQL = Path(PATH_FILE) / "setup.sql"
DEFAULT_CACHE_DIR = Path(PATH_FILE).parent / "assets" / "cache"


class TPCHDataset(object):
    def __init__(
        self,
        hf_path: str = "pufanyi/TPC-H",
        query_time_out: int = 60,
        max_output_rows: int = 20,
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
        self.query_time_out = query_time_out
        self.subsets = [x.stem for x in self.data_path.glob("*.tbl")]
        self.host_status = False
        self.max_output_rows = max_output_rows
        self.visualizer = Visualizer()

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
        self.db_info = dict(
            host=host, port=port, dbname=dbname, user=user, password=password
        )

        if self.host_status:
            self.close()

        self.conn = psycopg.connect(
            dbname=dbname, user=user, password=password, host=host, port=port
        )
        self.cursor = self.conn.cursor()
        self.host_status = True
        self.cursor.execute(f"SET statement_timeout = {self.query_time_out * 1000};")

        return self

    def setup(self, **kwargs):
        if not self.host_status:
            self.db_info.update(kwargs)
            self.host(**self.db_info)
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
            self.cursor.execute(f"EXPLAIN {query}")
            explain = self.cursor.fetchall()
            tree = parse_query_explanation_to_tree(explain)
            traverse_node = tree.traversal()
            explain_str = ""
            for idx, node in enumerate(traverse_node, 1):
                explain_str += f"Step {idx} : {node.natural_language()}\n"
            fig = self.visualizer.visualize(tree)
            if len(results) > self.max_output_rows:
                messages = {
                    "status": "Success",
                    "message": f"Returned {len(results)} rows, only showing first {self.max_output_rows}",
                }
                results = results[: self.max_output_rows]
            else:
                messages = {
                    "status": "Success",
                    "message": f"Returned {len(results)} rows",
                }
            return results, messages, explain_str, fig
        except Exception as e:
            self.host(**self.db_info)
            return (
                [],
                {"status": "Error", "message": str(traceback.format_exc())},
                "Wrong execution",
            )

    def set_query_config(self, query_config: str):
        try:
            self.cursor.execute(query_config)
        except Exception as e:
            self.host(**self.db_info)
            return {"status": "Error", "message": str(traceback.format_exc())}

    def execute_with_what_if(self, query_input: str, scan_type: str, join_type: str):
        if scan_type != "No Changing":
            scan_command = prepare_scan_command(scan_type)
            self.set_query_config(scan_command)
        else:
            scan_command = ""

        if join_type != "No Changing":
            join_command = prepare_join_command(join_type)
            self.set_query_config(join_command)
        else:
            join_command = ""

        return self.execute(query_input)


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
