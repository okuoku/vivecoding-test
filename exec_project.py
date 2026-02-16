import json
import subprocess
import os
import sys


def main():
    # Read input from stdin
    data = sys.stdin.read().strip()
    if not data:
        return

    # Process each line
    for line in data.split("\n"):
        if not line.strip():
            continue

        try:
            # Parse JSON
            cmd = json.loads(line)
        except json.JSONDecodeError as e:
            print(f"Error parsing JSON: {e}", file=sys.stderr)
            continue

        # Get directory and arguments
        directory = cmd.get("directory", "")
        if not directory:
            continue

        args = cmd.get("arguments", [])
        if not args:
            continue

        # Get output file path
        output_file = None
        for i, arg in enumerate(args):
            if arg == "-o" and i + 1 < len(args):
                output_file = args[i + 1]
                break

        if not output_file:
            continue

        # Change to directory and run command
        try:
            os.chdir(directory)
        except OSError as e:
            print(f"Error changing directory to {directory}: {e}", file=sys.stderr)
            continue

        # Run the command
        try:
            subprocess.run(args, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error executing command {args[0]}: {e}", file=sys.stderr)
            continue


if __name__ == "__main__":
    main()
