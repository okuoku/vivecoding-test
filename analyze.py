#!/usr/bin/env python3
import json
import sys


def main():
    for line in sys.stdin:
        try:
            data = json.loads(line)
            output = {
                "file": data.get("file", ""),
                "directory": data.get("directory", ""),
                "arguments": data.get("arguments", []),
            }
            print(json.dumps(output))
        except json.JSONDecodeError:
            continue


if __name__ == "__main__":
    main()
