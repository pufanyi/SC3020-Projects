import glob

from utils import run_unittest_files

if __name__ == "__main__":
    files = glob.glob("**/test_*.py", recursive=True)

    exit_code = run_unittest_files(files)
    exit(exit_code)
