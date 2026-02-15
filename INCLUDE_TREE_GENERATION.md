# "Include tree" generation algorithm

"include tree" is a file dependency tree of a C/C++ source file that
constructed by `#include` preprocessor operation which is observed by
previous `exec_project.py` operation.

## Output structure

`rs_project_include_tree.json` file will contain a JSON object that has:

- `nodes` : Array of paths that will be used with "include tree" notation
- `files` : Array of entries that corresponds to `rs_project_preprocess.jsonl` lines

Its `files` entry is an array and each array entries are a JSON object that contains:

- `file` : Path of source file (copied from `rs_project_preprocess.jsonl` entry)
- `directory` : Path of directory (ditto)
- `preprocess_file` : Copy of `preprocess_file` entry for debug (ditto)
- `inctree` : "include tree" object of the file

"Include tree" is a JSON object that describes file dependency tree of the file.

## "include tree" generation

"include tree" for a file will be generated from its preprocessor output
(`preprocess_file`). Preprocessor output is actually a event stream
and the tree is just a alternative representation of the events.

### Algorithm overview

The algorithm will fold a event stream -- preprocessed file -- into
a tree("include tree") that is denoted by a recursive JSON array.

In a "include tree," each array elements are tree node that means:

- A string(e.g. `"./c.h"`) is a file without any other dependencies(ie. leaf)
- An array(e.g. `["./b.h", "./c.h"]`) means the first element was
  included by the parent. Following elements are the its dependencies.

To save disk space, actual "include tree" output will be compressed with indices
and array of strings.

Overall, "include tree" generation algorithm can be summarized with
following steps:

1. Construct event list from preprocessor output
2. Filter out unused events
3. Construct In/Out event list
4. Construct actual "include tree"
5. Compress the result with indices

### 1: Constructing event list

A preprocessor output should be look like:

```
# 1 "temp.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 466 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "temp.c" 2
line

# 1 "./b.h" 1
# 1 "./c.h" 1
# 2 "./b.h" 2
# 4 "temp.c" 2

line
```

Preprocessor output can be considered as ordered event list.
Each line should denote a single event.

Lines starting `#` and following single space contains space-separated fields. 
A line of preprocessor output should be parsed in following algorithm:

1. Skip the line that was not started with `#` character and following single space
2. Skip the line if the first field is not a numeric value
3. Take second value as double-quoted string of `filename`
4. Take third and beyond values as numeric `flags`

### 2: Filter out unnecessary events

If `filename` was one of these values, we can ignore the event.

`<built-in>` `<command line>` `<stdin>`

### 3: Construct In/Out event stream

Each event can be considered as either an `in` or `out` event.

- If `flags` contained numeric value `1` the event is a `in` event
- If `flags` contained numeric value `2` the event is an `out` event
- If `flags` did not contain any of value above, ignore the event

For example,

```
# 1 "./b.h" 1
```

means, the event is `in` event that denotes starting
processing of `./b.h` file.

```
# 4 "temp.c" 2
```

means, the event is `out` event that denotes we have finished
previous file processing and backed to `temp.c` context.

With these `in` and `out` events, we can construct a tree, the "include tree"
from them.

### 4: Constructing tree

Ignoring lines not relevant here, the example above can be reduced 
to following in/out events:

```
# 1 "temp.c" 2
# 1 "./b.h" 1
# 1 "./c.h" 1
# 2 "./b.h" 2
# 4 "temp.c" 2
```

This example should be converted to a JSON array just like:

```json
["temp.c", ["b.h", "c.h"]]
```

Constructing algorithm can be described following JavaScript pseudo code:

```javascript
const EV_IN = 1
const EV_OUT = 2

// Above example in JavaScript notation
const EVENTS = [
  ["temp.c", EV_OUT],
  ["./b.h", EV_IN],
  ["./c.h", EV_IN],
  ["./b.h", EV_OUT],
  ["temp.c", EV_OUT],
];

function convert(events) { // => ARRAY
  const makeNode = (name) => ({ name, children: [] });

  let root = null;
  const stack = [];

  for(const [file, ev] of events){
    // Root init (first real file we see)
    if(!root){
      root = makeNode(file);
      stack.length = 0;
      stack.push(root);
      continue;
    }

    if(ev === EV_IN){
      const parent = stack[stack.length - 1] ?? root;
      const child = makeNode(file);
      parent.children.push(child);
      stack.push(child);
    }else if(ev === EV_OUT){
      // Return to `file`: pop until stack top matches `file`
      while (stack.length && stack[stack.length - 1].name !== file) {
        stack.pop();
      }
      // If markers are odd and we popped everything, recover to root
      if (!stack.length) stack.push(root);
    }
  }

  // Convert to final notation
  const serialize = (n) =>
  n.children.length ? [n.name, ...n.children.map(serialize)] : n.name;

  return root ? serialize(root) : null;
}

// Prints ["temp.c", ["b.h", "c.h"]]
console.log(convert(EVENTS));
```

### 5: Compressing output with indices

To prevent bloating output, we will convert "include tree" into array of
indices.

Above example will be encoded as

```json
{
  "files": [{"inctree": [0, [1, 2]]}],
  "nodes": ["temp.c", "b.h", "c.h"]
}
```

On actual `rs_project_include_tree.json` file,
`files` fields will include multiple entities that shares single
`nodes` array so we can expect further space saving.
