#!/usr/bin/env python3
"""
analyze_include_tree.py - Analyze preprocessed files for include tree
"""

import json
import sys
import os


def parse_preprocessor_events(filepath):
    """
    Parse preprocessor output and extract include events
    Returns list of [filename, flags] tuples
    """
    events = []

    try:
        with open(filepath, "r") as f:
            for line in f:
                line = line.strip()

                # Check if line starts with '# ' pattern
                if len(line) >= 2 and line[0] == "#" and line[1] == " ":
                    parts = line[2:].split()

                    if not parts:
                        continue

                    # First part should be a number (line number)
                    if not parts[0].isdigit():
                        continue

                    # Second part should be a quoted filename
                    if len(parts) < 2:
                        continue

                    # Extract filename (remove quotes)
                    filename = parts[1].strip('"')

                    # Third and beyond should be flags
                    if len(parts) < 3:
                        continue

                    try:
                        flags = list(map(int, parts[2:]))
                        events.append([filename, flags])
                    except ValueError:
                        pass
    except Exception as e:
        print(f"Error reading {filepath}: {e}", file=sys.stderr)

    return events


def filter_events(events):
    """
    Filter out unnecessary events (built-in, command line, stdin)
    """
    ignore_files = {"<built-in>", "<command line>", "<stdin>"}
    return [
        [filename, flags] for filename, flags in events if filename not in ignore_files
    ]


def extract_in_out_events(events):
    """
    Extract in/out events from filtered events
    Returns list of [filename, event_type] where event_type is 1 (in) or 2 (out)
    """
    in_out_events = []

    for filename, flags in events:
        if 1 in flags and 2 not in flags:
            # In event
            in_out_events.append([filename, 1])
        elif 2 in flags and 1 not in flags:
            # Out event
            in_out_events.append([filename, 2])

    return in_out_events


def build_include_tree(events):
    """
    Build include tree from in/out events
    Returns a nested structure representing the tree
    """
    if not events:
        return None

    class TreeNode:
        def __init__(self, name):
            self.name = name
            self.children = []

    root = None
    stack = []

    for file, ev in events:
        if ev == 1:  # In event
            node = TreeNode(file)

            if not root:
                # First real file becomes root
                root = node
                stack = [root]
            else:
                # Add as child of current top
                parent = stack[-1] if stack else root
                parent.children.append(node)
                stack.append(node)

        elif ev == 2:  # Out event
            # Pop until we find matching file or reach root
            while stack and stack[-1].name != file:
                stack.pop()

            # If we popped everything, recover to root
            if not stack and root:
                stack.append(root)

    return root


def serialize_tree(node):
    """
    Serialize tree to the required format
    """
    if not node:
        return None

    if not node.children:
        return node.name

    return [node.name] + [serialize_tree(child) for child in node.children]


def compress_tree(tree, nodes):
    """
    Compress tree using node indices
    Returns [index, ...] structure
    """
    if not tree:
        return None

    if isinstance(tree, str):
        # Leaf node - add to nodes list and return index
        if tree not in nodes:
            nodes.append(tree)
        return nodes.index(tree)

    # Array node - recursively compress children
    if isinstance(tree, list) and len(tree) > 0:
        # First element is the node name
        if isinstance(tree[0], str):
            index = compress_tree(tree[0], nodes)
            children = tree[1:]
        else:
            index = compress_tree(tree, nodes)
            children = []

        # Compress all children
        compressed_children = [compress_tree(child, nodes) for child in children]

        return [index] + compressed_children

    return None


def process_jsonl(jsonl_lines):
    """
    Process rs_project_preprocess.jsonl and generate include tree
    """
    files = []
    all_nodes = []

    for line in jsonl_lines:
        try:
            entry = json.loads(line.strip())
        except json.JSONDecodeError:
            continue

        preprocess_file = entry.get("preprocess_file")
        if not preprocess_file or not os.path.exists(preprocess_file):
            continue

        # Parse events from preprocessed file
        events = parse_preprocessor_events(preprocess_file)
        filtered_events = filter_events(events)
        in_out_events = extract_in_out_events(filtered_events)

        # Build tree
        root_node = build_include_tree(in_out_events)

        if not root_node:
            continue

        # Serialize and compress tree
        serialized = serialize_tree(root_node)
        compressed_nodes = []
        compressed_tree = compress_tree(serialized, compressed_nodes)

        # Collect all unique nodes
        for node in compressed_nodes:
            if node not in all_nodes:
                all_nodes.append(node)

        # Create file entry
        file_entry = {
            "file": entry.get("file"),
            "directory": entry.get("directory"),
            "preprocess_file": preprocess_file,
            "inctree": compressed_tree,
        }

        files.append(file_entry)

    # Generate final output
    result = {"nodes": all_nodes, "files": files}

    print(json.dumps(result, ensure_ascii=False))


def main():
    # Read from stdin
    process_jsonl(sys.stdin)


if __name__ == "__main__":
    main()
