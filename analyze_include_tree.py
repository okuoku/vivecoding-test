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
        import re

        m = re.match(r'^#\s+\d+\s+"([^"]+)"(.*)$', line)
        if not m:
            continue
        filename = m.group(1)
        # ignore built-in, command line, stdin
        if filename in {"<built-in>", "<command line>", "<stdin>"}:
            continue
        rest = m.group(2).strip()
        flags = [int(f) for f in rest.split()] if rest else []
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
    children_map = {}
    for name, ev in events:
        print(stack)
        if not root:
            root = name
            stack = [name]
            children_map.setdefault(name, [])
            continue
        if ev == "in":
            parent = stack[-1]
            children_map.setdefault(name, [])
            children_map[parent].append(name)
            stack.append(name)
        else:  # out
            while stack and stack[-1] != name:
                stack.pop()

    def serialize(node):
        ch = children_map.get(node, [])
        if not ch:
            return node
        return [node] + [serialize(c) for c in ch]

    return serialize(root)


def compress(tree, nodes_list):
    # assign indices
    idx_map = {}

    def build(node):
        if isinstance(node, str):
            if node not in idx_map:
                idx_map[node] = len(idx_map)
                nodes_list.append(node)
            return idx_map[node]
        # node is list
        if node[0] not in idx_map:
            idx_map[node[0]] = len(idx_map)
            nodes_list.append(node[0])
        return [idx_map[node[0]]] + [build(c) for c in node[1:]]

    return build(tree)


def main():
    result = {"nodes": [], "files": []}
    for line in sys.stdin:
        entry = json.loads(line)
        file_path = entry.get("preprocess_file")
        if not os.path.exists(file_path):
            continue
        with open(file_path, "r") as f:
            events = parse_events(f.readlines())
        tree = build_tree(events)
        nodes_list = []
        inctree = compress(tree, nodes_list)
        result["nodes"] = list(set(result["nodes"] + nodes_list))
        result["files"].append(
            {
                "file": entry.get("file"),
                "directory": entry.get("directory"),
                "preprocess_file": file_path,
                "inctree": inctree,
            }
        )
    json.dump(result, sys.stdout)


if __name__ == "__main__":
    main()
