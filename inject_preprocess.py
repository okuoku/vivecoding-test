#!/usr/bin/env python3
import argparse
import json
import sys


def process_arguments(arguments, basename, counter):
    if "-c" not in arguments:
        return None

    new_args = []
    i = 0
    while i < len(arguments):
        arg = arguments[i]
        if arg == "-c":
            new_args.append("-E")
        elif arg == "-o":
            i += 1
            preprocess_filename = f"{basename}-{counter}.i"
            new_args.append("-o")
            new_args.append(preprocess_filename)
        else:
            new_args.append(arg)
        i += 1

    return new_args, preprocess_filename


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", required=True, help="basename for temporary files")
    args = parser.parse_args()

    counter = 0
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        obj = json.loads(line)
        arguments = obj.get("arguments", [])

        result = process_arguments(arguments, args.b, counter)
        if result is None:
            continue

        new_args, preprocess_filename = result
        output = {
            "file": obj["file"],
            "directory": obj["directory"],
            "arguments": new_args,
            "preprocess_file": preprocess_filename,
        }
        print(json.dumps(output))
        counter += 1


if __name__ == "__main__":
    main()
