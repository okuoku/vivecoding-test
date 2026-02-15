# `patgen` spec

`patgen` is a random JSON testcase generator that replaces
placeholder string inside a JSON in specified pattern.

## Environment

`patgen` is a single Node.js script `patgen.mjs` that
written in plain JavaScript without any dependency.

- The script should be written in ES6 module syntax
- Access file with Node.js `node:fs` library

## Operation

- User will invoke the script as `node patgen.mjs input.json pattern.txt 30`
- `input.json` should contain a JSON object that should contain a string object "{{replace}}" as a placeholder
- `pattern.txt` should contain "patterns" that should contain replacement pattern
- `30` means the script should generate 30 json files. `patgen` should accept any integer here.

## Pattern replacement

If a pattern contained curly-brace pattern in their text,
`patgen` will choose content randomly inside of it.
For example, `{a|b|c}` will become either `a` or `b` or `c`.

There is no need to handle nested pattern nor escaping braces(`{` `}`)
or pipe(`|`).

## Examples

`input.json` should look like:

```json
{"a": null, "b": "non replaced string", 
 "nested": {"c": "{{replace}}"}}
```

`pattern.txt` should look like:

```
an anime {boy|girl} has {3|4|5} {green|red|blue} balls.
```

output JSON should be:
```json
{"a": null, "b": "non replaced string", 
 "nested": {"c": "an anime boy has 3 red balls."}}
```

or,

```json
{"a": null, "b": "non replaced string", 
 "nested": {"c": "an anime girl has 5 green balls."}}
```

