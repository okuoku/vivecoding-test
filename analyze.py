#!/usr/bin/env python3
"""
Script to analyze compile_commands.json and output rs_project_cmds.jsonl.
"""

import sys, json, shlex


def main():
    data = json.load(sys.stdin)
    for entry in data:
        if "arguments" in entry:
            args = entry["arguments"]
        elif "command" in entry:
            args = shlex.split(entry["command"])
        else:
            continue
        out = {
            "file": entry.get("file", ""),
            "directory": entry.get("directory", ""),
            "arguments": args,
        }
        sys.stdout.write(json.dumps(out) + "\n")


if __name__ == "__main__":
    main()
