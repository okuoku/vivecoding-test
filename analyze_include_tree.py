#!/usr/bin/env python3
import json
import sys
from collections import defaultdict


def parse_include_lines(lines):
    include_tree = defaultdict(list)
    for line in lines:
        if line.startswith("#include"):
            parts = line.split()
            if len(parts) >= 2 and parts[1].startswith("<"):
                header = parts[1][1:-1]  # Remove < and >
                include_tree[header].append(line)
    return include_tree


def main():
    input_data = sys.stdin.read()
    try:
        data_list = [
            json.loads(line) for line in input_data.splitlines() if line.strip()
        ]
    except json.JSONDecodeError as e:
        print(f"Error decoding JSON: {e}", file=sys.stderr)
        sys.exit(1)

    include_trees = []
    for data in data_list:
        try:
            with open(data["preprocess_file"], "r") as f:
                lines = f.readlines()
                include_tree = parse_include_lines(lines)
                output = {**data, "include_tree": dict(include_tree)}
                include_trees.append(output)
        except (KeyError, FileNotFoundError) as e:
            print(
                f"Error processing file {data.get('preprocess_file', '')}: {e}",
                file=sys.stderr,
            )

    print(json.dumps({"include_trees": include_trees}))


if __name__ == "__main__":
    main()
