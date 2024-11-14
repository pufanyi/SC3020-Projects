from tqdm import tqdm
from pathlib import Path
from datasets import load_dataset, get_dataset_config_names, Dataset
from typing import Dict, List


class TPCHDataset(object):
    def __init__(self, hf_path: str = "pufanyi/TPC-H"):
        self.subsets: List[str] = get_dataset_config_names(hf_path)
        self.data: Dict[str, Dataset] = {}
        for subset in tqdm(self.subsets, desc="Loading subsets"):
            self.data[subset] = load_dataset(hf_path, subset, split="train")

    def save(self, path: str | Path = Path(__file__).parents[1] / "assets" / "data"):
        path.mkdir(parents=True, exist_ok=True)
        if isinstance(path, str):
            path = Path(path)
        for subset in tqdm(self.subsets, desc="Saving subsets"):
            self.data[subset].to_csv(path / f"{subset}.csv", index=False)
