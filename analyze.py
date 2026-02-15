#!/usr/bin/env python3
import json
import shlex
import sys


def convert_command_to_arguments(obj):
    if "command" in obj and "arguments" not in obj:
        obj["arguments"] = shlex.split(obj["command"])
    return obj


def main():
    data = json.load(sys.stdin)
    for entry in data:
        entry = convert_command_to_arguments(entry)
        output = {
            "file": entry["file"],
            "directory": entry["directory"],
            "arguments": entry.get("arguments", []),
        }
        print(json.dumps(output))


if __name__ == "__main__":
    main()
