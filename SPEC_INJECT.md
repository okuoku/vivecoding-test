# `inject_preprocess.py` operational specs

`inject_preprocess.py` will read `rs_project_cmds.jsonl`
and convert contained command lines with following rules.

- Drop the line entirely if it do not contain any `-c` options in `arguments` field
- In `arguments` field, replace `-c` option with `-E` option
- In `arguments` field, generate temporary filename and replace current `-o`
  and its next element(ie. argument of `-o`) with `-o` `<generated-filename>.i`
- Add new field `preprocess_file` as `<generated-filename>.i` 

For `<generated-filename>.i`, `<generated-filename>` should be replaced with
the generate filename for the line. See next section for filename generation.

## Filename Generation

The script file should receive `-b` option to specify a "basename" of
the temporary file.

Temporary filename should be in format of:

```
<basename>-<counter>.i
```

For example, if the script run in:

```
./inject_preprocess.py -b check < /path/to/compile_commands.json > rs_project_preprocess.jsonl
```

Generated `rs_project_preprocess.jsonl` will contain:

- check-0.i
- check-1.i
- ...

as `preprocess_file` filenames.


