# ctransian Project Plan

## Project Overview

**Project Name:** `ctransian` (C + translation + "-ian" suffix suggesting "one who translates")

**Mission:** To create the most advanced WebAssembly to C translator by leveraging Binaryen's optimization capabilities while providing comprehensive modern C support.

## Technical Architecture

### Translation Pipeline

```mermaid
graph TD
    A[WASM Binary/WAT] --> B[Binaryen Parser]
    B --> C[Binaryen IR]
    C --> D[Binaryen Optimizer]
    D --> E[IR Analysis & Type System]
    E --> F[ctransian Code Generator]
    F --> G[C11 Source + Headers]
    G --> H[ctransian Runtime Library]
    
    I[ctransian CLI] --> A
    J[ctransian C Library API] --> A
    K[Build System] --> G
```

### Core Components

1. **Translation Engine** (`src/core/`)
   - `translator.h/cpp`: Main translation orchestrator
   - `type_mapper.h/cpp`: WASM types to C types mapping
   - `instruction_visitor.h/cpp`: Visitor pattern for WASM instructions
   - `code_generator.h/cpp`: C code generation utilities

2. **CLI Interface** (`src/cli/`)
   - Command-line argument parsing
   - File I/O handling
   - Configuration management

3. **Public API** (`src/api/`)
   - C API headers and implementation
   - Context management
   - Configuration structures

4. **Runtime Library** (`src/runtime/`)
   - Memory management
   - Thread support
   - Exception handling
   - Import/Export bridges

## Feature Support Matrix

| Category | Features | Status | Notes |
|----------|----------|--------|-------|
| **Core WASM** | i32, i64, f32, f64, control flow | ✅ Planned | Full MVP support |
| **SIMD** | v128 operations, vector intrinsics | ✅ Planned | GCC/Clang intrinsics |
| **Threading** | Shared memory, atomic operations | ✅ Planned | C11 threads |
| **Memory** | Multi-memory, bulk operations | ✅ Planned | Bounds checking |
| **GC Types** | struct, array, string references | 🔄 Future | WasmGC proposal |
| **WASI** | System interface | 🔄 Future | Full WASI support |
| **Exception** | try/catch, throw | 🔄 Future | setjmp/longjmp |

## Implementation Phases

### Phase 0: Foundation (Start Here) - **IMMEDIATE**
**Priority: HIGH** - Must be implemented first
- [x] Project structure and build system
- [x] Binaryen integration setup (vendored)
- [x] Core headers and API design
- [ ] Constants: `i32.const`, `i64.const`, `f32.const`, `f64.const`, `v128.const`

### Phase 1: Core MVP (Essential for any functional program)
**Priority: HIGH** - Required for basic WebAssembly functionality
- [ ] Parametric Instructions: `drop`, `select` (stack manipulation)
- [ ] Control Flow: `nop`, `unreachable`, `block`, `loop`, `if`, `br`, `br_if`, `br_table`, `return`, `call`, `call_indirect`
- [ ] Variable Access: `local.get`, `local.set`, `local.tee`, `global.get`, `global.set`

### Phase 2: Data Operations (Essential for useful programs)
**Priority: HIGH** - Required for real-world programs
- [ ] Memory Instructions: `i32.load`, `i64.load`, `f32.load`, `f64.load`, `i32.store`, `i64.store`, `f32.store`, `f64.store`, `memory.size`, `memory.grow`
- [ ] Basic Arithmetic: `i32.add`, `i32.sub`, `i32.mul`, `i64.add`, `i64.sub`, `i64.mul`, `f32.add`, `f32.sub`, `f32.mul`, `f64.add`, `f64.sub`, `f64.mul`
- [ ] Comparisons: `i32.eq`, `i32.ne`, `i32.lt_s/u`, `i32.gt_s/u`, `i32.le_s/u`, `i32.ge_s/u`, `f32.eq`, `f32.ne`, `f32.lt`, `f32.gt`, `f32.le`, `f64.ge`

### Phase 3: Advanced Operations (Medium priority)
**Priority: MEDIUM** - Important for complete language support
- [ ] Bitwise Instructions: `i32.and`, `i32.or`, `i32.xor`, `i32.shl`, `i32.shr_s/u`, `i32.rotl`, `i32.rotr`
- [ ] Conversion Instructions: `i32.wrap_i64`, `i64.extend_i32_s/u`, `f32.convert_i32_s/u`, `f32.convert_i64_s/u`
- [ ] Advanced Arithmetic: `i32.div_s/u`, `i32.rem_s/u`, `i64.div_s/u`, `i64.rem_s/u`, `f32.div`, `f64.div`

### Phase 4: Threading Support (Conditional)
**Priority: MEDIUM** - Only if threading enabled
- [ ] Atomic Instructions: `atomic.load`, `atomic.store`, `atomic.rmw`, `atomic.cmpxchg` variants
- [ ] C11 thread support runtime implementation

### Phase 5: Advanced Features (Low priority)
**Priority: LOW** - Optional extensions
- [ ] SIMD Instructions: All `v128.*` operations (conditional - only if SIMD enabled)
- [ ] Reference Type Instructions: `ref.null`, `ref.is_null`, `ref.func`, `ref.as_non_null` (WasmGC proposal)
- [ ] Performance tuning and Binaryen optimization integration
- [ ] Comprehensive testing and documentation completion

## Technical Specifications

### CLI Tool Design

```bash
ctransian [OPTIONS] INPUT.wasm -o OUTPUT.c

Options:
  --optimize-level[=0-3]     Binaryen optimization level (default: 2)
  --enable-simd             Enable SIMD translation with intrinsics
  --enable-threads          Enable C11 threading support
  --enable-gc               Enable WebAssembly GC types
  --bounds-check[=mode]     Bounds checking: strict, relaxed, none
  --runtime=embedded|standalone  Runtime implementation choice
  --target-c=c99|c11|gnu11   C language standard
  --debug                   Generate debug information
  --validate               Validate input before translation
  --wasi                   Enable WASI system interface
  --experimental           Enable experimental WASM proposals
```

### C Library API

```c
typedef struct ctransian_config {
    int optimization_level;
    bool enable_simd;
    bool enable_threads;
    bool enable_gc;
    bool bounds_checking;
    bool wasi_support;
    const char* target_c_standard;
} ctransian_config_t;

typedef struct ctransian_context ctransian_context_t;

ctransian_context_t* ctransian_create(const ctransian_config_t* config);

int ctransian_translate_binary(ctransian_context_t* ctx, 
                               const uint8_t* wasm_data, 
                               size_t wasm_size,
                               char** source, char** header);

void ctransian_destroy_context(ctransian_context_t* ctx);
```

## Performance Targets

### Translation Performance
- **Phase 0**: Constants translate correctly, basic test WASM files compile to valid C
- **Phase 1**: Simple functions with variables work, basic control flow translates correctly
- **Phase 2**: Mathematical functions work, memory operations work with bounds checking
- **Final**: Complete MVP WebAssembly-to-C translator, generated C code compiles and runs correctly

### Runtime Performance
- **Execution speed**: 2-5x faster than WebAssembly interpreters
- **Memory usage**: Within 20% of native C implementation
- **Code size**: 80-90% size of hand-written C equivalent

### Optimization Features
- **Dead code elimination**: Automatic removal of unused code
- **Constant folding**: Compile-time evaluation of constants
- **Loop optimization**: Recognition and optimization of common patterns
- **Inlined operations**: Direct C equivalents for simple WASM instructions

### Success Metrics by Phase
- **Phase 0 Success**: Constants translate correctly, basic test WASM files compile to valid C
- **Phase 1 Success**: Simple functions with variables work, basic control flow translates correctly, test suite passes with generated C code
- **Phase 2 Success**: Mathematical functions work, memory operations work with bounds checking, real-world WASM modules translate successfully
- **Final Success**: Complete MVP WebAssembly-to-C translator, generated C code compiles and runs correctly, performance comparable to existing tools

## Testing Strategy

### Unit Tests
- Individual instruction translation accuracy
- Type mapping correctness
- Error handling and edge cases

### Integration Tests
- Complete module translation
- Runtime behavior verification
- Cross-platform compatibility

### Performance Tests
- Benchmark suites vs existing tools
- Memory usage profiling
- Translation speed measurement

### Conformance Tests
- WebAssembly official test suite
- WABT compatibility tests
- Real-world application validation

## Build System Configuration

### CMake Features
```cmake
project(ctransian VERSION 1.0.0 LANGUAGES C CXX)

# Core requirements
find_package(PkgConfig REQUIRED)
pkg_check_modules(BINARYEN REQUIRED binaryen)

# Optional features
option(CTRANIAN_ENABLE_WASI "Enable WASI support" ON)
option(CTRANIAN_ENABLE_GC "Enable WasmGC support" OFF)
option(CTRANIAN_BUILD_TESTS "Build tests" ON)
option(CTRANIAN_BUILD_EXAMPLES "Build examples" ON)

# Build targets
add_executable(ctransian src/cli/main.cpp)
add_library(ctransian-static STATIC ${CORE_SOURCES})
add_library(ctransian-shared SHARED ${CORE_SOURCES})
```

## Documentation Plan

### User Documentation
1. **Getting Started**: Installation and basic usage
2. **API Reference**: Complete C API documentation
3. **Performance Guide**: Optimization tips and best practices
4. **Migration Guide**: Moving from existing tools
5. **Feature Matrix**: Detailed feature support information

### Developer Documentation
1. **Architecture Overview**: System design and components
2. **Contributing Guide**: Development setup and guidelines
3. **Code Style**: Formatting and conventions
4. **Testing Guide**: Running and writing tests

## Success Metrics

### Technical Goals
- 100% WebAssembly MVP compliance by Phase 2 (Data Operations)
- Support for 80% of active WASM proposals by Phase 5 (Advanced Features)
- 2-5x performance improvement over interpreters
- 80%+ reduction in code size vs naive translation
- Sub-second translation time for typical modules

### Adoption Goals
- Easy migration path from existing tools
- Integration with major build systems
- Community contribution guidelines
- Performance comparison published

## Competitive Advantages

1. **Binaryen Integration**: Superior optimization pipeline
2. **Modern C Support**: C11 with threading and atomics
3. **Comprehensive Features**: Support for most WASM proposals
4. **Performance Focus**: Optimized for speed and size
5. **Developer Experience**: Clean API and extensive documentation

## Risk Mitigation

### Technical Risks
- **Binaryen Dependency**: Version compatibility management
- **Feature Complexity**: Phased implementation approach
- **Performance Targets**: Continuous benchmarking and optimization

### Project Risks
- **Timeline Management**: Regular milestone reviews
- **Resource Allocation**: Core vs advanced feature prioritization
- **Community Adoption**: Early engagement and feedback collection

## Future Roadmap

### Short Term (Current - Phase 2)
- Complete Phase 0-2 implementation (Constants → Data Operations)
- Achieve functional WebAssembly-to-C translator
- Real-world WASM module translation

### Medium Term (Phase 3-4)
- Advanced operations and threading support
- Comprehensive test coverage and edge cases
- Performance benchmarking vs existing tools

### Long Term (Phase 5+)
- SIMD and WasmGC proposal support
- Advanced optimization passes
- Plugin architecture for extensions

---

This plan provides a comprehensive foundation for building `ctransian` as a leading WebAssembly to C translator, leveraging Binaryen's strengths while addressing the gaps in existing tools.