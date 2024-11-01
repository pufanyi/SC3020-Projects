"""Launch the inference server."""

import os
import sys

from sc3020.main import launch_server, prepare_server_args

if __name__ == "__main__":
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    server_args = prepare_server_args(sys.argv[1:])

    try:
        launch_server(server_args)
    except Exception as e:
        raise e
