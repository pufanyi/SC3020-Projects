from typing import Literal

JOIN_REGISTRY = {
    "HASH_JOIN": "enable_hashjoin",
    "MERGE_JOIN": "enable_mergejoin",
    "NESTED_LOOP_JOIN": "enable_nestloop",
}

SCAN_REGISTRY = {
    "BITMAP_SCAN": "enable_bitmapscan",
    "INDEX_SCAN": "enable_indexscan",
    "INDEX_ONLY_SCAN": "enable_indexonlyscan",
    "SEQ_SCAN": "enable_seqscan",
}

# Above is the registry of the join and scan types


def format_set_str(status: str, command: Literal["on", "off"]) -> str:
    return f"SET {status} = {command};\n"


def prepare_join_command(selected_join: str):
    command_str = ""

    if selected_join not in JOIN_REGISTRY:
        raise ValueError(
            f"Invalid join type {selected_join}, Please select from {list(JOIN_REGISTRY.keys())}"
        )

    for join_type, status in JOIN_REGISTRY.items():
        if join_type == selected_join:
            command_str += format_set_str(status, "on")
        else:
            command_str += format_set_str(status, "off")

    return command_str


def prepare_scan_command(selected_command: str):
    command_str = ""

    if selected_command not in SCAN_REGISTRY:
        raise ValueError(
            f"Invalid scan type {selected_command}, Please select from {list(SCAN_REGISTRY.keys())}"
        )

    for scan_type, status in SCAN_REGISTRY.items():
        if scan_type == selected_command:
            command_str += format_set_str(status, "on")
        else:
            command_str += format_set_str(status, "off")

    return command_str
