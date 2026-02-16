#!/usr/bin/env python3
import json
import sys


def main():
    for line in sys.stdin:
        try:
            data = json.loads(line)
            preprocess_file = f"{data.get('file', '')}.i"
            output = {**data, "preprocess_file": preprocess_file}
            print(json.dumps(output))
        except json.JSONDecodeError:
            continue


if __name__ == "__main__":
    main()
