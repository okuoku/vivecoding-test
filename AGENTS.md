# ctransian Development Agents.md

This file contains context and instructions for continuing development of the ctransian WebAssembly to C translator in future sessions.

## Project Overview

**ctransian** is an advanced WebAssembly to C translator powered by Binaryen's optimization pipeline. It aims to provide superior performance and feature support compared to existing tools like wasm2c and w2c2.

### Key Differentiators
- **Binaryen Integration**: Leverages Binaryen's advanced optimization passes vs direct translation
- **Modern C11 Support**: Full threading, atomics, and modern language features
- **Comprehensive WASM Support**: Support for most WebAssembly proposals including SIMD, GC, threading
- **Performance Focus**: 2-5x faster performance than interpretation approaches

### Current Status
✅ **Foundation Complete** - Project structure, build system, API design, CLI framework, basic tests
🔄 **Next Phase** - Core translation engine implementation with Binaryen integration

## Architecture Overview

```
WebAssembly Input → Binaryen Parser → Binaryen IR → Optimizations → 
Type Analysis → Code Generation → C11 Output + Runtime
```

### Core Components
1. **Translation Engine** (`src/core/translator.h/cpp`)
   - Main orchestrator using Binaryen
   - Pipeline coordination and error handling

2. **Type Mapper** (`src/core/type_mapper.h`) 
   - WASM types to C types mapping
   - Function signature generation

3. **Instruction Visitor** (`src/core/instruction_visitor.h`)
   - Visitor pattern for WASM instructions
   - Expression tree traversal and C generation

4. **Code Generator** (`src/core/code_generator.h`)
   - Final C code output generation
   - Runtime integration and optimization

5. **Public API** (`src/api/ctransian.cpp`, `include/ctransian/public.h`)
   - C interface for external integration
   - Configuration management

6. **CLI Tool** (`src/cli/main.cpp`)
   - Command-line interface with comprehensive options
   - File I/O and user interaction

7. **Runtime Library** (`src/runtime/ctransian_runtime.c/h`)
   - Memory management and bounds checking
   - Trap handling and error management
   - Threading and atomic operations support

## Development Context

### Build System
- **CMake 3.15+** with modern configuration
- **Configurable Features**: SIMD, threading, GC, WASI support
- **Multi-platform**: Linux, macOS, Windows support
- **Vendored Dependencies**: Binaryen as git submodule (third_party/binaryen)

### Dependencies
- **Required**: None (Binaryen is vendored as submodule)
- **Optional**: Threads (C11), SIMD intrinsics, GC support
- **Vendored**: Binaryen library (third_party/binaryen)

### Testing Framework
- **Unit Tests**: Individual component testing
- **Integration Tests**: End-to-end translation testing  
- **Performance Tests**: Benchmarking vs existing tools
- **Conformance Tests**: WebAssembly official test suite

## Current Implementation Status

### ✅ Completed Foundation
1. **Project Structure** - All directories and files created
2. **CMake Configuration** - Build system with options, Binaryen integration working
3. **Public API Design** - Complete C interface specification with basic implementation
4. **Core Headers** - All component interfaces defined
5. **CLI Framework** - Command-line parsing and options, complete implementation
6. **Runtime Foundation** - Memory management and trap handling (80% complete)
7. **Testing Infrastructure** - Test structure with passing basic tests
8. **Documentation** - README, project plan, implementation plan, contributing guide
9. **Build System** - Successfully builds and tests pass

### 🔄 Current Implementation Status (Detailed)
- **Translator.cpp**: ✅ 85% complete - basic parsing, optimization, and orchestration logic
- **TypeMapper.cpp**: ✅ 60% complete - basic type mappings, missing function signatures
- **InstructionVisitor.cpp**: ✅ 25% complete - Phase 0 constants fully implemented, other instructions still stub methods
- **CodeGenerator.cpp**: ❌ 10% complete - only stub methods exist
- **ctransian_runtime.c**: ✅ 80% complete - memory management, error handling
- **Runtime files**: ❌ Empty - memory.c, threads.c, exceptions.c, imports.c need implementation

### 🔄 Next Session Priority Tasks
1. **InstructionVisitor Implementation** - Phase 1: Parametric instructions, control flow, variable access
2. **CodeGenerator Implementation** - Generate proper C code output
3. **TypeMapper Enhancement** - Complete function signature generation
4. **Runtime Files** - Implement missing runtime components

### ⏳ Future Phases
1. **Advanced Features** - SIMD, threading, GC support
2. **Performance Optimization** - Advanced optimization passes
3. **Toolchain Integration** - Build system integration
4. **Production Readiness** - Error handling, robustness

## Key Files to Continue Development

### Core Implementation Files
```cpp
// Primary files needing implementation:
src/core/translator.cpp         // ✅ Basic implementation done
src/core/type_mapper.cpp          // ❌ Empty - needs full implementation  
src/core/instruction_visitor.cpp   // ❌ Empty - needs full implementation
src/core/code_generator.cpp       // ❌ Empty - needs full implementation
```

### Build Configuration
```cmake
CMakeLists.txt                   // ✅ Complete configuration
include/ctransian/config.h.in     // ✅ Feature flags and configuration
cmake/CtransianConfig.cmake.in   // ✅ Package configuration
```

### API Files  
```cpp
src/api/ctransian.cpp            // ✅ Basic C API implementation
include/ctransian/public.h        // ✅ Complete public API specification
include/ctransian/config.h.in    // ✅ Configuration template
```

### Runtime Files
```c
src/runtime/ctransian_runtime.c   // ✅ Basic runtime implementation
include/ctransian/ctransian_runtime.h // ✅ Runtime API specification
```

### CLI Files
```cpp
src/cli/main.cpp                  // ✅ Complete CLI implementation
```

## Development Commands

### Building
```bash
# Initialize submodules (if needed)
git submodule update --init --recursive

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCTRANIAN_ENABLE_SIMD=ON
make
```

**Note**: The `/build` directory is ignored by Git (see `.gitignore`). Use it for local testing and development only.

### Testing
```bash
make test                    # Run all tests
ctest --verbose             # Detailed test output
ctest -R TestName         # Run specific tests
```

### Development Setup
```bash
# Initialize and update submodules (includes Binaryen)
git submodule update --init --recursive

# No external Binaryen installation needed - it's vendored!
```

## Next Development Session - Implementation Plan

### **WebAssembly Instruction Implementation Strategy**

Based on WebAssembly specification and practical translation needs, the implementation should follow this prioritized approach (see `docs/IMPLEMENTATION_PLAN.md` for details):

**✅ Phase 0: Foundation (COMPLETED)**
- **Constants**: `i32.const`, `i64.const`, `f32.const`, `f64.const`, `v128.const`
- *Why first?* Every non-trivial program uses constants
- *Status*: ✅ **FULLY IMPLEMENTED** - All basic constants (i32, i64, f32, f64) with proper C literal generation, special value handling (NaN, Infinity), and type detection. SIMD v128 has placeholder implementation.

**Phase 1: Core MVP (Essential for any functional program)**
1. **Parametric Instructions** - `drop`, `select` (stack manipulation)
2. **Control Flow Instructions** - `nop`, `unreachable`, `block`, `loop`, `if`, `br`, `br_if`, `br_table`, `return`, `call`, `call_indirect` (program structure)
3. **Variable Access Instructions** - `local.get`, `local.set`, `local.tee`, `global.get`, `global.set` (variables)

**Phase 2: Data Operations (Essential for useful programs)**
4. **Memory Instructions** - `i32.load`, `i64.load`, `f32.load`, `f64.load`, `i32.store`, `i64.store`, `f32.store`, `f64.store`, `memory.size`, `memory.grow` (data manipulation)
5. **Basic Arithmetic Instructions** - `i32.add`, `i32.sub`, `i32.mul`, etc. (core computation)
6. **Comparison Instructions** - `i32.eq`, `i32.ne`, `i32.lt_s/u`, etc. (conditionals)

**Phase 3: Advanced Operations (Medium priority)**
7. **Bitwise Instructions** - `i32.and`, `i32.or`, `i32.xor`, etc.
8. **Conversion Instructions** - `i32.wrap_i64`, `i64.extend_i32_s/u`, etc.
9. **Advanced Arithmetic** - `i32.div_s/u`, `i32.rem_s/u`, etc.

**Phase 4-5: Advanced Features**
10. **Atomic Instructions** (threading support)
11. **SIMD Instructions** (conditional - if SIMD enabled)
12. **Reference Type Instructions** (WasmGC proposal)

### **Immediate Next Session Tasks (Phase 1)**

**First Priority - Parametric Instructions:**
```cpp
// In src/core/instruction_visitor.cpp
std::string InstructionVisitor::visitDrop(BinaryenExpressionRef expr) {
    // Generate code to pop and discard value from stack
}

std::string InstructionVisitor::visitSelect(BinaryenExpressionRef expr) {
    // Generate conditional expression: condition ? value1 : value2
}
```

**Second Priority - Variable Access Instructions:**
```cpp
// Implement local.get, local.set, local.tee, global.get, global.set
std::string InstructionVisitor::visitLocalGet(BinaryenExpressionRef expr) {
    // Generate local variable access with proper indexing
}

std::string InstructionVisitor::visitLocalSet(BinaryenExpressionRef expr) {
    // Generate local variable assignment
}
```

**Third Priority - TypeMapper Enhancement:**
```cpp
// Complete function signature generation in src/core/type_mapper.cpp
std::string generateFunctionSignature(BinaryenFunctionRef func_ref, const std::string& name) {
    // Extract function type from Binaryen
    // Generate C function signature with proper types
    // Handle multi-value returns (if supported)
}
```

### **Success Metrics**
- **✅ Phase 0 Success**: Constants translate correctly, basic test WASM files compile to valid C
- **Phase 1 Success**: Simple functions with variables work, basic control flow translates correctly
- **Phase 2 Success**: Mathematical functions work, memory operations work with bounds checking
- **Final Success**: Complete MVP WebAssembly-to-C translator

## Commit Guidelines

### Build Testing Rules
- **Always keep the repository in buildable state** - Test the build locally before committing
- **Exception for documentation** - Skip build testing for changes that don't affect the build (e.g., documentation updates, non-code files)

### Build Validation
Before committing code changes, run:
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

## Implementation Guidelines

### Code Style
- **C++17** for core implementation
- **C11** for runtime library
- **snake_case** for functions/variables
- **PascalCase** for classes/types
- **4 spaces** indentation, no tabs

### Documentation Synchronization
- **Always update `docs/` in sync** with source code changes
- **When editing source files**: Update corresponding documentation files immediately
- **Documentation files to maintain**:
  - `docs/API.md` - Update when changing public API (`src/api/`, `include/ctransian/public.h`)
  - `docs/ARCHITECTURE.md` - Update when changing core architecture (`src/core/`)
  - `docs/RUNTIME.md` - Update when changing runtime implementation (`src/runtime/`)
  - `docs/IMPLEMENTATION_PLAN.md` - Update when changing implementation status or adding new features
  - `docs/CLI.md` - Update when changing CLI options or behavior (`src/cli/`)
  - `README.md` - Update for major feature changes or status updates
- **Documentation update workflow**:
  1. Make source code change
  2. Immediately update relevant documentation files
  3. Verify documentation reflects the actual implementation
  4. Test both code and documentation examples
- **Documentation quality**: Keep examples working, maintain accuracy, update status indicators

### Memory Management
- **RAII** in C++ code
- **Smart pointers** where appropriate
- **Explicit cleanup** in C code
- **Bounds checking** for all memory access

### Error Handling
- **Return codes** for C API
- **Exceptions** for C++ internal code
- **Detailed messages** for debugging
- **Graceful failure** for unsupported features

### Performance Considerations
- **Avoid unnecessary allocations**
- **Minimize memory copies**
- **Use Binaryen optimizations**
- **Profile before optimizing**

## Testing Strategy

### Unit Tests
- Test each component independently
- Mock dependencies where needed
- Cover edge cases and error conditions
- Verify type mappings and conversions

### Integration Tests  
- Test complete translation pipeline
- Use real WASM modules as input
- Verify generated C compiles and runs correctly
- Compare with reference implementations

### Performance Tests
- Benchmark translation speed
- Measure generated code performance
- Compare with existing tools (wasm2c, w2c2)
- Profile memory usage

### Conformance Tests
- WebAssembly official test suite
- WABT compatibility tests
- Real-world application validation

## Build Options for Development

### Feature Flags
```cmake
-DCTRANIAN_ENABLE_SIMD=ON       # Enable SIMD support
-DCTRANIAN_ENABLE_THREADS=ON     # Enable threading  
-DCTRANIAN_ENABLE_GC=OFF         # Disable WasmGC (not ready)
-DCTRANIAN_ENABLE_WASI=ON        # Enable WASI support
-DCTRANIAN_BUILD_TESTS=ON         # Build test suite
```

### Debug Configuration
```cmake
-DCMAKE_BUILD_TYPE=Debug         # Debug builds with symbols
-DCTRANIAN_DEBUG_BUILD=ON       # Enable debug assertions
-DCTRANIAN_LOG_LEVEL=TRACE       # Verbose logging
```

## Known Issues and Limitations

### Current Dependencies
- **None External** - All dependencies are vendored or optional
- **Binaryen Integration** - Successfully vendored and integrated
- **Config Generation** - CMake config.h.in needs to be generated properly

### TODOs
- [x] Vendor Binaryen as git submodule
- [x] Update CMakeLists.txt for vendored Binaryen
- [x] Implement Phase 0 constants in instruction_visitor.cpp
- [ ] Complete type_mapper.cpp implementation
- [ ] Implement Phase 1 instructions (parametric, control flow, variable access)
- [ ] Complete code_generator.cpp
- [ ] Add comprehensive test cases
- [ ] Performance benchmarking
- [ ] Documentation of API usage

## External Resources

### WebAssembly Resources
- [WebAssembly Official Site](https://webassembly.org/)
- [Binaryen Documentation](https://github.com/WebAssembly/binaryen)
- [WebAssembly Specification](https://webassembly.github.io/spec/)
- [WABT Project](https://github.com/WebAssembly/wabt)

### Related Projects for Reference
- [w2c2](https://github.com/turbolent/w2c2) - Alternative WASM to C translator
- [wasm2c](https://github.com/WebAssembly/wabt/tree/main/wasm2c) - WABT's WASM to C tool
- [wastrel](https://github.com/leaningtech/wastrel) - New WASM to C project with GC goals

---

**Next Session Focus**: Start with Phase 1 implementation - parametric instructions, control flow, and variable access. Phase 0 constants are fully implemented and working!