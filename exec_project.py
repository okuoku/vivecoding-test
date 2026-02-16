#!/usr/bin/env python3
import sys
import json
import subprocess
import argparse


def exec_project(input_file):
    """Execute preprocess commands to generate preprocessed files"""
    for line in input_file:
        try:
            entry = json.loads(line)
        except json.JSONDecodeError:
            continue

        if "arguments" not in entry:
            continue

        arguments = entry["arguments"]
        directory = entry.get("directory", ".")

        try:
            subprocess.run(arguments, cwd=directory, check=True)

        except subprocess.CalledProcessError as e:
            print(f"Error executing command: {' '.join(arguments)}", file=sys.stderr)
            print(f"Return code: {e.returncode}", file=sys.stderr)
            print(f"stderr: {e.stderr}", file=sys.stderr)
            continue


def main():
    parser = argparse.ArgumentParser(description="Execute preprocess commands")
    parser.add_argument(
        "input_file", nargs="?", help="Input rs_project_preprocess.jsonl file"
    )
    args = parser.parse_args()

    if args.input_file:
        with open(args.input_file, "r") as f:
            exec_project(f)
    else:
        exec_project(sys.stdin)


if __name__ == "__main__":
    main()
