#!/usr/bin/env python3
"""
inject_preprocess.py - Replace command arguments for analysis
"""

import json
import sys


def inject_preprocess(jsonl_lines, basename):
    """
    Process rs_project_cmds.jsonl and inject preprocess commands
    """
    counter = 0

    for line in jsonl_lines:
        try:
            entry = json.loads(line.strip())
        except json.JSONDecodeError:
            continue

        # Check if arguments contains -c option
        has_c_option = False
        arguments = entry.get("arguments", [])

        for i, arg in enumerate(arguments):
            if arg == "-c":
                has_c_option = True
                break

        if not has_c_option:
            continue

        # Create preprocess filename
        preprocess_filename = f"{basename}-{counter}.i"
        counter += 1

        # Replace -c with -E and replace -o with preprocess file
        new_arguments = []
        i = 0
        while i < len(arguments):
            arg = arguments[i]

            if arg == "-c":
                new_arguments.append("-E")
                i += 1
            elif arg == "-o" and i + 1 < len(arguments):
                new_arguments.append("-o")
                new_arguments.append(preprocess_filename)
                i += 2
            else:
                new_arguments.append(arg)
                i += 1

        # Create output entry
        output = {
            "file": entry.get("file"),
            "directory": entry.get("directory"),
            "arguments": new_arguments,
            "preprocess_file": preprocess_filename,
        }

        print(json.dumps(output, ensure_ascii=False))


def main():
    import argparse

    parser = argparse.ArgumentParser(description="Inject preprocess commands")
    parser.add_argument(
        "-b", "--basename", required=True, help="Basename for preprocess files"
    )

    args = parser.parse_args()

    # Read from stdin
    for line in sys.stdin:
        inject_preprocess([line], args.basename)


if __name__ == "__main__":
    main()
