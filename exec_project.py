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
    The -o option is already set in the arguments, so we just need to run it
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

        # Run the command - it will write to preprocess_file via -o option
        result = subprocess.run(arguments, capture_output=True, text=True)

        if result.returncode != 0:
            print(
                f"Error processing {preprocess_file}: {result.stderr}", file=sys.stderr
            )
            return False

        # Check if output file was created
        if not os.path.exists(preprocess_file):
            print(f"Preprocess file {preprocess_file} was not created", file=sys.stderr)
            return False

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
