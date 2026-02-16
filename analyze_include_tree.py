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
    Returns a nested structure representing the tree (root node)
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
        if not root:
            # First event - create root regardless of type
            root = TreeNode(file)
            if ev == 1:
                # If it's an IN event, add to stack
                stack.append(root)
            # If it's an OUT event, don't add to stack yet
            continue

        if ev == 1:  # In event
            node = TreeNode(file)
            parent = stack[-1] if stack else root
            parent.children.append(node)
            stack.append(node)

        elif ev == 2:  # Out event
            # Return to `file`: pop until stack top matches `file`
            while stack and stack[-1].name != file:
                stack.pop()

    return root


def serialize_tree(node):
    """
    Serialize tree to the required format
    Returns: string for leaf, [name, child1, child2...] for node with children
    """
    if not node:
        return None

    # Convert children recursively
    children = [serialize_tree(child) for child in node.children]

    # If node has children, return [name, child1, child2...]
    if children:
        return [node.name] + children

    # Leaf node: just the name
    return node.name


def compress_tree(tree, nodes):
    """
    Compress tree using node indices
    Returns [index, ...] structure where index points to nodes array
    """
    if not tree:
        return None

    # Collect all unique node names from the tree
    def collect_nodes(obj):
        if isinstance(obj, str):
            return [obj]
        elif isinstance(obj, list):
            result = []
            # First element of a list is the node name
            if obj and isinstance(obj[0], str):
                result.append(obj[0])
            # Then collect from children
            for item in obj[1:]:
                if item is not None:
                    result.extend(collect_nodes(item))
            return result
        return []

    all_node_names = collect_nodes(tree)
    unique_nodes = sorted(list(set(all_node_names)))

    # Update the nodes list with unique nodes (only add new ones)
    for node in unique_nodes:
        if node not in nodes:
            nodes.append(node)

    # Now compress the tree structure using indices from the global nodes array
    def compress(obj):
        if isinstance(obj, str):
            return nodes.index(obj)
        elif isinstance(obj, list):
            # Only compress non-None items
            return [compress(item) if item is not None else None for item in obj]
        return None

    return compress(tree)


def process_jsonl(jsonl_lines):
    """
    Process rs_project_preprocess.jsonl and generate include tree
    """
    files = []
    all_nodes = []  # Global nodes list shared across all files

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

        # Serialize tree: leaf -> string, node with children -> [name, child1, child2...]
        serialized = serialize_tree(root_node)

        # Compress tree using indices
        compressed_nodes = []
        compressed_tree = compress_tree(serialized, all_nodes)

        # Create file entry
        file_entry = {
            "file": entry.get("file"),
            "directory": entry.get("directory"),
            "preprocess_file": preprocess_file,
            "inctree": compressed_tree,
        }

        files.append(file_entry)

    # Generate final output with shared nodes array
    result = {"nodes": all_nodes, "files": files}

    print(json.dumps(result, ensure_ascii=False))


def main():
    # Read from stdin
    process_jsonl(sys.stdin)


if __name__ == "__main__":
    main()
