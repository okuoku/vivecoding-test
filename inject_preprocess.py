#!/usr/bin/env python3
"""
Replace command arguments for analysis.
"""

import sys, json, argparse


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-b", "--basename", required=True)
    args = parser.parse_args()
    counter = 0
    for line in sys.stdin:
        obj = json.loads(line)
        if not any(a.startswith("-c") for a in obj.get("arguments", [])):
            continue
        new_args = []
        i = 0
        temp = None
        while i < len(obj["arguments"]):
            a = obj["arguments"][i]
            if a == "-c":
                new_args.append("-E")
                i += 1
            elif a == "-o" and i + 1 < len(obj["arguments"]):
                temp = f"{args.basename}-{counter}.i"
                new_args.extend(["-o", temp])
                counter += 1
                i += 2
            else:
                new_args.append(a)
                i += 1
        obj["arguments"] = new_args
        if temp is not None:
            obj["preprocess_file"] = temp
        print(json.dumps(obj))


if __name__ == "__main__":
    main()
