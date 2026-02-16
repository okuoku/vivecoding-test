#!/usr/bin/env python3
import json
import sys


def parse_includes(file_path):
    # Simple include parser for .i files (C preprocessor output)
    includes = []
    try:
        with open(file_path, "r") as f:
            for line in f:
                if line.startswith('#include "'):
                    start = len('#include "')
                    end = line.find('"', start)
                    if end != -1:
                        includes.append(line[start:end])
    except Exception as e:
        print(f"Error parsing {file_path}: {e}", file=sys.stderr)
    return includes


def main():
    # Read from stdin
    input_data = sys.stdin.read()
    if not input_data:
        return

    try:
        entries = [json.loads(line) for line in input_data.strip().split("\n")]
    except json.JSONDecodeError as e:
        print(f"Error parsing JSONL: {e}", file=sys.stderr)
        return

    # Build include tree
    include_tree = {}
    for entry in entries:
        includes = parse_includes(entry["preprocess_file"])
        include_tree[entry["file"]] = {
            "includes": includes,
            "directory": entry["directory"],
        }

    # Output as JSON
    print(json.dumps(include_tree, indent=2))


if __name__ == "__main__":
    main()
