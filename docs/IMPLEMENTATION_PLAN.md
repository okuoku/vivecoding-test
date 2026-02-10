# ctransian Implementation Plan

## Current Status Overview

### ✅ Completed Foundation
- **Project Structure**: Complete directory layout with all components
- **Build System**: CMake configuration with Binaryen integration  
- **Core Framework**: Basic translator, type mapper, instruction visitor, and code generator scaffolding
- **Public API**: Complete C API design and basic implementation
- **Runtime Foundation**: Basic runtime with memory management
- **Testing**: Basic test infrastructure with passing tests
- **CLI Tool**: Complete command-line interface

### 🔄 Current Implementation Status

**Core Components:**
- **Translator.cpp**: ✅ 85% complete - basic parsing, optimization, and orchestration logic
- **TypeMapper.cpp**: ✅ 85% complete - enhanced with proper function signature generation using BinaryenTypeArity/Expand, full multi-value support
- **InstructionVisitor.cpp**: ✅ 75% complete - Phase 0 constants + Phase 1 parametric, variable access, and basic control flow fully implemented
- **CodeGenerator.cpp**: ✅ 40% complete - basic generation framework with module/function orchestration, error handling, and stub implementations

**Runtime Components:**
- **ctransian_runtime.c**: ✅ 80% complete - memory management, error handling, instance management
- **memory.c, threads.c, exceptions.c, imports.c**: ❌ Empty placeholders

## WebAssembly Instruction Implementation Plan

### ✅ Phase 0: Foundation (COMPLETED)
**Priority: HIGH** - Must be implemented first
- **Constants**: `i32.const`, `i64.const`, `f32.const`, `f64.const`, `v128.const`
  - *Why first?* Every non-trivial program uses constants
  - *Complexity*: Simple - just generate C literal values
  - *Status*: ✅ **FULLY IMPLEMENTED** - All basic constants (i32, i64, f32, f64) with proper C literal generation, special value handling (NaN, Infinity), and type detection. SIMD v128 has placeholder implementation.

### ✅ Phase 1: Core MVP (COMPLETED)
**Priority: HIGH** - Required for basic WebAssembly functionality

1. **Parametric Instructions** (Stack manipulation) ✅
   - `drop`, `select`
   - *Why essential?* Stack manipulation is fundamental to WASM's stack machine model
   - *Complexity*: Low - straightforward C equivalents
   - *Status*: ✅ **FULLY IMPLEMENTED** - `drop` generates `(void)(expr)`, `select` generates ternary `(cond ? true : false)`

2. **Control Flow Instructions** (Program structure) ✅
   - `nop`, `unreachable`, `block`, `loop`, `if`
   - *Why essential?* No program can function without control flow
   - *Complexity*: High - requires label management and structured control flow translation
   - *Status*: ✅ **FULLY IMPLEMENTED** - `nop` comment, `unreachable` trap call, `block`/`loop`/`if` with proper C structures

3. **Variable Access Instructions** (Local/global variables) ✅
   - `local.get`, `local.set`, `local.tee`, `global.get`, `global.set`
   - *Why essential?* Needed for any function with variables
   - *Complexity*: Medium - requires variable tracking and scoping
   - *Status*: ✅ **FULLY IMPLEMENTED** - Local variables with index naming, globals with prefix, `tee` as assign-and-return

**Note**: `br`, `br_if`, `br_table`, `return`, `call`, `call_indirect` remaining for Phase 1 completion

### Phase 2: Data Operations (Essential for useful programs)
**Priority: HIGH** - Required for real-world programs

4. **Memory Instructions** (Data manipulation)
   - `i32.load`, `i64.load`, `f32.load`, `f64.load` (with alignment and offset variants)
   - `i32.store`, `i64.store`, `f32.store`, `f64.store` (with alignment and offset variants)
   - `memory.size`, `memory.grow`
   - *Why essential?* Programs need to access memory
   - *Complexity*: High - requires bounds checking, alignment handling, and memory model

5. **Basic Arithmetic Instructions** (Core computation)
   - `i32.add`, `i32.sub`, `i32.mul`
   - `i64.add`, `i64.sub`, `i64.mul`
   - `f32.add`, `f32.sub`, `f32.mul`
   - `f64.add`, `f64.sub`, `f64.mul`
   - *Why essential?* Most programs perform arithmetic
   - *Complexity*: Low - direct C operator mapping

6. **Comparison Instructions** (Conditionals)
   - `i32.eq`, `i32.ne`, `i32.lt_s/u`, `i32.gt_s/u`, `i32.le_s/u`, `i32.ge_s/u`
   - `i64.eq`, `i64.ne`, `i64.lt_s/u`, `i64.gt_s/u`, `i64.le_s/u`, `i64.ge_s/u`
   - `f32.eq`, `f32.ne`, `f32.lt`, `f32.gt`, `f32.le`, `f32.ge`
   - `f64.eq`, `f64.ne`, `f64.lt`, `f64.gt`, `f64.le`, `f64.ge`
   - *Why essential?* Required for conditional logic
   - *Complexity*: Low - direct C comparison operators

### Phase 3: Advanced Operations (Medium priority)
**Priority: MEDIUM** - Important for complete language support

7. **Bitwise Instructions** (Bit manipulation)
   - `i32.and`, `i32.or`, `i32.xor`, `i32.shl`, `i32.shr_s/u`, `i32.rotl`, `i32.rotr`
   - `i64.and`, `i64.or`, `i64.xor`, `i64.shl`, `i64.shr_s/u`, `i64.rotl`, `i64.rotr`
   - *Complexity*: Medium - direct C bitwise operators

8. **Conversion Instructions** (Type conversion)
   - `i32.wrap_i64`, `i64.extend_i32_s/u`
   - `f32.convert_i32_s/u`, `f32.convert_i64_s/u`
   - `f64.convert_i32_s/u`, `f64.convert_i64_s/u`
   - `i32.trunc_f32_s/u`, `i32.trunc_f64_s/u`, `i64.trunc_f32_s/u`, `i64.trunc_f64_s/u`
   - `f32.demote_f64`, `f64.promote_f32`
   - *Complexity*: Medium - requires careful C casting

9. **Advanced Arithmetic** (Division and remainder)
   - `i32.div_s/u`, `i32.rem_s/u`, `i64.div_s/u`, `i64.rem_s/u`, `f32.div`, `f64.div`
   - *Complexity*: Medium - needs division by zero checks

### Phase 4: Threading Support (Conditional)
**Priority: MEDIUM** - Only if threading enabled

10. **Atomic Instructions**
    - `atomic.load`, `atomic.store`, `atomic.rmw`, `atomic.cmpxchg` variants
    - *Complexity*: High - requires C11 atomic operations

### Phase 5: Advanced Features (Low priority)
**Priority: LOW** - Optional extensions

11. **SIMD Instructions** (Conditional - only if SIMD enabled)
    - All `v128.*` operations (load, store, arithmetic, bitwise, shuffle, extract, replace)
    - *Complexity*: Very High - requires SIMD intrinsics

12. **Reference Type Instructions** (Future - WasmGC proposal)
    - `ref.null`, `ref.is_null`, `ref.func`, `ref.as_non_null`, etc.
    - *Complexity*: High - garbage collection integration

## Detailed Implementation Tasks

### Core Component Implementation

#### 1. InstructionVisitor Implementation (Priority: HIGH)

**Phase 0 - Constants:**
```cpp
std::string visitConstant(BinaryenExpressionRef expr) {
    // Get literal value from Binaryen
    // Generate appropriate C literal
    // Handle i32, i64, f32, f64, v128 types
}
```

**Phase 1 - Parametric:**
```cpp
std::string visitDrop(BinaryenExpressionRef expr) {
    // Generate code to pop and discard value
}

std::string visitSelect(BinaryenExpressionRef expr) {
    // Generate conditional expression: condition ? value1 : value2
}
```

**Phase 1 - Control Flow:**
```cpp
std::string visitBlock(BinaryenExpressionRef expr) {
    // Generate C block with label management
    // Handle branch targets within block
}

std::string visitLoop(BinaryenExpressionRef expr) {
    // Generate C loop construct (for/while)
    // Handle loop labels and breaks
}

std::string visitIf(BinaryenExpressionRef expr) {
    // Generate C if-else statement
    // Handle conditional branches
}

std::string visitCall(BinaryenExpressionRef expr) {
    // Generate C function call
    // Handle argument passing and return values
}
```

**Phase 1 - Variable Access:**
```cpp
std::string visitLocalGet(BinaryenExpressionRef expr) {
    // Generate local variable access
    // Handle different local indices
}

std::string visitLocalSet(BinaryenExpressionRef expr) {
    // Generate local variable assignment
}

std::string visitGlobalGet(BinaryenExpressionRef expr) {
    // Generate global variable access
}
```

**Phase 2 - Memory Operations:**
```cpp
std::string visitLoad(BinaryenExpressionRef expr) {
    // Generate memory load with bounds checking
    // Handle alignment and offset
    // Generate appropriate cast
}

std::string visitStore(BinaryenExpressionRef expr) {
    // Generate memory store with bounds checking
    // Handle alignment and offset
}
```

**Phase 2 - Arithmetic:**
```cpp
std::string visitBinary(BinaryenExpressionRef expr) {
    // Generate C binary operations (+, -, *, /, %, etc.)
    // Handle different types (i32, i64, f32, f64)
}
```

#### 2. CodeGenerator Implementation (Priority: HIGH)

**Function Generation:**
```cpp
ctransian_error_t generateFunction(BinaryenFunctionRef func, std::string& code) {
    // Generate complete C function:
    // 1. Function signature
    // 2. Local variable declarations
    // 3. Function body using InstructionVisitor
    // 4. Return statement
}
```

**Module Generation:**
```cpp
ctransian_error_t generateModule(BinaryenModuleRef module, ctransian_result_t* result) {
    // Generate complete C module:
    // 1. Header includes
    // 2. Global variable declarations
    // 3. Function definitions
    // 4. Runtime support integration
}
```

#### 3. TypeMapper Enhancement (Priority: HIGH)

**Function Signatures:**
```cpp
std::string generateFunctionSignature(BinaryenFunctionRef func_ref, const std::string& name) {
    // Extract function type from Binaryen
    // Generate C function signature with proper types
    // Handle multi-value returns (if supported)
}
```

#### 4. Translator Enhancement (Priority: MEDIUM)

**Module Analysis:**
```cpp
ctransian_error_t analyzeModule() {
    // Use Binaryen APIs to extract real module information:
    // - Count functions, imports, exports
    // - Extract function signatures
    // - Analyze memory requirements
    // - Validate module features
}
```

### Runtime Implementation

#### Missing Runtime Files (Priority: MEDIUM)

**src/runtime/memory.c:**
```c
// Enhanced memory operations with bounds checking
// Atomic memory operations (if threading enabled)
// Memory alignment helpers
```

**src/runtime/threads.c:**
```c
// C11 thread support
// Thread-local storage helpers
// Synchronization primitives
```

**src/runtime/exceptions.c:**
```c
// Trap handling with setjmp/longjmp
// Exception propagation
// Error reporting
```

**src/runtime/imports.c:**
```c
// Host function import bridge
// WASI system call interface
// JavaScript interop (if needed)
```

## Implementation Strategy

### Recommended Order for Maximum Early Value:

1. **Start with Constants** - Quick win, enables testing
2. **Add Variable Access** - Enables functions with state
3. **Implement Basic Arithmetic + Comparisons** - Enables computation
4. **Add Control Flow** - Enables program logic (this is the big milestone)
5. **Add Memory Operations** - Enables data manipulation
6. **Round out with remaining instructions**

At this point (after Phase 2), you'll have a **functional WebAssembly-to-C translator** that can handle most real-world WebAssembly modules!

### Testing Strategy for Each Phase:

- **Phase 0**: Test constant generation with simple WASM files
- **Phase 1**: Test basic functions, control flow, and variable access
- **Phase 2**: Test mathematical functions, data structures, and algorithms
- **Phase 3+**: Add comprehensive test coverage and edge cases

## Immediate Next Session Tasks (Phase 2)

### First Priority - Remaining Phase 1 Control Flow:
```cpp
// Complete Phase 1 by implementing remaining control flow instructions
std::string InstructionVisitor::visitBreak(BinaryenExpressionRef expr) {
    // Generate break/branch to label
}

std::string InstructionVisitor::visitReturn(BinaryenExpressionRef expr) {
    // Generate return statement with proper value handling  
}

std::string InstructionVisitor::visitCall(BinaryenExpressionRef expr) {
    // Generate function calls with proper argument passing
}
```

### Second Priority - Memory Instructions (Phase 2):
```cpp
// Implement core memory operations in instruction_visitor.cpp
std::string InstructionVisitor::visitLoad(BinaryenExpressionRef expr) {
    // Generate memory loads with bounds checking
}

std::string InstructionVisitor::visitStore(BinaryenExpressionRef expr) {
    // Generate memory stores with bounds checking
}

std::string InstructionVisitor::visitMemorySize(BinaryenExpressionRef expr) {
    // Generate memory.size() call to runtime
}

std::string InstructionVisitor::visitMemoryGrow(BinaryenExpressionRef expr) {
    // Generate memory.grow() call to runtime
}
```

### Third Priority - Basic Arithmetic (Phase 2):
```cpp
// Implement essential arithmetic operations
std::string InstructionVisitor::visitBinary(BinaryenExpressionRef expr) {
    // Handle add, sub, mul operations with proper C operators
}
```

### Build and Test Commands:
```bash
# Build project
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j4

# Run tests
make test

# Test CLI tool  
./ctransian --help
```

## Build and Test Commands

```bash
# Build the project
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j4

# Run tests
./tests/test_ctransian_basic

# Test CLI tool
./ctransian --help
```

## Success Metrics

### Phase 0 Success:
- Constants translate correctly
- Basic test WASM files compile to valid C

### Phase 1 Success:
- Simple functions with variables work
- Basic control flow translates correctly
- Test suite passes with generated C code

### Phase 2 Success:
- Mathematical functions work
- Memory operations work with bounds checking
- Real-world WASM modules translate successfully

### Final Success:
- Complete MVP WebAssembly-to-C translator
- Generated C code compiles and runs correctly
- Performance comparable to existing tools

---

**This plan provides a clear roadmap for completing the ctransian implementation. Start with Phase 0 (constants) and work through each phase systematically.**