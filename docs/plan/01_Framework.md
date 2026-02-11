# wacgen Conversion Framework

This document describes the core framework for converting WebAssembly modules to C99 source code using Binaryen's C API.

## Key Binaryen C API Functions to Use

### Module Operations
- `BinaryenModuleRead()` - Load WebAssembly binary from memory buffer
- `BinaryenModuleDispose()` - Clean up module and free resources
- `BinaryenModuleValidate()` - Validate module structure

### Function Information
- `BinaryenModuleGetNumFunctions()` - Get total number of functions in module
- `BinaryenModuleGetFunction()` - Get function by index
- `BinaryenFunctionGetName()` - Get function name string
- `BinaryenFunctionGetNumParams()` - Get parameter count
- `BinaryenFunctionGetNumResults()` - Get return value count
- `BinaryenFunctionGetNumLocals()` - Get local variable count
- `BinaryenFunctionGetBody()` - Get function body as expression tree

### Expression Tree Traversal
- `BinaryenExpressionGetId()` - Identify instruction types (BinaryenNopId, BinaryenAddId, etc.)
- `BinaryenExpressionGetChildren()` - Get child expressions from parent
- `BinaryenBlockGetName()` - Get block label name
- `BinaryenIfGetCondition()` - Get if condition expression

### Type Information
- `BinaryenExpressionGetType()` - Get expression type
- `BinaryenFunctionGetType()` - Get function signature type

## C Function Templates

### WebAssembly Function Template
```c
void __{basename}_{function_name}(wacgenrt_ctx* ctx, {params}) {
    // Local variables
    {local_declarations}
    
    // Function body translation
    {translated_instructions}
}
```

### Initialization Function Template
```c
typedef struct {
    const char* name;
    void* ptr;
} __wacgen_{basename}_symbol_t;

__wacgen_{basename}_symbol_t* __wacgen_{basename}_init(void) {
    static __wacgen_{basename}_symbol_t symbols[] = {
        {{"function1"}, __{basename}_function1},
        {{"function2"}, __{basename}_function2},
        // ... more symbols
        {NULL, NULL}
    };
    return symbols;
}
```

### Context Structure Definitions
```c
// Runtime context structure (defined in wacgen_rt.h)
struct wacgenrt_ctx {
    uint8_t* memory;       // Linear memory
    size_t memory_size;    // Current memory size in bytes
    size_t memory_pages;   // Current memory pages
    void** table;          // Function table for indirect calls
    size_t table_size;     // Table size
    void** globals;        // Global variables
    size_t num_globals;    // Number of globals
};
```

### Memory and Safety Macros
```c
// Memory bounds checking (no-op implementation)
#define WACGEN_CHECK_BOUNDS(ptr, size) do {} while(0)

// Abort wrapper
#define WACGEN_ABORT() do { abort(); } while(0)

// Memory access macros
#define WACGEN_LOAD_I32(ctx, addr) (*(int32_t*)((ctx)->memory + (addr)))
#define WACGEN_STORE_I32(ctx, addr, val) (*(int32_t*)((ctx)->memory + (addr)) = (val))
```

## Code Generation Strategy

### Translation Pipeline
1. **Parse WebAssembly module** using Binaryen
2. **Extract function information** (name, parameters, locals, body)
3. **Generate C function signatures** based on WebAssembly types
4. **Translate expression tree** to C statements recursively
5. **Generate initialization function** with symbol table
6. **Write C source and header files**

### Type Mapping
- `i32` → `int32_t`
- `i64` → `int64_t` 
- `f32` → `float`
- `f64` → `double`

### Control Flow Translation
- `block` → C block with labeled goto targets
- `loop` → C while(1) loop with break/continue
- `if` → C if/else statement
- `br` → goto label
- `br_if` → conditional goto

## Error Handling

### Error Codes
```c
typedef enum {
    WACGEN_SUCCESS = 0,
    WACGEN_ERROR_INVALID_INPUT = 1,
    WACGEN_ERROR_PARSE_FAILED = 2,
    WACGEN_ERROR_CODEGEN_FAILED = 3,
    WACGEN_ERROR_MEMORY = 4
} wacgen_result_t;
```

### Error Reporting
- Use `fprintf(stderr, ...)` for error messages
- Return appropriate error codes from functions
- Cleanup resources on error paths

## Memory Management

### Binaryen Resource Management
- Always call `BinaryenModuleDispose()` when done
- Don't free expressions manually (handled by module)
- Copy string values immediately if needed long-term

### Code Generator Memory Management
- Use malloc/free for dynamic structures
- Implement cleanup functions for context objects
- Check return values of memory allocations