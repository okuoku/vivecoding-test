#!/usr/bin/env python3
import json
import subprocess
import sys
import os


def run_command(command, input_data):
    try:
        result = subprocess.run(
            command,
            input=input_data.encode(),
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            print(f"Error running command: {command}", file=sys.stderr)
            sys.exit(1)
    except Exception as e:
        print(f"Exception: {e}", file=sys.stderr)
        sys.exit(1)


def main():
    for line in sys.stdin:
        try:
            data = json.loads(line)
            command = [
                "gcc",
                "-E",
                f"-o {data['preprocess_file']}",
                data["file"],
            ] + data["arguments"]
            run_command(command, line.encode())
        except (json.JSONDecodeError, KeyError) as e:
            print(f"Error processing input: {e}", file=sys.stderr)


if __name__ == "__main__":
    main()
