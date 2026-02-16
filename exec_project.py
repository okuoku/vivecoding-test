#!/usr/bin/env python3
"""
Execute preprocessing commands from rs_project_preprocess.jsonl and generate .i files.
"""

import sys, json, subprocess


def main():
    for line in sys.stdin:
        obj = json.loads(line)
        args = obj.get("arguments", [])
        if not args:
            continue
        # Run the command; assume it writes to preprocess_file via -o option
        try:
            subprocess.run(args, check=True)
        except subprocess.CalledProcessError as e:
            sys.stderr.write(f"Command failed: {e}\n")


if __name__ == "__main__":
    main()
