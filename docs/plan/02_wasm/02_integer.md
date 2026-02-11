# Integer Instructions

This document describes the translation strategy for WebAssembly integer arithmetic and bitwise instructions.

## Instructions Overview

### Addition
- **i32.add**: `BinaryenBinary(BinaryenAddInt32, left, right)`
- **i64.add**: `BinaryenBinary(BinaryenAddInt64, left, right)`
- **Translation**: Direct C addition
```c
// i32.add
result = left + right;

// i64.add  
result = left + right;
```

### Subtraction
- **i32.sub**: `BinaryenBinary(BinaryenSubInt32, left, right)`
- **i64.sub**: `BinaryenBinary(BinaryenSubInt64, left, right)`
- **Translation**: Direct C subtraction
```c
// i32.sub
result = left - right;

// i64.sub
result = left - right;
```

### Multiplication
- **i32.mul**: `BinaryenBinary(BinaryenMulInt32, left, right)`
- **i64.mul**: `BinaryenBinary(BinaryenMulInt64, left, right)`
- **Translation**: Direct C multiplication
```c
// i32.mul
result = left * right;

// i64.mul
result = left * right;
```

### Division (Signed)
- **i32.div_s**: `BinaryenBinary(BinaryenDivSInt32, left, right)`
- **i64.div_s**: `BinaryenBinary(BinaryenDivSInt64, left, right)`
- **Translation**: C division with overflow handling
```c
// i32.div_s
if (right == 0) WACGEN_ABORT();
if (left == INT32_MIN && right == -1) WACGEN_ABORT();
result = left / right;

// i64.div_s
if (right == 0) WACGEN_ABORT();
if (left == INT64_MIN && right == -1) WACGEN_ABORT();
result = left / right;
```

### Division (Unsigned)
- **i32.div_u**: `BinaryenBinary(BinaryenDivUInt32, left, right)`
- **i64.div_u**: `BinaryenBinary(BinaryenDivUInt64, left, right)`
- **Translation**: Cast to unsigned, divide, cast back
```c
// i32.div_u
if (right == 0) WACGEN_ABORT();
result = (int32_t)((uint32_t)left / (uint32_t)right);

// i64.div_u
if (right == 0) WACGEN_ABORT();
result = (int64_t)((uint64_t)left / (uint64_t)right);
```

### Remainder (Signed)
- **i32.rem_s**: `BinaryenBinary(BinaryenRemSInt32, left, right)`
- **i64.rem_s**: `BinaryenBinary(BinaryenRemSInt64, left, right)`
- **Translation**: C modulo with overflow handling
```c
// i32.rem_s
if (right == 0) WACGEN_ABORT();
if (left == INT32_MIN && right == -1) result = 0;
else result = left % right;
```

### Remainder (Unsigned)
- **i32.rem_u**: `BinaryenBinary(BinaryenRemUInt32, left, right)`
- **i64.rem_u**: `BinaryenBinary(BinaryenRemUInt64, left, right)`
- **Translation**: Cast to unsigned, modulo, cast back
```c
// i32.rem_u
if (right == 0) WACGEN_ABORT();
result = (int32_t)((uint32_t)left % (uint32_t)right);
```

### Bitwise AND
- **i32.and**: `BinaryenBinary(BinaryenAndInt32, left, right)`
- **i64.and**: `BinaryenBinary(BinaryenAndInt64, left, right)`
- **Translation**: Direct C bitwise AND
```c
result = left & right;
```

### Bitwise OR
- **i32.or**: `BinaryenBinary(BinaryenOrInt32, left, right)`
- **i64.or**: `BinaryenBinary(BinaryenOrInt64, left, right)`
- **Translation**: Direct C bitwise OR
```c
result = left | right;
```

### Bitwise XOR
- **i32.xor**: `BinaryenBinary(BinaryenXorInt32, left, right)`
- **i64.xor**: `BinaryenBinary(BinaryenXorInt64, left, right)`
- **Translation**: Direct C bitwise XOR
```c
result = left ^ right;
```

### Shift Left
- **i32.shl**: `BinaryenBinary(BinaryenShlInt32, left, right)`
- **i64.shl**: `BinaryenBinary(BinaryenShlInt64, left, right)`
- **Translation**: C shift with mask
```c
// i32.shl (mask with 31)
result = left << (right & 31);

// i64.shl (mask with 63)
result = left << (right & 63);
```

### Shift Right (Signed)
- **i32.shr_s**: `BinaryenBinary(BinaryenShrSInt32, left, right)`
- **i64.shr_s**: `BinaryenBinary(BinaryenShrSInt64, left, right)`
- **Translation**: C arithmetic shift
```c
// i32.shr_s
result = left >> (right & 31);

// i64.shr_s
result = left >> (right & 63);
```

### Shift Right (Unsigned)
- **i32.shr_u**: `BinaryenBinary(BinaryenShrUInt32, left, right)`
- **i64.shr_u**: `BinaryenBinary(BinaryenShrUInt64, left, right)`
- **Translation**: Cast to unsigned, shift, cast back
```c
// i32.shr_u
result = (int32_t)((uint32_t)left >> (right & 31));

// i64.shr_u
result = (int64_t)((uint64_t)left >> (right & 63));
```

### Rotations
- **i32.rotl**: `BinaryenBinary(BinaryenRotLInt32, left, right)`
- **i32.rotr**: `BinaryenBinary(BinaryenRotRInt32, left, right)`
- **i64.rotl**: `BinaryenBinary(BinaryenRotLInt64, left, right)`
- **i64.rotr**: `BinaryenBinary(BinaryenRotRInt64, left, right)`
- **Translation**: Use rotation functions or macros
```c
// i32.rotl
uint32_t mask = right & 31;
result = (int32_t)(((uint32_t)left << mask) | ((uint32_t)left >> (32 - mask)));

// i32.rotr  
uint32_t mask = right & 31;
result = (int32_t)(((uint32_t)left >> mask) | ((uint32_t)left << (32 - mask)));
```

## Constants
- **i32.const**: `BinaryenConst(Int32, value)`
- **i64.const**: `BinaryenConst(Int64, value)`
- **Translation**: Direct C literal
```c
// i32.const 42
42

// i64.const 10000000000LL
10000000000LL
```

## Comparisons

### Equality
- **i32.eq**: `BinaryenBinary(BinaryenEqInt32, left, right)`
- **i64.eq**: `BinaryenBinary(BinaryenEqInt64, left, right)`
- **Translation**: C equality comparison
```c
result = left == right;
```

### Inequality
- **i32.ne**: `BinaryenBinary(BinaryenNeInt32, left, right)`
- **i64.ne**: `BinaryenBinary(BinaryenNeInt64, left, right)`
- **Translation**: C inequality comparison
```c
result = left != right;
```

### Less Than (Signed)
- **i32.lt_s**: `BinaryenBinary(BinaryenLtSInt32, left, right)`
- **i64.lt_s**: `BinaryenBinary(BinaryenLtSInt64, left, right)`
- **Translation**: C less-than comparison
```c
result = left < right;
```

### Less Than (Unsigned)
- **i32.lt_u**: `BinaryenBinary(BinaryenLtUInt32, left, right)`
- **i64.lt_u**: `BinaryenBinary(BinaryenLtUInt64, left, right)`
- **Translation**: Cast to unsigned, compare
```c
result = (uint32_t)left < (uint32_t)right;
```

### Other Comparisons
- **le**: Less than or equal (<=)
- **gt**: Greater than (>)
- **ge**: Greater than or equal (>=)

## Implementation Strategy

### Type Mapping
```c
typedef int32_t wasm_i32_t;
typedef int64_t wasm_i64_t;
typedef uint32_t wasm_u32_t;
typedef uint64_t wasm_u64_t;
```

### Overflow Handling
WebAssembly defines specific overflow behavior:
- Division by zero → trap
- INT_MIN / -1 → trap for signed division
- Rotations use modulo bit width

### Example Translation

### WebAssembly
```wat
(func $test (param i32 i32) (result i32)
  local.get 0
  local.get 1
  i32.add
  i32.const 42
  i32.mul
  local.get 0
  i32.div_s
)
```

### Generated C
```c
int32_t __basename_test(wacgenrt_ctx* ctx, int32_t param0, int32_t param1) {
    int32_t temp0;
    int32_t temp1;
    int32_t result;
    
    // local.get 0
    temp0 = param0;
    
    // local.get 1  
    temp1 = param1;
    
    // i32.add
    temp0 = temp0 + temp1;
    
    // i32.const 42, i32.mul
    temp0 = temp0 * 42;
    
    // local.get 0, i32.div_s
    if (param0 == 0) WACGEN_ABORT();
    if (temp0 == INT32_MIN && param0 == -1) WACGEN_ABORT();
    result = temp0 / param0;
    
    return result;
}
```