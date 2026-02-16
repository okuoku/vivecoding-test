#!/usr/bin/env python3
import sys
import json
import re
import argparse
import os
from collections import defaultdict


def parse_preprocessor_line(line):
    """Parse a preprocessor output line and return (filename, flags) or None"""
    if not line.startswith("#"):
        return None

    parts = line.split()
    if len(parts) < 3:
        return None

    try:
        line_num = int(parts[1])
    except ValueError:
        return None

    filename = parts[2].strip('"')
    flags = parts[3:]

    return (filename, flags)


def construct_event_list(preprocess_file_path):
    """Construct event list from preprocessor output"""
    events = []

    try:
        with open(preprocess_file_path, "r") as f:
            content = f.read()
    except FileNotFoundError:
        return events

    lines = content.split("\n")

    for line in lines:
        parsed = parse_preprocessor_line(line)
        if parsed is None:
            continue

        filename, flags = parsed

        if filename in ["<built-in>", "<command line>", "<stdin>"]:
            continue

        if not flags:
            continue

        try:
            flag_int = int(flags[0])
        except (ValueError, IndexError):
            continue

        events.append((filename, flag_int))

    return events


def construct_include_tree(events):
    """Construct include tree from event list"""
    EV_IN = 1
    EV_OUT = 2

    class Node:
        def __init__(self, name):
            self.name = name
            self.children = []

    root = None
    stack = []

    for filename, event_type in events:
        if root is None:
            root = Node(filename)
            stack = [root]
            continue

        if event_type == EV_IN:
            parent = stack[-1] if stack else root
            child = Node(filename)
            parent.children.append(child)
            stack.append(child)
        elif event_type == EV_OUT:
            while stack and stack[-1].name != filename:
                stack.pop()
            if not stack:
                stack.append(root)

    def serialize(node):
        if not node.children:
            return node.name
        return [node.name] + [serialize(child) for child in node.children]

    return serialize(root) if root else None


def compress_include_tree(include_tree, nodes):
    """Compress include tree using indices"""

    node_index_map = {node: idx for idx, node in enumerate(nodes)}

    def compress_tree(tree):
        if isinstance(tree, str):
            return node_index_map.get(tree, -1)
        elif isinstance(tree, list):
            return [compress_tree(item) for item in tree]
        return tree

    return {"files": [{"inctree": compress_tree(include_tree)}], "nodes": nodes}


def analyze_include_tree(input_file, output_file):
    """Analyze preprocessed files and generate include tree"""
    preprocess_entries = []
    nodes_set = set()

    for line in input_file:
        try:
            entry = json.loads(line)
        except json.JSONDecodeError:
            continue

        if "preprocess_file" not in entry:
            continue

        preprocess_file = entry["preprocess_file"]
        directory = entry.get("directory", ".")

        preprocess_path = preprocess_file
        if not os.path.isabs(preprocess_path):
            preprocess_path = os.path.join(directory, preprocess_file)

        events = construct_event_list(preprocess_path)
        include_tree = construct_include_tree(events)

        if include_tree:
            nodes_set.add(entry["file"])

        preprocess_entries.append(
            {
                "file": entry["file"],
                "directory": entry["directory"],
                "preprocess_file": entry["preprocess_file"],
                "inctree": include_tree,
            }
        )

    nodes = sorted(nodes_set)
    compressed_entries = []
    for entry in preprocess_entries:
        if entry["inctree"]:
            compressed = compress_include_tree(entry["inctree"], nodes)
            compressed_entries.append(compressed["files"][0])
        else:
            compressed_entries.append({"inctree": None})

    output = {"nodes": nodes, "files": compressed_entries}

    print(json.dumps(output))


def main():
    parser = argparse.ArgumentParser(description="Analyze include tree")
    parser.add_argument(
        "input_file", nargs="?", help="Input rs_project_preprocess.jsonl file"
    )
    parser.add_argument(
        "output_file", nargs="?", help="Output rs_project_include_tree.json file"
    )
    args = parser.parse_args()

    if args.input_file and args.output_file:
        with open(args.input_file, "r") as f, open(args.output_file, "w") as f_out:
            analyze_include_tree(f, f_out)
    elif args.input_file:
        analyze_include_tree(sys.stdin, sys.stdout)
    else:
        analyze_include_tree(sys.stdin, sys.stdout)


if __name__ == "__main__":
    main()
