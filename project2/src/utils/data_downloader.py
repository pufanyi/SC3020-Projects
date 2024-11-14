import os
from pathlib import Path
from typing import Dict, List, Union

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

    def save(self, path: Union[str, Path]):
        if isinstance(path, str):
            path = Path(path)
        path.mkdir(parents=True, exist_ok=True)
        for subset in tqdm(self.subsets, desc="Saving subsets"):
            self.data[subset].to_csv(path / f"{subset}.csv", index=False)


if __name__ == "__main__":
    tpch = TPCHDataset()
    tpch.save(path.parent / "sc3020" / "assets" / "data")
