#!/usr/bin/env python3
import sys
import json
import argparse
import os
import re


def generate_temp_filename(basename, counter):
    """Generate temporary filename in format <basename>-<counter>.i"""
    return f"{basename}-{counter}.i"


def inject_preprocess(input_file, output_file, basename):
    """Inject preprocess options into compile commands"""
    counter = 0

    for line in input_file:
        try:
            entry = json.loads(line)
        except json.JSONDecodeError:
            continue

        if "arguments" not in entry:
            continue

        arguments = entry["arguments"]
        if "-c" not in arguments:
            continue

        new_arguments = []
        output_file_arg = None
        output_file_index = -1
        source_file = None
        source_file_index = -1

        for i, arg in enumerate(arguments):
            if arg == "-o":
                output_file_arg = arguments[i + 1]
                output_file_index = i
            elif arg.startswith("-"):
                continue
            else:
                source_file = arg
                source_file_index = i

        for i, arg in enumerate(arguments):
            if arg == "-c":
                new_arguments.append("-E")
            elif i == output_file_index:
                temp_filename = generate_temp_filename(basename, counter)
                new_arguments.append("-o")
                new_arguments.append(temp_filename)
                counter += 1
            elif arg == "-E":
                continue
            elif arg == output_file_arg:
                continue
            else:
                new_arguments.append(arg)

        if source_file and source_file_index != -1:
            if source_file not in new_arguments:
                new_arguments.append(source_file)

        if new_arguments:
            output_entry = {
                "file": entry.get("file", ""),
                "directory": entry.get("directory", ""),
                "arguments": new_arguments,
                "preprocess_file": generate_temp_filename(basename, counter - 1),
            }
            print(json.dumps(output_entry))


def main():
    parser = argparse.ArgumentParser(description="Inject preprocess options")
    parser.add_argument(
        "-b", "--basename", required=True, help="Basename for temporary files"
    )
    parser.add_argument(
        "input_file", nargs="?", help="Input rs_project_cmds.jsonl file"
    )
    parser.add_argument(
        "output_file", nargs="?", help="Output rs_project_preprocess.jsonl file"
    )
    args = parser.parse_args()

    if args.input_file and args.output_file:
        with open(args.input_file, "r") as f, open(args.output_file, "w") as f_out:
            inject_preprocess(f, f_out, args.basename)
    elif args.input_file:
        inject_preprocess(sys.stdin, sys.stdout, args.basename)
    else:
        inject_preprocess(sys.stdin, sys.stdout, args.basename)


if __name__ == "__main__":
    main()
