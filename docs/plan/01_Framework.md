# Wacgen Framework Documentation

This document describes the core framework for `wacgen`, including Binaryen C API usage and C function templates for WebAssembly to C translation.

## Key Binaryen C API Functions

### Module Loading and Cleanup
- `BinaryenModuleRead()` - Load WebAssembly binary from buffer
- `BinaryenModuleDispose()` - Clean up module and free memory

### Module Information
- `BinaryenModuleGetNumFunctions()` - Get total function count in module
- `BinaryenModuleGetFunction()` - Access individual function by index
- `BinaryenModuleGetNumGlobals()` - Get global variable count
- `BinaryenModuleGetNumTables()` - Get table count
- `BinaryenModuleGetNumMemorySegments()` - Get memory segment count

### Function Analysis
- `BinaryenFunctionGetName()` - Get function name/symbol
- `BinaryenFunctionGetNumParams()` - Get parameter count
- `BinaryenFunctionGetParamType()` - Get parameter type by index
- `BinaryenFunctionGetNumResults()` - Get return value count
- `BinaryenFunctionGetResultType()` - Get return type
- `BinaryenFunctionGetNumLocals()` - Get local variable count
- `BinaryenFunctionGetLocalType()` - Get local variable type by index
- `BinaryenFunctionGetBody()` - Get function body expression tree

### Expression Tree Traversal
- `BinaryenExpressionGetId()` - Identify instruction type (BinaryenNop, BinaryenAdd, etc.)
- `BinaryenExpressionGetType()` - Get expression result type
- `BinaryenExpressionGetChildren()` - Get child expressions for tree traversal
- `BinaryenBlockGetName()` - Get block label name
- `BinaryenIfGetCondition()` - Get if condition expression
- `BinaryenIfGetIfTrue()` - Get if true branch
- `BinaryenIfGetIfFalse()` - Get if false branch

### Type System
- `BinaryenTypeInt32()` - i32 type
- `BinaryenTypeInt64()` - i64 type
- `BinaryenTypeFloat32()` - f32 type
- `BinaryenTypeFloat64()` - f64 type
- `BinaryenTypeNone()` - void/no result type
- `BinaryenTypeAreEqual()` - Check type equality

## C Function Templates

### Generated Function Template

```c
// Template for WebAssembly function with parameters
return_type __{basename}_{func_name}(wacgenrt_ctx* ctx, param1_type param1, param2_type param2) {
    // Local variable declarations
    local_var1_type local1;
    local_var2_type local2;
    
    // WebAssembly instruction translation
    // Each instruction becomes corresponding C code
    
    // Return value (if any)
    return result;
}
```

### Void Function Template

```c
// Template for WebAssembly function with no return value
void __{basename}_{func_name}(wacgenrt_ctx* ctx, param1_type param1, param2_type param2) {
    // Function body
    // No return statement needed
}
```

### Parameter-only Function Template

```c
// Template for WebAssembly function with no parameters
return_type __{basename}_{func_name}(wacgenrt_ctx* ctx) {
    // Function body with no external parameters
    return result;
}
```

### Simple Void No-params Template

```c
// Template for simplest WebAssembly function
void __{basename}_{func_name}(wacgenrt_ctx* ctx) {
    // Function body
}
```

## Context Structure Definition

```c
// Runtime context structure
typedef struct {
    void* memory;           // Linear memory pointer
    size_t memory_size;     // Current memory size in bytes
    void** table;          // Indirect function call table
    size_t table_size;     // Table size
    void* globals;         // Global variables storage
} wacgenrt_ctx;
```

## Memory and Safety Macros

```c
// Memory bounds checking (no-op implementation)
#define WACGEN_CHECK_BOUNDS(ptr, size) ((void)0)

// Abort wrapper macro
#define WACGEN_ABORT() do { abort(); } while(0)

// Memory access helpers
#define WACGEN_LOAD_I32(offset) (*(int32_t*)((uint8_t*)ctx->memory + (offset)))
#define WACGEN_STORE_I32(offset, value) (*(int32_t*)((uint8_t*)ctx->memory + (offset)) = (value))
```

## Module Initialization Function Template

```c
// Module initialization function
typedef struct {
    const char* name;
    void* func_ptr;
} wacgen_func_info_t;

typedef struct {
    const char* name;
    void* global_ptr;
    BinaryenType type;
} wacgen_global_info_t;

// Template for initialization function
void __wacgen_{basename}_init(wacgenrt_ctx* ctx, 
                              wacgen_func_info_t** functions, 
                              size_t* num_functions,
                              wacgen_global_info_t** globals, 
                              size_t* num_globals) {
    static wacgen_func_info_t exported_functions[] = {
        { "exported_func1", __{basename}_exported_func1 },
        { "exported_func2", __{basename}_exported_func2 },
        // Add more exported functions here
    };
    
    static wacgen_global_info_t exported_globals[] = {
        { "global1", &global1_var, BinaryenTypeInt32() },
        { "global2", &global2_var, BinaryenTypeInt64() },
        // Add more exported globals here
    };
    
    *functions = exported_functions;
    *num_functions = sizeof(exported_functions) / sizeof(exported_functions[0]);
    *globals = exported_globals;
    *num_globals = sizeof(exported_globals) / sizeof(exported_globals[0]);
}
```

## Type Mapping WebAssembly to C

| WebAssembly Type | C Type | Binaryen Type |
|------------------|--------|---------------|
| i32              | int32_t | BinaryenTypeInt32() |
| i64              | int64_t | BinaryenTypeInt64() |
| f32              | float   | BinaryenTypeFloat32() |
| f64              | double  | BinaryenTypeFloat64() |
| void             | void    | BinaryenTypeNone() |

## Translation Strategy

### Expression Tree Processing
1. Traverse expression tree depth-first using `BinaryenExpressionGetChildren()`
2. For each node, identify instruction type using `BinaryenExpressionGetId()`
3. Generate corresponding C code based on instruction type
4. Handle type conversions between WebAssembly and C types

### Function Generation Steps
1. Extract function signature using `BinaryenFunctionGet*` APIs
2. Generate C function declaration with appropriate types
3. Process function body expression tree
4. Generate C statements for each WebAssembly instruction
5. Add return statement if function has return value

### Symbol Naming Conventions
- Exported WebAssembly functions: `__{basename}_{func_name}`
- Local WebAssembly functions: `__{basename}_{func_name}_local`
- Global variables: `__{basename}_{global_name}`
- Labels/branches: `__{basename}_{label_name}`