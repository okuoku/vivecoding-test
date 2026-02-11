# `wacgen` Project Plan

`wacgen` is a WebAssembly to C translator that uses Binaryen to
parse WebAssembly.

## Overview

`wacgen` command:

- Simple CLI that generates C99 source files from a `.wasm` file
- CLI written in standard C99 and uses Binaryen C interface
- To prevent generating too large C source code, it will optionally
  take source mapping file that splits WebAssembly module into
  multiple C99 sources
- Generated C99 sources will be part of other program. Typically
  compiled as a shared library and loaded by another program.
- `wacgen` supports WebAssembly 3.0 features

Project's build infrastructure:

- Uses CMake to build entire project including tests
- Add bundled(submoduled) `binaryen` and `wabt` as `add_subdirectory`
- Vendored Binaryen should be used to keep stability
- Vendored WABT should be used for tests(only)

Project's test infrastructure:

- `test/**/*.wat` contains test suite written in WebAssembly
  text format.
- Vendored Wabt `wasm-as` command should be used to assemble
  WebAssembly sources into `.wasm` binary to be used as test cases
- Generated C99 sources should be compiled with same compiler
  as `wacgen` command compiled


### `wacgen` CLI

`wacgen` comes with simple CLI to convert a WebAssembly module
into C source(s).

- `-b` option specifies "basename" that will be used as part of
  generated symbols
- When `-b` option suppressed, basename of WebAssembly module
  will be used instead.

### Generated source content

Generated source will implement for each original WebAssembly 
module's functions and variables.

- Every function will take arguments that original module
  decleared and a pointer to context(`wacgenrt_ctx`)
  - The context should be the first argument
- A special function that returns array of:
  - functions
  - variables
  that will be named as `__wacgen_<basename>_init`
  where `<basename>` is specified in the commandline

### `wacgen_rt` Runtime Library

To implement WebAssembly interfacing and memory operation,
separate shared library `wacgen_rt` will be implemented.

It will contain:

- `wacgenrt_ctx_init` -- Initializes context
- `wacgenrt_ctx_destroy` -- Destroys context

Every generated function will take a pointer to opaque structure
`wacgenrt_ctx` to denote its context.

`wacgen_rt` will implements callbacks that needed to be called from
generated C99 sources such as:

- `wacgenrt_grow_memory` -- To request growing memory

### Usage exsample: Simple conversion

Simple generation will generate a C99 source and C99 header.
The header contains function 

```sh
wacgen -b basename input.wasm # -> generates basename.c and .h
```

Generated header should include prototypes for exported functions.

Generated source code can be compiled / linked against `wacgen_rt`
runtime library. Separate loader will load the shared library 
and run it.

```sh
cc -fPIC -o basename.o basename.c
cc -shared -o basename.so -lwacgen_rt basename.o
```

### Usage example: Split to multiple files

```sh
wacgen -b basename -m map.txt input.wasm
```

`map.txt` will list symbols that should be included into each 
sources

```text:map.txt
region_01:
hello_001
hello_002

region_02:
sampleA
sampleB
```

This command generates:
- `basename__region_01.c` `basename__region_01.h` that includes
  translated `hello_001` and `hello_002` function
- `basename__region_02.c` `basename__region_02.h` that includes
  translated `sampleA` and `sampleB` function
- `basename.c` and `basename.h` should include rest symbols

# Implementation plan

To make sure every coder on the same page, we use "document then 
implement" strategy here.

## Phase1: Document for conversion framework

Write `docs/plan/01_Framework.md` that should contain:

- Binaryen C API that need to be used to load WebAssembly
  and walk its function/variable and its instructions
- C function templates that should be part of generated C99 code

## Phase2: Document for WebAssembly instructions

- Group WebAssembly instructions into several groups(branch,
  integer arithmetic, floating point arithmetic, SIMD, exception
  handling, ...) and write their:
    - Binaryen C API instruction symbol
    - Its specification
  Into `docs/plan/02_wasm/{group name here}.md`

## Phase3: Implement simple `nop` module conversion

- Write `.wat` file that contains single function with `nop` (only)
  and place it under `tests` directory
- Add `tests/CMakeLists.txt` to assembles that `.wat` file
- Implement C99 source and header generation for that function
- Make sure we can test the project with them

## Phase4: Implement branch(control flow) instructions

- Write `.wat` files that contains branch instructions in addition
  of `nop`
- Add them to `test/` and connect these to the buildsystem
- Implement C99 source generation for these instructions

## Phase5: Extend instruction support

- Pick any unimplemented instruction group that documented in the
  Phase2
- Write `.wat` files that contains instructions in chosen group
- Implement these instructions conversions

Repeat Phase5 until every Phase2 documented instructions being
implemented and tested.

Write and Update `/STATUS.md` to denote current implementation 
status.
