import json
import sys
import os
import re


def parse_preprocessor_output(output):
    # Parse preprocessor output
    events = []
    for line in output.split("\n"):
        if not line.strip() or not line.startswith("#"):
            continue

        # Parse line: # <line_number> <filename> <flags>
        parts = re.split(r"\s+", line.strip(), maxsplit=3)
        if len(parts) < 3:
            continue

        try:
            line_num = int(parts[0][1:])  # Remove # prefix
            filename = parts[1].strip('"')
            flags = list(map(int, parts[2].split(",")))
        except ValueError:
            continue

        # Filter out unnecessary events
        if filename in ["<built-in>", "<command line>", "<stdin>"]:
            continue

        # Determine if it's an in or out event
        if 1 in flags:
            events.append((filename, "in"))
        elif 2 in flags:
            events.append((filename, "out"))

    return events


def build_include_tree(events):
    # Build include tree from events
    # Using the algorithm described in the documentation
    # Convert events to a JSON array structure
    # Initialize stack and root
    stack = []
    root = []

    for filename, event in events:
        if event == "in":
            # Push to stack
            if stack:
                # Add to parent's children
                stack[-1].append(filename)
            else:
                # This is the first file, so add to root
                root.append(filename)
            stack.append([filename])
        elif event == "out":
            # Pop from stack
            if stack:
                stack.pop()
            # If stack becomes empty, reset to root
            if not stack:
                stack.append(root)

    return root


def main():
    # Read input from stdin
    data = sys.stdin.read().strip()
    if not data:
        return

    # Parse input JSONL
    files = []
    for line in data.split("\n"):
        if not line.strip():
            continue

        try:
            cmd = json.loads(line)
        except json.JSONDecodeError as e:
            print(f"Error parsing JSON: {e}", file=sys.stderr)
            continue

        # Get preprocessed file path
        preprocess_file = cmd.get("preprocess_file", "")
        if not preprocess_file:
            continue

        # Check if file exists
        if not os.path.exists(preprocess_file):
            continue

        # Read preprocessed file
        try:
            with open(preprocess_file, "r") as f:
                output = f.read()
        except Exception as e:
            print(f"Error reading {preprocess_file}: {e}", file=sys.stderr)
            continue

        # Parse preprocessor output
        events = parse_preprocessor_output(output)

        # Build include tree
        include_tree = build_include_tree(events)

        # Add to files array
        files.append(
            {
                "file": cmd["file"],
                "directory": cmd["directory"],
                "preprocess_file": preprocess_file,
                "inctree": include_tree,
            }
        )

    # Create nodes array
    nodes = []
    for file in files:
        # Flatten the include tree to get all nodes
        def flatten(tree):
            if isinstance(tree, str):
                return [tree]
            else:
                result = []
                for item in tree:
                    if isinstance(item, str):
                        result.append(item)
                    else:
                        result.extend(flatten(item))
                return result

        nodes.extend(flatten(file["inctree"]))

    # Remove duplicates
    nodes = list(set(nodes))

    # Output JSON
    output_data = {"files": files, "nodes": nodes}
    print(json.dumps(output_data))


if __name__ == "__main__":
    main()
