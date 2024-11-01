"Copied from sglang https://github.com/sgl-project/sglang/blob/main/python/sglang/test/test_utils.py"

import os
import subprocess
import time
from pathlib import Path
from typing import List


def run_unittest_files(files: List[Path]):
    tic = time.time()
    success = True

    for filename in files:

        def run_one_file(filename: Path):
            filename = filename.resolve().absolute()
            print(f"\n\nRun:\npython -m unittest {filename}\n\n", flush=True)

            try:
                # Use check_output to capture output and handle errors
                subprocess.check_output(
                    ["python", "-m", "unittest", "-v", str(filename)],
                    stderr=subprocess.STDOUT,
                    env=os.environ.copy(),
                )
                return 0
            except subprocess.CalledProcessError as e:
                print(e.output.decode())
                return e.returncode

        try:
            ret_code = run_one_file(filename)
            assert ret_code == 0
        except Exception as e:
            time.sleep(5)
            print(
                f"\nError {e} when running {filename}\n",
                flush=True,
            )
            success = False
            break

    if success:
        print(f"Success. Time elapsed: {time.time() - tic:.2f}s", flush=True)
    else:
        print(f"Fail. Time elapsed: {time.time() - tic:.2f}s", flush=True)

    return 0 if success else -1
