# SIMD Instructions

This document describes the translation strategy for WebAssembly SIMD (Single Instruction, Multiple Data) operations.

## Instructions Overview

SIMD operations work on 128-bit vectors (v128) containing multiple lanes of data.

### Vector Creation
- **v128.const**: Create constant vector
- **v128.load**: Load 128-bit from memory
- **v128.store**: Store 128-bit to memory

### Integer Vector Operations
- **i8x16.add**: Add 16 x 8-bit integers
- **i16x8.add**: Add 8 x 16-bit integers  
- **i32x4.add**: Add 4 x 32-bit integers
- **i64x2.add**: Add 2 x 64-bit integers

### Float Vector Operations
- **f32x4.add**: Add 4 x 32-bit floats
- **f64x2.add**: Add 2 x 64-bit doubles

### Vector Shape Operations
- **i8x16.shuffle**: Shuffle bytes in specific pattern
- **i8x16.swizzle**: Swizzle bytes using indices
- **i8x16.extract_lane_s/u**: Extract and sign/zero-extend lane
- **i8x16.replace_lane**: Replace lane value

## Type Mapping

WebAssembly SIMD vectors map to C structures:

```c
typedef struct {
    uint8_t bytes[16];
} wasm_v128_t;

// Access helpers for different interpretations
typedef union {
    wasm_v128_t v128;
    int8_t i8x16[16];
    uint8_t u8x16[16];
    int16_t i16x8[8];
    uint16_t u16x8[8];
    int32_t i32x4[4];
    uint32_t u32x4[4];
    int64_t i64x2[2];
    uint64_t u64x2[2];
    float f32x4[4];
    double f64x2[2];
} wasm_vector_t;
```

## Translation Strategy

### Vector Load/Store
```c
// v128.load addr
wasm_v128_t load_v128(wacgenrt_ctx* ctx, uint32_t addr) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 16);
    wasm_v128_t result;
    memcpy(result.bytes, WACGEN_MEM_PTR(ctx, addr), 16);
    return result;
}

// v128.store addr value
void store_v128(wacgenrt_ctx* ctx, uint32_t addr, wasm_v128_t value) {
    WACGEN_CHECK_BOUNDS(ctx, addr, 16);
    memcpy(WACGEN_MEM_PTR(ctx, addr), value.bytes, 16);
}
```

### Vector Arithmetic Operations
```c
// i32x4.add
wasm_v128_t i32x4_add(wacgen_v128_t a, wasm_v128_t b) {
    wasm_vector_t va, vr;
    va.v128 = a;
    
    for (int i = 0; i < 4; i++) {
        vr.i32x4[i] = va.i32x4[i] + ((wasm_vector_t*)&b)->i32x4[i];
    }
    
    return vr.v128;
}

// f32x4.mul
wasm_v128_t f32x4_mul(wacgen_v128_t a, wasm_v128_t b) {
    wasm_vector_t va, vr;
    va.v128 = a;
    
    for (int i = 0; i < 4; i++) {
        vr.f32x4[i] = va.f32x4[i] * ((wasm_vector_t*)&b)->f32x4[i];
    }
    
    return vr.v128;
}
```

### Lane Operations
```c
// i32x4.extract_lane
int32_t i32x4_extract_lane(wacgen_v128_t v, uint8_t lane) {
    wasm_vector_t* vec = (wasm_vector_t*)&v;
    return vec->i32x4[lane & 3];  // mask to 0-3
}

// i32x4.replace_lane
wasm_v128_t i32x4_replace_lane(wacgen_v128_t v, uint8_t lane, int32_t value) {
    wasm_vector_t* vec = (wasm_vector_t*)&v;
    vec->i32x4[lane & 3] = value;
    return v;
}
```

### Shuffle Operations
```c
// i8x16.shuffle imm0..imm15
wasm_v128_t i8x16_shuffle(wasm_v128_t a, wasm_v128_t b, 
                         uint8_t imm[16]) {
    wasm_vector_t va, vb, vr;
    va.v128 = a;
    vb.v128 = b;
    
    for (int i = 0; i < 16; i++) {
        uint8_t index = imm[i];
        if (index < 16) {
            vr.u8x16[i] = va.u8x16[index];
        } else {
            vr.u8x16[i] = vb.u8x16[index - 16];
        }
    }
    
    return vr.v128;
}
```

## Implementation Notes

### Performance Considerations
- Consider using compiler SIMD intrinsics for better performance
- Fall back to scalar operations when intrinsics unavailable
- Vector operations can be expensive on platforms without SIMD support

### Portability
- Ensure endianness handling is correct
- Test on both little and big-endian systems if needed
- Consider alignment requirements for vector operations

### Compiler Support
- GCC/Clang: `__builtin_vector` operations
- MSVC: `__m128i` types
- Runtime detection of SIMD capabilities

## Example Translation

### WebAssembly
```wat
(func $vector_add (param i32) (result v128)
  local.get 0
  v128.load
  local.get 0
  i32.const 16
  i32.add
  v128.load
  i32x4.add
)
```

### Generated C
```c
wasm_v128_t __basename_vector_add(wacgenrt_ctx* ctx, int32_t param0) {
    wasm_v128_t vec1, vec2, result;
    
    // v128.load addr
    vec1 = load_v128(ctx, param0);
    
    // i32.const 16, i32.add, v128.load
    vec2 = load_v128(ctx, param0 + 16);
    
    // i32x4.add
    result = i32x4_add(vec1, vec2);
    
    return result;
}
```

Note: SIMD implementation is complex and may require platform-specific optimizations for production use.