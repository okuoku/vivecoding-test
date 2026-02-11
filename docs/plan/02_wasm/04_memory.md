# Memory Instructions

This document describes the translation strategy for WebAssembly memory operations.

## Instructions Overview

### Load Operations
- **i32.load**: Load 32-bit signed integer from memory
- **i64.load**: Load 64-bit signed integer from memory  
- **f32.load**: Load 32-bit float from memory
- **f64.load**: Load 64-bit double from memory

#### Extended Load Variants
- **i32.load8_s**: Load 8-bit signed, sign-extend to 32-bit
- **i32.load8_u**: Load 8-bit unsigned, zero-extend to 32-bit
- **i32.load16_s**: Load 16-bit signed, sign-extend to 32-bit
- **i32.load16_u**: Load 16-bit unsigned, zero-extend to 32-bit
- **i64.load8_s**: Load 8-bit signed, sign-extend to 64-bit
- **i64.load8_u**: Load 8-bit unsigned, zero-extend to 64-bit
- **i64.load16_s**: Load 16-bit signed, sign-extend to 64-bit
- **i64.load16_u**: Load 16-bit unsigned, zero-extend to 64-bit
- **i64.load32_s**: Load 32-bit signed, sign-extend to 64-bit
- **i64.load32_u**: Load 32-bit unsigned, zero-extend to 64-bit

### Store Operations
- **i32.store**: Store 32-bit integer to memory
- **i64.store**: Store 64-bit integer to memory
- **f32.store**: Store 32-bit float to memory
- **f64.store**: Store 64-bit double to memory

#### Extended Store Variants
- **i32.store8**: Store 8-bit from 32-bit value (truncate)
- **i32.store16**: Store 16-bit from 32-bit value (truncate)
- **i64.store8**: Store 8-bit from 64-bit value (truncate)
- **i64.store16**: Store 16-bit from 64-bit value (truncate)
- **i64.store32**: Store 32-bit from 64-bit value (truncate)

### Memory Management
- **memory.size**: Get current memory size in pages
- **memory.grow**: Grow memory by specified number of pages

## Translation Strategy

### Memory Access Macros
```c
#define WACGEN_MEM_PTR(ctx, addr) ((ctx)->memory + (addr))
#define WACGEN_CHECK_BOUNDS(ctx, addr, size) \
    do { if ((addr) + (size) > (ctx)->memory_size) WACGEN_ABORT(); } while(0)
```

### Load Operations Translation
```c
// i32.load addr
int32_t load_i32(wacgenrt_ctx* ctx, uint32_t addr) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 4);
    return *(int32_t*)WACGEN_MEM_PTR(ctx, addr);
}

// i32.load8_s addr
int32_t load_i32_8s(wacgenrt_ctx* ctx, uint32_t addr) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 1);
    return (int32_t)*(int8_t*)WACGEN_MEM_PTR(ctx, addr);
}

// i32.load8_u addr  
int32_t load_i32_8u(wacgenrt_ctx* ctx, uint32_t addr) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 1);
    return (int32_t)*(uint8_t*)WACGEN_MEM_PTR(ctx, addr);
}
```

### Store Operations Translation
```c
// i32.store addr value
void store_i32(wacgenrt_ctx* ctx, uint32_t addr, int32_t value) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 4);
    *(int32_t*)WACGEN_MEM_PTR(ctx, addr) = value;
}

// i32.store8 addr value
void store_i32_8(wacgenrt_ctx* ctx, uint32_t addr, int32_t value) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 1);
    *(uint8_t*)WACGEN_MEM_PTR(ctx, addr) = (uint8_t)value;
}
```

### Memory Size Operations
```c
// memory.size
uint32_t memory_size(wacgenrt_ctx* ctx) {
    return ctx->memory_pages;
}

// memory.grow pages
int32_t memory_grow(wacgenrt_ctx* ctx, uint32_t pages) {
    uint32_t old_pages = ctx->memory_pages;
    if (wacgenrt_grow_memory(ctx, pages) == 0) {
        return -1;  // growth failed
    }
    return old_pages;
}
```

## Alignment Handling

WebAssembly supports alignment hints for optimization:
- Natural alignment is default
- Misaligned loads may be slower but must work
- Use memcpy for potentially unaligned access if needed

```c
// Safe unaligned load
int32_t load_i32_unaligned(wacgenrt_ctx* ctx, uint32_t addr) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 4);
    int32_t value;
    memcpy(&value, WACGEN_MEM_PTR(ctx, addr), 4);
    return value;
}
```

## Implementation Notes

### Memory Layout
- WebAssembly memory is a contiguous byte array
- Default page size is 64KB (65536 bytes)
- Memory is zero-initialized
- Little-endian byte order

### Bounds Checking
- Check address + size <= memory_size for each access
- Trap (abort) on out-of-bounds access
- Consider performance impact of bounds checking

### Atomic Operations
- WebAssembly also supports atomic memory operations
- These require special handling for thread safety
- Not covered in basic memory instruction set

## Example Translation

### WebAssembly
```wat
(func $test (param i32) (result i32)
  local.get 0
  i32.load offset=4 align=4
  i32.const 42
  i32.store offset=8 align=4
  local.get 0
  i32.load offset=4 align=4
)
```

### Generated C
```c
int32_t __basename_test(wacgenrt_ctx* ctx, int32_t param0) {
    int32_t addr = param0;
    int32_t value;
    
    // i32.load offset=4
    WACGEN_CHECK_BOUNDS(ctx, addr + 4, 4);
    value = *(int32_t*)WACGEN_MEM_PTR(ctx, addr + 4);
    
    // i32.const 42, i32.store offset=8
    WACGEN_CHECK_BOUNDS(ctx, addr + 8, 4);
    *(int32_t*)WACGEN_MEM_PTR(ctx, addr + 8) = 42;
    
    // i32.load offset=4 (return)
    WACGEN_CHECK_BOUNDS(ctx, addr + 4, 4);
    value = *(int32_t*)WACGEN_MEM_PTR(ctx, addr + 4);
    
    return value;
}
```