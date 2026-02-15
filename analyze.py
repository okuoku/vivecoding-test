#!/usr/bin/env python3
"""
analyze.py - Analyzes compile_commands.json and emits rs_project_cmds.jsonl
"""

import json
import sys


def parse_command_line(command_str):
    """
    Parse a command line string into an array of arguments.
    This is a simplified version of shell command line parsing.
    """
    args = []
    current_arg = []
    in_quotes = False

    for char in command_str:
        if char == '"':
            in_quotes = not in_quotes
        elif char.isspace() and not in_quotes:
            if current_arg:
                args.append("".join(current_arg))
                current_arg = []
        else:
            current_arg.append(char)

    if current_arg or in_quotes:
        args.append("".join(current_arg))

    return args


def process_compile_commands(input_data):
    """
    Process compile_commands.json and convert to rs_project_cmds.jsonl format
    """
    if not isinstance(input_data, list):
        return

    for entry in input_data:
        # Ensure we have a dictionary
        if not isinstance(entry, dict):
            continue

        # Get arguments - either from 'arguments' field or parse 'command'
        if "arguments" in entry:
            arguments = entry["arguments"]
        elif "command" in entry:
            arguments = parse_command_line(entry["command"])
        else:
            continue

        # Get file and directory
        file_path = entry.get("file")
        directory = entry.get("directory")

        if not file_path or not arguments:
            continue

        # Create output object
        output = {"file": file_path, "directory": directory, "arguments": arguments}

        # Output as JSONL line
        print(json.dumps(output, ensure_ascii=False))


def main():
    # Read from stdin
    input_data = json.load(sys.stdin)
    process_compile_commands(input_data)


if __name__ == "__main__":
    main()
