#!/usr/bin/env python3
import json
import sys
import argparse


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--behavior", choices=["check", "replace"], required=True)
    args = parser.parse_args()

    # Read from stdin
    input_data = sys.stdin.read()
    if not input_data:
        return

    try:
        entries = [json.loads(line) for line in input_data.strip().split("\n")]
    except json.JSONDecodeError as e:
        print(f"Error parsing JSONL: {e}", file=sys.stderr)
        return

    # Process each entry
    for entry in entries:
        new_args = [arg for arg in entry["arguments"] if not arg.endswith(".c")]
        new_args.append("-E")  # Preprocess only
        new_args.append("-o")
        new_args.append(entry["file"] + ".i")  # Output to .i file

        new_entry = entry.copy()
        new_entry["arguments"] = new_args
        new_entry["preprocess_file"] = entry["file"] + ".i"
        print(json.dumps(new_entry))


if __name__ == "__main__":
    main()
