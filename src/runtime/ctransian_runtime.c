#include "../../include/ctransian/ctransian_runtime.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Error handling */
static char runtime_error[256] = {0};

void ctransian_runtime_set_error(const char* message) {
    if (message) {
        strncpy(runtime_error, message, sizeof(runtime_error) - 1);
        runtime_error[sizeof(runtime_error) - 1] = '\0';
    }
}

const char* ctransian_runtime_get_error(void) {
    return runtime_error;
}

void ctransian_runtime_clear_error(void) {
    runtime_error[0] = '\0';
}

/* Memory management */
struct ctransian_memory* ctransian_memory_create(size_t initial_size, size_t max_size, bool is_shared) {
    struct ctransian_memory* memory = malloc(sizeof(struct ctransian_memory));
    if (!memory) {
        ctransian_runtime_set_error("Failed to allocate memory structure");
        return NULL;
    }
    
    memory->data = malloc(initial_size);
    if (!memory->data) {
        ctransian_runtime_set_error("Failed to allocate memory data");
        free(memory);
        return NULL;
    }
    
    memory->size = initial_size;
    memory->page_size = 65536; // 64KB pages
    memory->max_pages = max_size / memory->page_size;
    
    return memory;
}

void ctransian_memory_destroy(struct ctransian_memory* memory) {
    if (memory) {
        free(memory->data);
        free(memory);
    }
}

size_t ctransian_memory_grow(struct ctransian_memory* memory, size_t delta_pages) {
    if (!memory) {
        ctransian_runtime_set_error("Invalid memory pointer");
        return SIZE_MAX;
    }
    
    size_t new_size = memory->size + (delta_pages * memory->page_size);
    size_t max_size = memory->max_pages * memory->page_size;
    
    if (new_size > max_size) {
        ctransian_runtime_set_error("Memory size would exceed maximum");
        return SIZE_MAX;
    }
    
    void* new_data = realloc(memory->data, new_size);
    if (!new_data) {
        ctransian_runtime_set_error("Failed to grow memory");
        return SIZE_MAX;
    }
    
    memory->data = new_data;
    memory->size = new_size;
    
    return (memory->size / memory->page_size) - delta_pages;
}

/* Memory access functions */
size_t ctransian_memory_size(const struct ctransian_memory* memory) {
    return memory ? memory->size : 0;
}

void* ctransian_memory_data(struct ctransian_memory* memory) {
    return memory ? memory->data : NULL;
}

int ctransian_memory_write(struct ctransian_memory* memory, size_t offset, const void* data, size_t size) {
    if (!memory || !memory->data) {
        ctransian_runtime_set_error("Invalid memory or data pointer");
        return 1;
    }
    if (offset + size > memory->size) {
        ctransian_runtime_set_error("Write offset out of bounds");
        return 2;
    }
    
    memcpy((uint8_t*)memory->data + offset, data, size);
    return 0;
}

int ctransian_memory_read(const struct ctransian_memory* memory, size_t offset, void* data, size_t size) {
    if (!memory || !memory->data) {
        ctransian_runtime_set_error("Invalid memory or data pointer");
        return 1;
    }
    if (offset + size > memory->size) {
        ctransian_runtime_set_error("Read offset out of bounds");
        return 2;
    }
    
    memcpy(data, (const uint8_t*)memory->data + offset, size);
    return 0;
}

/* Instance management */
struct ctransian_instance* ctransian_instance_create(void) {
    struct ctransian_instance* instance = malloc(sizeof(struct ctransian_instance));
    if (!instance) {
        ctransian_runtime_set_error("Failed to allocate instance");
        return NULL;
    }
    
    memset(instance, 0, sizeof(struct ctransian_instance));
    instance->memory = ctransian_memory_create(64 * 1024 * 1024, 4ULL * 1024 * 1024 * 1024, false);
    if (!instance->memory) {
        free(instance);
        return NULL;
    }
    
    return instance;
}

void ctransian_instance_destroy(struct ctransian_instance* instance) {
    if (instance) {
        ctransian_memory_destroy(instance->memory);
        free(instance);
    }
}

/* Trap handling */
const char* ctransian_trap_string(ctransian_trap_t trap) {
    switch (trap) {
        case CTRANIAN_TRAP_NONE: return "no trap";
        case CTRANIAN_TRAP_UNREACHABLE: return "unreachable";
        case CTRANIAN_TRAP_DIVISION_BY_ZERO: return "division by zero";
        case CTRANIAN_TRAP_INTEGER_OVERFLOW: return "integer overflow";
        case CTRANIAN_TRAP_INVALID_CONVERSION_TO_INTEGER: return "invalid conversion to integer";
        case CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS: return "out of bounds memory access";
        case CTRANIAN_TRAP_OUT_OF_BOUNDS_TABLE_ACCESS: return "out of bounds table access";
        case CTRANIAN_TRAP_INDIRECT_CALL_TYPE_MISMATCH: return "indirect call type mismatch";
        case CTRANIAN_TRAP_UNALIGNED_ACCESS: return "unaligned access";
        case CTRANIAN_TRAP_UNINITIALIZED_ELEMENT: return "uninitialized element";
        default: return "unknown trap";
    }
}