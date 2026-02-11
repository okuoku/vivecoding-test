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

- Uses CMake 3.20+ to build entire project including tests
- Add bundled(submoduled) `binaryen` and `wabt` as `add_subdirectory`
- Use latest stable versions of vendored dependencies
- Vendored Binaryen should be used to keep stability
- Vendored WABT should be used for tests(only)
- Project should be configured at `build/` directory with Ninja generator
  with CMake. (`.clangd` includes hardcoded path to this directory)

Project's test infrastructure:

- `test/**/*.wat` contains test suite written in WebAssembly
  text format.
- Vendored Wabt `wat2wasm` command should be used to assemble
  WebAssembly sources into `.wasm` binary to be used as test cases
- Generated C99 sources should be compiled with same compiler
  as `wacgen` command compiled
- Test verification: compilation checking only (no runtime execution)
- Test data: minimal test cases for easy review

## Directory Structure

```
wacgen/
├── CMakeLists.txt
├── src/
│   ├── main.c (CLI entry point)
│   ├── wacgen_core.c (conversion logic)
│   ├── wacgen_core.h
│   └── codegen/
│       ├── codegen.h
│       ├── codegen.c
│       └── templates.h (C function templates)
├── runtime/
│   ├── wacgen_rt.c
│   └── wacgen_rt.h
├── tests/
├── docs/plan/
│   ├── 01_Framework.md
│   └── 02_wasm/
│       └── [instruction groups].md
├── ext/
│   ├── binaryen (submodule)
│   └── wabt (submodule)
├── STATUS.md
└── PLAN.md
```


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
  declared and a pointer to context(`wacgenrt_ctx`)
  - The context should be the first argument
- A special function that returns array of:
  - functions
  - variables
  that will be named as `__wacgen_<basename>_init`
  where `<basename>` is specified in the commandline

### Code generation conventions

- Symbol naming: `snake_case` for generated symbols, preserve original WebAssembly module symbols
- WebAssembly module symbols prefixed with `__{basename}_` where `{basename}` is CLI-specified
- Formatting: TAB is softtab in 4 spaces (concise formatting)
- Error handling: leave to users
- Memory bounds checking: C macro with no-op implementation
- Abort calls: wrapped in C macro (e.g., `WACGEN_ABORT()`)
- Context structure (`wacgenrt_ctx`): contains pointers to functions, memories, tables

### `wacgen_rt` Runtime Library

To implement WebAssembly interfacing and memory operation,
separate shared library `wacgen_rt` will be implemented.

It will contain:

- `wacgenrt_ctx_init` -- Initializes context with memory, tables, globals
- `wacgenrt_ctx_destroy` -- Destroys context
- `wacgenrt_grow_memory` -- Memory growth management
- `wacgenrt_table_get` -- Table operations for indirect calls
- `wacgenrt_table_set` -- Table operations for indirect calls

Memory and safety macros:

- `WACGEN_CHECK_BOUNDS(ptr, size)` -- Memory bound checking (no-op implementation)
- `WACGEN_ABORT()` -- Abort wrapper macro

Every generated function will take a pointer to opaque structure
`wacgenrt_ctx` to denote its context. Context structure contains
pointers to functions, memories, tables.

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

### Key Binaryen C API Functions to Use:
- `BinaryenModuleRead()` - Load WebAssembly binary
- `BinaryenModuleDispose()` - Clean up module
- `BinaryenModuleGetNumFunctions()` - Count functions
- `BinaryenModuleGetFunction()` - Access individual functions
- `BinaryenFunctionGetNumParams()` - Get parameter count
- `BinaryenFunctionGetNumResults()` - Get return value count
- `BinaryenFunctionGetNumLocals()` - Get local variable count
- `BinaryenFunctionGetBody()` - Get function body expression
- `BinaryenExpressionGetId()` - Identify instruction types
- `BinaryenExpressionGetChildren()` - Walk instruction tree

### C Function Templates:
- Function template for each WebAssembly function
- Initialization function template (`__wacgen_{basename}_init`)
- Context structure definitions
- Memory and safety macros

## Phase2: Document for WebAssembly instructions

Group WebAssembly instructions logically, prioritizing implementation convenience:

- `01_control_flow.md` - Basic control flow (`nop`, `block`, `loop`, `if`, `br`, `br_if`, `return`)
- `02_integer.md` - Integer arithmetic (`i32/i64` operations, bitwise, shifts)
- `03_float.md` - Floating point arithmetic (`f32/f64` operations, comparisons, conversions)
- `04_memory.md` - Memory operations (`load`, `store`, `memory.size`, `memory.grow`)
- `05_simd.md` - SIMD operations (vector operations, lane access)
- `06_advanced.md` - Advanced features (atomic ops, reference types, exception handling)

For each instruction, document:
- Binaryen C API instruction symbol
- WebAssembly specification summary
- Translation strategy to C

## Phase3: Implement simple `nop` module conversion

- Write minimal `.wat` file containing single function with `nop` only:
```wat
(module
  (func $nop_func (export "nop_func")
    nop
  )
)
```
- Add `tests/CMakeLists.txt` to assemble `.wat` files using `wat2wasm`
- Implement C99 source and header generation for that function
- Generated C function template:
```c
void __{basename}_nop_func(wacgenrt_ctx* ctx) {
    // WebAssembly nop -> empty statement in C
}
```
- Test compilation of generated code

## Phase4: Implement branch(control flow) instructions

- Write `.wat` files with control flow instructions:
  - Simple if-then blocks
  - Loop constructs
  - Branch targets
  - Function returns with values
- Add them to `test/` and connect to buildsystem
- Implement C99 source generation for these instructions
- Test compilation of generated code

## Phase5: Extend instruction support

- Pick any unimplemented instruction group documented in Phase2
- Write minimal `.wat` test files for that group
- Add CMake build rules for new tests
- Implement translation logic for instructions
- Test compilation only (no runtime execution needed)
- Update `STATUS.md` with progress

Repeat Phase5 until all Phase2 documented instructions are implemented and tested.

### Status Tracking

Maintain `STATUS.md` with:
```markdown
# Implementation Status

## Completed Features
- [x] Project structure and CMake setup
- [x] Phase 1: Framework documentation  
- [x] Phase 2: Instruction grouping
- [x] Phase 3: NOP conversion
- [ ] Phase 4: Control flow instructions
- [ ] Phase 5: Additional instruction groups

## Supported WebAssembly Instructions
| Group | Instructions | Status |
|-------|-------------|---------|
| Control Flow | nop, block, loop | 🔄 In Progress |
| Integer | i32.add, i32.sub | ⏳ Not Started |
```

### Development iteration

On development iteration, configure the project on `/build`
directory. This directory must not be commited to the Git history.
(already added in `.gitignore`)

