#!/usr/bin/env python3
"""
exec_project.py - Run preprocess commands from rs_project_preprocess.jsonl
"""

import json
import sys
import subprocess
import os


def run_preprocess_command(entry):
    """
    Run a single preprocess command
    """
    directory = entry.get("directory", "")
    arguments = entry.get("arguments", [])
    preprocess_file = entry.get("preprocess_file")

    if not arguments or not preprocess_file:
        return False

    # Change to the directory if specified
    old_cwd = os.getcwd()
    try:
        if directory:
            os.chdir(directory)

        # Run the command
        result = subprocess.run(arguments, capture_output=True, text=True)

        # Write output to preprocess file
        with open(preprocess_file, "w") as f:
            f.write(result.stdout)

        return True
    except Exception as e:
        print(f"Error processing {preprocess_file}: {e}", file=sys.stderr)
        return False
    finally:
        os.chdir(old_cwd)


def main():
    # Read from stdin
    for line in sys.stdin:
        try:
            entry = json.loads(line.strip())
            run_preprocess_command(entry)
        except json.JSONDecodeError:
            continue


if __name__ == "__main__":
    main()
