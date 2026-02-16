import json
import sys
import shlex


def main():
    # Read input from stdin
    data = sys.stdin.read().strip()
    if not data:
        return

    # Parse the JSON data
    try:
        commands = json.loads(data)
    except json.JSONDecodeError as e:
        print(f"Error parsing JSON: {e}", file=sys.stderr)
        sys.exit(1)

    # Process each command
    for cmd in commands:
        # Handle command field if present
        if "command" in cmd:
            # Parse command using shlex to handle proper shell parsing
            args = shlex.split(cmd["command"])
        else:
            # Use arguments field if present
            args = cmd.get("arguments", [])

        # Get file path
        file_path = cmd.get("file", "")
        if not file_path:
            continue

        # Get directory path
        directory = cmd.get("directory", "")
        if not directory:
            continue

        # Output JSONL format
        output = {"file": file_path, "directory": directory, "arguments": args}
        print(json.dumps(output))


if __name__ == "__main__":
    main()
