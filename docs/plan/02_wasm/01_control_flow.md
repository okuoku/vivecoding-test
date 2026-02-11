# Control Flow Instructions

This document describes the translation strategy for WebAssembly control flow instructions.

## Instructions Overview

### nop
- **Binaryen C API**: `BinaryenNop()`
- **WebAssembly spec**: Does nothing
- **Translation**: Empty statement or comment
```c
// nop -> (no operation)
```

### block
- **Binaryen C API**: `BinaryenBlock()`
- **WebAssembly spec**: Sequenced statement block with optional label
- **Translation**: C block with labeled goto targets
```c
// block $label
//   instruction1
//   instruction2
// end
{
    // instruction1
    // instruction2
}
```

### loop
- **Binaryen C API**: `BinaryenLoop()`
- **WebAssembly spec**: Loop construct with entry label for continue
- **Translation**: C while(1) loop with break/continue
```c
// loop $label
//   instruction1
//   br $label  // continue
//   instruction2
// end
while (1) {
    // instruction1
    continue;  // goto label
    // instruction2
    break;     // exit loop
}
```

### if
- **Binaryen C API**: `BinaryenIf()`
- **WebAssembly spec**: Conditional execution with optional else
- **Translation**: C if/else statement
```c
// if (condition)
//   then_instruction
// else
//   else_instruction
// end
if (condition) {
    // then_instruction
} else {
    // else_instruction
}
```

### br
- **Binaryen C API**: `BinaryenBreak()`
- **BinaryenGetBreakValue()`: Get branch value
- **WebAssembly spec**: Unconditional branch to label
- **Translation**: C goto statement
```c
// br $label
goto label;
```

### br_if
- **Binaryen C API**: `BinaryenBreak()`
- **WebAssembly spec**: Conditional branch to label
- **Translation**: C conditional goto
```c
// br_if $label (condition)
if (condition) {
    goto label;
}
```

### br_table
- **Binaryen C API**: `BinaryenSwitch()`
- **WebAssembly spec**: Indexed branch with default case
- **Translation**: C switch statement
```c
// br_table indices... $default (index)
switch (index) {
    case 0: goto label0;
    case 1: goto label1;
    // ...
    default: goto default_label;
}
```

### return
- **Binaryen C API**: `BinaryenReturn()`
- **WebAssembly spec**: Return from function with optional value
- **Translation**: C return statement
```c
// return (value)
return value;
// return
return;
```

### unreachable
- **Binaryen C API**: `BinaryenUnreachable()`
- **WebAssembly spec**: Trap execution
- **Translation**: Call to abort macro
```c
// unreachable
WACGEN_ABORT();
```

## Implementation Strategy

### Label Management
- Map WebAssembly labels to C labels using basename prefix
- Generate unique labels to avoid conflicts
- Use stack to track nested control structures

### Control Flow Stack
Maintain stack of active control contexts:
```c
typedef struct {
    char label[256];     // Label name
    int is_loop;         // Whether this is a loop
    char break_label[256];  // Label for break
    char continue_label[256]; // Label for continue
} control_context_t;
```

### Translation Process
1. Enter control structure and push context
2. Translate nested instructions
3. Generate label definitions at appropriate points
4. Pop context when exiting structure

### Edge Cases
- Empty blocks/bodies
- Multiple return values (WebAssembly MVP supports single)
- Branch values
- Deep nesting (handle recursion limits)

## Example Translation

### WebAssembly
```wat
(func $test (param i32) (result i32)
  local.get 0
  if
    return 42
  else
    loop $l
      local.get 0
      i32.const 1
      i32.sub
      local.tee 0
      br_if $l
    end
  end
  local.get 0
)
```

### Generated C
```c
int32_t __basename_test(wacgenrt_ctx* ctx, int32_t param0) {
    int32_t local0 = param0;
    
    if (local0) {
        return 42;
    } else {
        while (1) {
            local0 = local0 - 1;
            if (local0) {
                continue;  // br_if $l
            }
            break;        // implicit break from loop
        }
    }
    
    return local0;
}
```