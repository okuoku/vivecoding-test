# Float Instructions

This document describes the translation strategy for WebAssembly floating-point arithmetic instructions.

## Instructions Overview

### Basic Arithmetic
- **f32.add/f64.add**: Direct C addition (+)
- **f32.sub/f64.sub**: Direct C subtraction (-)
- **f32.mul/f64.mul**: Direct C multiplication (*)
- **f32.div/f64.div**: Direct C division (/)

### Comparisons
- **f32.eq/f64.eq**: Equality comparison (==)
- **f32.ne/f64.ne**: Inequality comparison (!=)
- **f32.lt/f64.lt**: Less than (<)
- **f32.gt/f64.gt**: Greater than (>)
- **f32.le/f64.le**: Less than or equal (<=)
- **f32.ge/f64.ge**: Greater than or equal (>=)

### Special Operations
- **f32.abs/f64.abs**: Absolute value (fabsf/fabs)
- **f32.neg/f64.neg**: Negation (-value)
- **f32.sqrt/f64.sqrt**: Square root (sqrtf/sqrt)
- **f32.ceil/f64.ceil**: Ceiling (ceilf/ceil)
- **f32.floor/f64.floor**: Floor (floorf/floor)
- **f32.trunc/f64.trunc**: Truncate toward zero (truncf/trunc)
- **f32.nearest/f64.nearest**: Round to nearest integer (roundf/round)

### Minimum/Maximum
- **f32.min/f64.min**: Minimum (fminf/fmin) with NaN handling
- **f32.max/f64.max**: Maximum (fmaxf/fmax) with NaN handling

### Conversions
- **i32.trunc_f32_s**: Signed truncation
- **i32.trunc_f32_u**: Unsigned truncation
- **i64.trunc_f64_s**: Signed truncation
- **i64.trunc_f64_u**: Unsigned truncation
- **f32.convert_i32_s**: Integer to float conversion
- **f32.convert_i64_s**: Long to float conversion

## Type Mapping
- **f32**: `float`
- **f64**: `double`

## Implementation Notes
- Include `<math.h>` for math functions
- Handle special NaN and infinity cases per WebAssembly spec
- Use appropriate float/double variants of math functions