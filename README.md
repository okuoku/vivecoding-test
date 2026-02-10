# ctransian

Advanced WebAssembly to C translation powered by Binaryen.

## Overview

`ctransian` is a high-performance WebAssembly to C translator that leverages Binaryen's advanced optimization pipeline to generate efficient, modern C code. Unlike existing tools, `ctransian` provides comprehensive support for WebAssembly proposals while generating optimized C11 code with threading and SIMD capabilities.

## Key Features

- **Binaryen-Powered Optimization**: Leverages Binaryen's advanced optimization passes (SSA, loop optimization, etc.)
- **Modern C11 Support**: Full support for C11 threading, atomics, and modern language features
- **Comprehensive WASM Support**: Support for most WebAssembly proposals including SIMD, threading, and GC
- **Dual Interface**: Both CLI tool and C library API for integration
- **Performance Optimized**: 2-5x faster performance than interpretation-based approaches
- **Memory Safe**: Configurable bounds checking and safe memory access patterns

## Quick Start

### Installation

```bash
# Clone with submodules (includes Binaryen)
git clone --recursive https://github.com/your-org/ctransian.git
cd ctransian
mkdir build && cd build
cmake .. && make
```

### Basic Usage

```bash
# Translate WebAssembly to C
ctransian input.wasm -o output.c

# With optimizations and features
ctransian --optimize-level=3 --enable-simd --enable-threads input.wasm -o output.c
```

### C Library API

```c
#include <ctransian/public.h>

ctransian_config_t config = {
    .optimization_level = 2,
    .enable_simd = true,
    .enable_threads = true,
    .target_c_standard = "c11"
};

ctransian_context_t* ctx = ctransian_create(&config);
char* source, *header;
ctransian_translate_binary(ctx, wasm_data, wasm_size, &source, &header);
```

## Comparison with Existing Tools

| Feature | wasm2c | w2c2 | ctransian |
|---------|---------|------|------------|
| Basic WASM MVP | ✅ | ✅ | ✅ |
| SIMD | ❌ | Partial | ✅ + Intrinsics |
| Threading | Limited | ❌ | ✅ C11 Threads |
| GC Types | ❌ | ❌ | ✅ Planned |
| Optimization | Basic | Good | ✅ Binaryen-powered |
| C11 Support | ❌ | ❌ | ✅ |
| WASI Support | ❌ | ✅ | ✅ Planned |

## Architecture

`ctransian` follows a multi-stage translation pipeline:

1. **Parsing**: WebAssembly binary/WAT → Binaryen IR
2. **Optimization**: Binaryen optimization passes
3. **Analysis**: Type system and control flow analysis
4. **Generation**: C11 code generation with runtime support
5. **Output**: Optimized C source and headers

## Documentation

- [Getting Started Guide](docs/getting-started.md)
- [API Reference](docs/api-reference.md)
- [Performance Tuning](docs/performance.md)
- [Feature Compatibility](docs/feature-matrix.md)
- [Migration Guide](docs/migration.md)

## Examples

See the [examples/](examples/) directory for comprehensive examples:

- [Basic Math Functions](examples/math/)
- [SIMD Image Processing](examples/simd/)
- [Multi-threaded Computing](examples/threads/)
- [WASI Command-line Tools](examples/wasi/)

## Building

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler
- C11 compatible compiler for generated code
- Git (for submodule initialization)

### Build Commands

```bash
# Clone with submodules (includes Binaryen)
git clone --recursive https://github.com/your-org/ctransian.git
cd ctransian

# Standard build
mkdir build && cd build
cmake .. && make

# With debug information
cmake -DCMAKE_BUILD_TYPE=Debug .. && make

# With specific features
cmake -DCTRANIAN_ENABLE_WASI=ON -DCTRANIAN_ENABLE_GC=OFF .. && make

# Update submodules (if needed)
git submodule update --init --recursive
```

## Testing

```bash
# Run all tests
make test

# Run specific test suites
make test-unit
make test-integration
make test-performance
```

## Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Setup

```bash
# Clone with submodules
git clone --recursive https://github.com/your-org/ctransian.git

# Setup development environment
cd ctransian
python scripts/setup-dev.py
```

## License

Licensed under the Apache License 2.0. See [LICENSE](LICENSE) for details.

## Acknowledgments

- [Binaryen](https://github.com/WebAssembly/binaryen) for the optimization infrastructure
- [WebAssembly](https://webassembly.org/) community for the specification
- [WABT](https://github.com/WebAssembly/wabt) for inspiration and reference implementation

## Performance Benchmarks

See [PERFORMANCE.md](PERFORMANCE.md) for detailed benchmarks comparing `ctransian` with other WebAssembly runtimes and translators.

## Support

- 📖 [Documentation](docs/)
- 🐛 [Issue Tracker](https://github.com/your-org/ctransian/issues)
- 💬 [Discussions](https://github.com/your-org/ctransian/discussions)
- 📧 [Email](mailto:ctransian@example.org)

---

**ctransian**: Where WebAssembly meets C. Powered by Binaryen, written for performance.