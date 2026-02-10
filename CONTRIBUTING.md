# Contributing to ctransian

We welcome contributions to ctransian! This document provides guidelines for contributing to the project.

## Development Setup

### Prerequisites

- **CMake 3.15+** for building
- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **Binaryen library** installed and available via pkg-config
- **C11 compatible compiler** for testing generated code

### Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/your-org/ctransian.git
   cd ctransian
   ```

2. **Install dependencies**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install cmake build-essential pkg-config binaryen-dev
   
   # macOS
   brew install cmake pkg-config binaryen
   
   # Build Binaryen from source if needed
   git clone https://github.com/WebAssembly/binaryen.git
   cd binaryen
   cmake . && make
   sudo make install
   ```

3. **Build the project**
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Debug
   make
   ```

4. **Run tests**
   ```bash
   make test
   ```

## Code Style

### C++ Code

- Use **C++17** features
- **4 spaces** for indentation (no tabs)
- **snake_case** for variables and functions
- **PascalCase** for classes and types
- **UPPER_CASE** for constants and macros
- **80-100 character** line limit where reasonable

### C Code (Runtime)

- Use **C11** features
- **snake_case** for all identifiers
- Include **proper headers** and **forward declarations**
- **Bounds checking** for all memory access
- **Error handling** with proper return codes

### Examples

```cpp
// Good C++ style
class WebAssemblyTranslator {
public:
    explicit WebAssemblyTranslator(const TranslationConfig& config);
    
    ctransian_error_t translateModule(
        const uint8_t* wasm_data,
        size_t wasm_size,
        TranslationResult* result
    );
    
private:
    const TranslationConfig* config_;
    std::unique_ptr<TypeMapper> type_mapper_;
};

// Good C style (runtime)
ctransian_error_t ctransian_translate_binary(
    ctransian_context_t* ctx,
    const uint8_t* wasm_data,
    size_t wasm_size,
    ctransian_result_t* result
) {
    if (!ctx || !wasm_data || wasm_size == 0 || !result) {
        return CTRANIAN_ERROR_INVALID_INPUT;
    }
    
    // Implementation...
    return CTRANIAN_SUCCESS;
}
```

## Testing

### Writing Tests

1. **Unit tests** for individual components
2. **Integration tests** for complete workflows
3. **Performance tests** for benchmarks
4. **Regression tests** for known issues

### Test Structure

```cpp
// tests/test_feature_name.cpp
#include <cassert>
#include "ctransian/public.h"

void test_specific_functionality() {
    // Arrange
    ctransian_config_t* config = ctransian_config_create();
    // Setup test data...
    
    // Act
    ctransian_context_t* ctx = ctransian_create(config);
    ctransian_result_t result;
    ctransian_result_init(&result);
    
    // Call function under test...
    
    // Assert
    assert(result.error_code == CTRANIAN_SUCCESS);
    
    // Cleanup
    ctransian_result_cleanup(&result);
    ctransian_destroy_context(ctx);
    ctransian_config_destroy(config);
}

int main() {
    test_specific_functionality();
    // Add more tests...
    return 0;
}
```

### Running Tests

```bash
# Run all tests
make test

# Run specific test
ctest -R TestName

# Run with verbose output
ctest --verbose

# Run performance tests
make test-performance
```

## Submitting Changes

### Workflow

1. **Fork** the repository
2. **Create** a feature branch: `git checkout -b feature/my-feature`
3. **Make** your changes
4. **Test** thoroughly
5. **Commit** with descriptive messages
6. **Push** to your fork
7. **Create** a pull request

### Commit Messages

Use conventional commit format:

```
type(scope): description

[optional body]

[optional footer]
```

Examples:
- `feat(cli): add optimization level flag`
- `fix(runtime): correct memory bounds checking`
- `docs(readme): update installation instructions`
- `test(translation): add WASM validation tests`

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation
- `style`: Code style (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Test additions/changes
- `chore`: Build process, maintenance

## Code Review Process

### Review Checklist

- [ ] **Code follows style guidelines**
- [ ] **Tests are included and passing**
- [ ] **Documentation is updated**
- [ ] **No breaking changes** (or clearly documented)
- [ ] **Performance impact considered**
- [ ] **Memory safety verified**
- [ ] **Error handling is robust**

### Review Guidelines

1. **Be constructive** and helpful
2. **Focus on code quality**, not personal preferences
3. **Explain reasoning** for suggestions
4. **Welcome contributions** from all experience levels

## Issue Reporting

### Bug Reports

1. **Search existing issues** first
2. **Use bug report template**
3. **Include**: 
   - **System information** (OS, compiler, versions)
   - **Minimal reproduction case**
   - **Expected vs actual behavior**
   - **Any error messages**

### Feature Requests

1. **Check roadmap** first
2. **Describe use case** clearly
3. **Consider implementation complexity**
4. **Discuss trade-offs**

## Development Guidelines

### Memory Management

- **RAII** in C++ code
- **Explicit cleanup** in C code
- **Bounds checking** for all array access
- **Smart pointers** where appropriate

### Error Handling

- **Return error codes**, not exceptions in C API
- **Use exceptions** in C++ internal code
- **Provide clear error messages**
- **Handle edge cases**

### Performance

- **Profile** before optimizing
- **Avoid premature optimization**
- **Consider memory locality**
- **Minimize allocations**

### WebAssembly Specific

- **Follow WASM specification** exactly
- **Handle all edge cases** (overflow, division by zero, etc.)
- **Consider validation requirements**
- **Test against official test suite**

## Documentation

### Code Comments

- **Complex algorithms**: Explain logic
- **Non-obvious code**: Explain reasoning
- **API boundaries**: Document contracts
- **Workarounds**: Explain why needed

### User Documentation

- **Keep README up to date**
- **Document public API**
- **Provide examples**
- **Include performance notes**

## Release Process

### Versioning

Follow [Semantic Versioning](https://semver.org/):
- **MAJOR**: Breaking changes
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

### Release Checklist

- [ ] **All tests passing**
- [ ] **Documentation updated**
- [ ] **CHANGELOG updated**
- [ ] **Version bumped**
- [ ] **Tag created**
- [ ] **Release published**

## Community

### Getting Help

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For general questions
- **Email**: For security issues or private concerns

### Contributing Areas

- **Core translation engine**
- **CLI tooling**
- **Runtime library**
- **Testing framework**
- **Documentation**
- **Build system**
- **Performance optimization**

## License

By contributing, you agree that your contributions will be licensed under the **Apache License 2.0**, the same license as the project.

---

Thank you for contributing to ctransian! 🚀