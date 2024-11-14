import argparse
import os
import sys
from dataclasses import dataclass, fields
from typing import List

import uvicorn
from sc3020.interface import app


@dataclass
class ServerArgs:
    host: str
    port: int
    log_level: str

    @staticmethod
    def add_cli_args(parser: argparse.ArgumentParser):
        parser.add_argument("--host", type=str, default="127.0.0.1")
        parser.add_argument("--port", type=int, default=8000)
        parser.add_argument("--log-level", type=str, default="info")

    @classmethod
    def from_cli_args(cls, args: argparse.Namespace):
        attrs = [attr.name for attr in fields(cls)]
        return cls(**{attr: getattr(args, attr) for attr in attrs})


def prepare_server_args(argv: List[str]) -> ServerArgs:
    parser = argparse.ArgumentParser()
    ServerArgs.add_cli_args(parser)
    raw_args = parser.parse_args(argv)
    server_args = ServerArgs.from_cli_args(raw_args)
    return server_args


def launch_server(
    server_args: ServerArgs,
):
    uvicorn.run(
        app,
        host=server_args.host,
        port=server_args.port,
        log_level=server_args.log_level,
    )


def main():
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    server_args = prepare_server_args(sys.argv[1:])

    launch_server(server_args)


if __name__ == "__main__":
    main()
