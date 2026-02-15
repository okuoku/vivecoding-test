# `analyze.py` operational specs

## format of `compile_commands.json`

`compile_commands.json` is a JSON file that is array of JSON object
that contains following fields.

If more detail is required, you can refer https://clang.llvm.org/docs/JSONCompilationDatabase.html

- `command` : (optional) A command line to execute
- `arguments` : (optional) Array of strings that contain the command line
- `file` : Path to target source file

## `command` field translation

If the object in `compile_commands.json` included `command` field,
it must be converted to `arguments` field.
To convert `command` fields, parse the string into array of strings,
just same as Shell's command line parsing algorithm.

## Output a `rs_project_cmds.jsonl` line

To emit a line of the file, follow these steps
for each array entry of input `compile_commands.json` :

1. Get an object from the input
2. If the object contained `command` field, convert it into `arguments`
3. Emit `file` `arguments` `directory` fields as single-line JSON


