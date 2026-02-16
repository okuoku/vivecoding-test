import json
import sys
import os
import argparse
import re


def main():
    # Parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", required=True, help="Base name for temporary files")
    args = parser.parse_args()

    # Read input from stdin
    data = sys.stdin.read().strip()
    if not data:
        return

    # Process each line
    counter = 0
    for line in data.split("\n"):
        if not line.strip():
            continue

        try:
            # Parse JSON
            cmd = json.loads(line)
        except json.JSONDecodeError as e:
            print(f"Error parsing JSON: {e}", file=sys.stderr)
            continue

        # Check if command contains -c option
        if not any(arg == "-c" for arg in cmd["arguments"]):
            continue

        # Replace -c with -E
        arg_list = [arg if arg != "-c" else "-E" for arg in cmd["arguments"]]

        # Find -o option and its value
        filename = None
        for i, arg in enumerate(arg_list):
            if arg == "-o" and i + 1 < len(arg_list):
                # Replace -o and its value with -o <generated-filename>.i
                # Generate filename with basename and counter
                while True:
                    filename = f"{args.b}-{counter}.i"
                    counter += 1
                    break

                # Replace -o and its value
                arg_list[i] = "-o"
                arg_list[i + 1] = filename
                break

        # Add preprocess_file field
        cmd["preprocess_file"] = filename

        # Update arguments field
        cmd["arguments"] = arg_list

        # Output JSON
        print(json.dumps(cmd))


if __name__ == "__main__":
    main()
