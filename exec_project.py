#!/usr/bin/env python3
import json
import subprocess
import sys


def main():
    # Read from stdin
    input_data = sys.stdin.read()
    if not input_data:
        return

    try:
        entries = [json.loads(line) for line in input_data.strip().split("\n")]
    except json.JSONDecodeError as e:
        print(f"Error parsing JSONL: {e}", file=sys.stderr)
        return

    # Execute each command and generate .i files
    for entry in entries:
        cmd = [arg for arg in entry["arguments"]]
        try:
            result = subprocess.run(
                cmd, cwd=entry["directory"], capture_output=True, text=True
            )
            if result.returncode != 0:
                print(f"Error running command: {result.stderr}", file=sys.stderr)
        except Exception as e:
            print(f"Exception running command: {e}", file=sys.stderr)


if __name__ == "__main__":
    main()
