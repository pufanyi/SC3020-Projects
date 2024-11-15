import json
from pathlib import Path

import pandas as pd
from datasets import Dataset
from huggingface_hub import HfApi

if __name__ == "__main__":
    folder = Path(__file__).parents[1] / "assets" / "data"
    with open(Path(__file__).parent / "headers.json", "r") as f:
        headers = json.load(f)

    api = HfApi()

    for file in folder.glob("*.tbl"):
        header = headers[file.stem]
        data = pd.read_table(file, sep="|", names=header)
        hf_data = Dataset.from_pandas(data, preserve_index=False)
        hf_data.push_to_hub(f"pufanyi/TPC-H", file.stem)

        api.upload_file(
            path_or_fileobj=file,
            path_in_repo=f"raw_data/{file.name}",
            repo_id="pufanyi/TPC-H",
            repo_type="dataset",
        )
