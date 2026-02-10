#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ctransian/ctransian_runtime.h"

/*
 * Example of using ctransian runtime for a simple math function
 * This would be the kind of code that ctransian would generate
 */

/* Example: Add two integers */
int32_t add_i32(struct ctransian_instance* instance, int32_t a, int32_t b) {
    return a + b;
}

/* Example: Multiply two integers */
int32_t mul_i32(struct ctransian_instance* instance, int32_t a, int32_t b) {
    /* Check for overflow */
    if (a != 0 && b > INT32_MAX / a) {
        ctransian_trap(CTRANIAN_TRAP_INTEGER_OVERFLOW);
        return 0;
    }
    return a * b;
}

/* Example: Memory access */
int32_t load_memory_i32(struct ctransian_instance* instance, uint32_t address) {
    if (!instance || !instance->memory) {
        ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS);
        return 0;
    }
    
    /* Bounds checking */
    if (!ctransian_memory_check_bounds(instance->memory, address, 4)) {
        ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS);
        return 0;
    }
    
    /* Load value */
    return CTRANIAN_MEMORY_LOAD32(instance->memory, address);
}

/* Example: Memory store */
void store_memory_i32(struct ctransian_instance* instance, uint32_t address, int32_t value) {
    if (!instance || !instance->memory) {
        ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS);
        return;
    }
    
    /* Bounds checking */
    if (!ctransian_memory_check_bounds(instance->memory, address, 4)) {
        ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS);
        return;
    }
    
    /* Store value */
    CTRANIAN_MEMORY_STORE32(instance->memory, address, value);
}

/* Example: Division with trap handling */
int32_t div_i32(struct ctransian_instance* instance, int32_t a, int32_t b) {
    /* Check for division by zero */
    if (b == 0) {
        ctransian_trap(CTRANIAN_TRAP_DIVISION_BY_ZERO);
        return 0;
    }
    
    /* Check for INT32_MIN / -1 overflow */
    if (a == INT32_MIN && b == -1) {
        ctransian_trap(CTRANIAN_TRAP_INTEGER_OVERFLOW);
        return 0;
    }
    
    return a / b;
}

/* Example: Function that uses global variables */
int32_t increment_global(struct ctransian_instance* instance, uint32_t global_index) {
    if (!instance || !instance->globals || global_index >= instance->num_globals) {
        ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_TABLE_ACCESS);
        return 0;
    }
    
    struct ctransian_global* global = &instance->globals[global_index];
    if (!global->is_mutable) {
        /* Trying to modify immutable global - this would be a runtime error in WebAssembly */
        ctransian_runtime_set_error("Attempt to modify immutable global");
        return 0;
    }
    
    /* Increment and return the new value */
    global->value.i32_val++;
    return global->value.i32_val;
}

/* Example main function that could be generated */
int main() {
    printf("ctransian simple math example\\n");
    printf("================================\\n");
    
    /* Create an instance */
    struct ctransian_instance* instance = ctransian_instance_create();
    if (!instance) {
        printf("Failed to create instance: %s\\n", ctransian_runtime_get_error());
        return 1;
    }
    
    /* Test basic operations */
    printf("add_i32(5, 3) = %d\\n", add_i32(instance, 5, 3));
    printf("mul_i32(4, 7) = %d\\n", mul_i32(instance, 4, 7));
    printf("div_i32(10, 2) = %d\\n", div_i32(instance, 10, 2));
    
    /* Test memory operations */
    store_memory_i32(instance, 0, 42);
    store_memory_i32(instance, 4, 84);
    
    printf("Memory at address 0: %d\\n", load_memory_i32(instance, 0));
    printf("Memory at address 4: %d\\n", load_memory_i32(instance, 4));
    
    /* Test trap handling */
    printf("\\nTesting error conditions:\\n");
    
    /* Division by zero */
    printf("Attempting division by zero...\\n");
    div_i32(instance, 10, 0);
    if (ctransian_get_trap() != CTRANIAN_TRAP_NONE) {
        printf("Trap caught: %s\\n", ctransian_trap_string(ctransian_get_trap()));
        ctransian_clear_trap();
    }
    
    /* Out of bounds memory access */
    printf("Attempting out of bounds memory access...\\n");
    load_memory_i32(instance, 0xFFFFFFFF);
    if (ctransian_get_trap() != CTRANIAN_TRAP_NONE) {
        printf("Trap caught: %s\\n", ctransian_trap_string(ctransian_get_trap()));
        ctransian_clear_trap();
    }
    
    /* Print memory usage */
    printf("\\nMemory information:\\n");
    printf("Current memory size: %zu pages\\n", ctransian_memory_size(instance->memory));
    
    /* Grow memory */
    if (ctransian_memory_grow(instance->memory, 1) != (size_t)-1) {
        printf("Memory grown to: %zu pages\\n", ctransian_memory_size(instance->memory));
    }
    
    /* Cleanup */
    ctransian_instance_destroy(instance);
    
    printf("\\nExample completed successfully!\\n");
    return 0;
}