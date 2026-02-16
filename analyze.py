#!/usr/bin/env python3
import json
import sys


def main():
    # Read compile_commands.json from stdin
    input_data = sys.stdin.read()
    if not input_data:
        return

    try:
        commands = json.loads(input_data)
    except json.JSONDecodeError as e:
        print(f"Error parsing JSON: {e}", file=sys.stderr)
        return

    # Process each command and output JSONL format
    for cmd in commands:
        entry = {
            "file": cmd["file"],
            "directory": cmd["directory"],
            "arguments": cmd["command"].split(),
        }
        print(json.dumps(entry))


if __name__ == "__main__":
    main()
