#!/usr/bin/env python3
import sys
import json
import subprocess
import os
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
        if "-E" not in arguments:
            continue

        directory = entry.get("directory", ".")
        preprocess_file = entry.get("preprocess_file", "")

        try:
            result = subprocess.run(
                arguments, cwd=directory, capture_output=True, text=True
            )

            if result.returncode != 0:
                print(
                    f"Error executing command: {' '.join(arguments)}", file=sys.stderr
                )
                print(f"Return code: {result.returncode}", file=sys.stderr)
                print(f"stderr: {result.stderr}", file=sys.stderr)
                continue

            if preprocess_file:
                output_path = os.path.join(directory, preprocess_file)
                output_dir = os.path.dirname(output_path)
                if output_dir and not os.path.exists(output_dir):
                    os.makedirs(output_dir, exist_ok=True)

                with open(output_path, "w") as f:
                    f.write(result.stdout)

        except subprocess.SubprocessError as e:
            print(f"Error executing command: {' '.join(arguments)}", file=sys.stderr)
            print(f"Error: {e}", file=sys.stderr)
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
