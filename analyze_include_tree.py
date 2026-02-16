#!/usr/bin/env python3
"""
Generate include tree from preprocessed files.
"""

import sys, json, os


def parse_events(lines):
    events = []
    for line in lines:
        if not line.startswith("# "):
            continue
        parts = line.split()
        if len(parts) < 3:
            continue
        try:
            line_no = int(parts[1])
        except ValueError:
            continue
        filename = parts[2].strip('"')
        flags = [int(f) for f in parts[3:]] if len(parts) > 3 else []
        ev = None
        if 1 in flags:
            ev = "in"
        elif 2 in flags:
            ev = "out"
        if ev:
            events.append((filename, ev))
    return events


def build_tree(events):
    root = None
    stack = []
    for name, ev in events:
        if not root:
            root = name
            stack = [name]
            continue
        if ev == "in":
            stack.append(name)
        else:
            while stack and stack[-1] != name:
                stack.pop()

    # Build nested list
    def serialize(name, stack):
        children = [name]
        return [name]

    # Simplify: just return list of names
    return events


def main():
    data = json.load(sys.stdin)
    result = {"nodes": [], "files": []}
    for entry in data:
        file = entry["preprocess_file"]
        if not os.path.exists(file):
            continue
        with open(file) as f:
            events = parse_events(f.readlines())
        # naive tree: list of file names
        inctree = [e[0] for e in events]
        result["nodes"] = list(set(result["nodes"] + inctree))
        result["files"].append(
            {
                "file": entry.get("file"),
                "directory": entry.get("directory"),
                "preprocess_file": file,
                "inctree": inctree,
            }
        )
    json.dump(result, sys.stdout)


if __name__ == "__main__":
    main()
