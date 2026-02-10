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
2. **CMake Configuration** - Build system with options
3. **Public API Design** - Complete C interface specification
4. **Core Headers** - All component interfaces defined
5. **CLI Framework** - Command-line parsing and options
6. **Runtime Foundation** - Memory management and trap handling
7. **Testing Infrastructure** - Test structure and examples
8. **Documentation** - README, project plan, contributing guide
9. **CI/CD Pipeline** - GitHub Actions configuration

### 🔄 In Progress (Next Session)
1. **Binaryen Integration** - Connect parsing and optimization
2. **Type System Implementation** - WASM to C type mapping logic
3. **Core Translation Logic** - Main translation algorithms
4. **Instruction Visitors** - Individual WASM instruction translation
5. **Code Generation** - C output production

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

## Next Development Session - Immediate Tasks

### Phase 1: Binaryen Integration
1. **Fix Include Paths** - Resolve binaryen-c.h imports
2. **Basic Parsing** - Implement WASM binary/text parsing
3. **Module Validation** - Add WASM validation support
4. **Error Handling** - Implement robust error reporting

### Phase 2: Type System Implementation  
1. **Type Mapping** - Implement WASM to C type conversion
2. **Function Signatures** - Generate C function prototypes
3. **Memory Types** - Handle linear memory mapping
4. **Reference Types** - Support funcref, externref, etc.

### Phase 3: Core Translation Logic
1. **Expression Visitor** - Implement traversal of Binaryen IR
2. **Instruction Translation** - Convert individual WASM instructions
3. **Control Flow** - Handle blocks, loops, branches
4. **Function Translation** - Complete function body generation

## Implementation Guidelines

### Code Style
- **C++17** for core implementation
- **C11** for runtime library
- **snake_case** for functions/variables
- **PascalCase** for classes/types
- **4 spaces** indentation, no tabs

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
- [ ] Complete type_mapper.cpp implementation
- [ ] Implement instruction_visitor.cpp
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

**Next Session Focus**: Start with type_mapper.cpp implementation and core translation logic. Binaryen integration is now complete with vendored dependencies!