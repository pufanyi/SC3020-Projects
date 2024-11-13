from pathlib import Path
import pandas as pd
from datasets import Dataset
import json


if __name__ == "__main__":
    folder = Path(__file__).parent.parent / "assets" / "data"
    print(folder)
    with open("headers.json") as f:
        headers = json.load(f)
    for file in folder.glob("*.tbl"):
        header = headers[file.stem]
        data = pd.read_table(file, sep="|", names=header)
        hf_data = Dataset.from_pandas(data, preserve_index=False)
        hf_data.push_to_hub(f"pufanyi/TPC-H", "3.0.1", split=file.stem)
