#!/usr/bin/env python3
import json
import subprocess
import sys


def main():
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        obj = json.loads(line)
        arguments = obj.get("arguments", [])
        directory = obj.get("directory", ".")

        result = subprocess.run(
            arguments, cwd=directory, capture_output=True, text=True
        )
        if result.returncode != 0:
            print(f"Error executing: {' '.join(arguments)}", file=sys.stderr)
            print(result.stderr, file=sys.stderr)


if __name__ == "__main__":
    main()
