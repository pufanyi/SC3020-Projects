import glob
from pathlib import Path

from utils import run_unittest_files

if __name__ == "__main__":
    this_file = Path(__file__).resolve()
    files = this_file.parent.rglob("test_*.py")

    exit_code = run_unittest_files(files)
    exit(exit_code)
