#!/usr/bin/env python3
import json
import re
import sys

EV_IN = 1
EV_OUT = 2

IGNORE_FILENAMES = {"<built-in>", "<command line>", "<stdin>"}


def parse_events(preprocessed_file):
    events = []
    pattern = re.compile(r'^#\s+(\d+)\s+"([^"]+)"(?:\s+(\d+))*$')

    try:
        with open(preprocessed_file, "r", encoding="utf-8", errors="ignore") as f:
            for line in f:
                line = line.rstrip("\n")
                if not line.startswith("# "):
                    continue

                match = pattern.match(line)
                if not match:
                    continue

                num = match.group(1)
                filename = match.group(2)

                if filename in IGNORE_FILENAMES:
                    continue

                parts = line.split()
                flags = []
                if len(parts) > 3:
                    flags = [int(f) for f in parts[3:] if f.isdigit()]

                ev_type = None
                if EV_IN in flags:
                    ev_type = EV_IN
                elif EV_OUT in flags:
                    ev_type = EV_OUT

                if ev_type is not None:
                    events.append((filename, ev_type))

    except FileNotFoundError:
        pass

    return events


def build_tree(events):
    if not events:
        return None

    class Node:
        def __init__(self, name):
            self.name = name
            self.children = []

    root = None
    stack = []

    for filename, ev in events:
        if root is None:
            root = Node(filename)
            stack.clear()
            stack.append(root)
            continue

        if ev == EV_IN:
            parent = stack[-1] if stack else root
            child = Node(filename)
            parent.children.append(child)
            stack.append(child)
        elif ev == EV_OUT:
            while stack and stack[-1].name != filename:
                stack.pop()
            if not stack:
                stack.append(root)

    def serialize(n):
        if n.children:
            return [n.name] + [serialize(c) for c in n.children]
        return n.name

    return serialize(root) if root else None


def compress_tree(tree, nodes_dict, next_index):
    if tree is None:
        return None

    if isinstance(tree, str):
        if tree not in nodes_dict:
            nodes_dict[tree] = next_index[0]
            next_index[0] += 1
        return nodes_dict[tree]

    if isinstance(tree, list):
        return [compress_tree(item, nodes_dict, next_index) for item in tree]

    return tree


def main():
    files = []
    nodes_dict = {}
    next_index = [0]

    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        obj = json.loads(line)

        file_path = obj.get("file")
        directory = obj.get("directory", ".")
        preprocess_file = obj.get("preprocess_file")

        if not preprocess_file:
            continue

        full_path = f"{directory}/{preprocess_file}"
        events = parse_events(full_path)
        tree = build_tree(events)

        compressed = compress_tree(tree, nodes_dict, next_index)

        files.append(
            {
                "file": file_path,
                "directory": directory,
                "preprocess_file": preprocess_file,
                "inctree": compressed,
            }
        )

    nodes_list = [None] * len(nodes_dict)
    for name, idx in nodes_dict.items():
        nodes_list[idx] = name

    output = {"nodes": nodes_list, "files": files}

    print(json.dumps(output, indent=2))


if __name__ == "__main__":
    main()
