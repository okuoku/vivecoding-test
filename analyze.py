#!/usr/bin/env python3
import sys
import json
import shlex
import argparse


def parse_command_line(command_str):
    """Parse a command line string into an array of strings."""
    return shlex.split(command_str)


def analyze_compile_commands(input_file, output_file):
    """Analyze compile_commands.json and emit rs_project_cmds.jsonl"""
    if isinstance(input_file, str):
        try:
            with open(input_file, "r") as f:
                compile_commands = json.load(f)
        except FileNotFoundError:
            print(f"Error: {input_file} not found", file=sys.stderr)
            sys.exit(1)
        except json.JSONDecodeError as e:
            print(f"Error: Invalid JSON in {input_file}: {e}", file=sys.stderr)
            sys.exit(1)
    else:
        compile_commands = json.load(input_file)

    for entry in compile_commands:
        if "command" in entry:
            entry["arguments"] = parse_command_line(entry["command"])
        if "arguments" not in entry:
            continue
        if "file" not in entry:
            continue
        if "directory" not in entry:
            continue

        output_line = {
            "file": entry["file"],
            "directory": entry["directory"],
            "arguments": entry["arguments"],
        }
        print(json.dumps(output_line))


def main():
    parser = argparse.ArgumentParser(description="Analyze compile_commands.json")
    parser.add_argument(
        "input_file", nargs="?", help="Input compile_commands.json file"
    )
    parser.add_argument(
        "output_file", nargs="?", help="Output rs_project_cmds.jsonl file"
    )
    args = parser.parse_args()

    if args.input_file and args.output_file:
        with open(args.output_file, "w") as f:
            analyze_compile_commands(args.input_file, f)
    elif args.input_file:
        analyze_compile_commands(args.input_file, sys.stdout)
    else:
        analyze_compile_commands(sys.stdin, sys.stdout)


if __name__ == "__main__":
    main()
