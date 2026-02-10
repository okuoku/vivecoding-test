#include "ctransian/ctransian_runtime.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef CTRANIAN_ENABLE_THREADS
#include <threads.h>
#endif

/* Default memory size */
#define CTRANIAN_DEFAULT_MEMORY_SIZE (64 * 1024 * 1024)  // 64MB
#define CTRANIAN_MAX_MEMORY_SIZE (4ULL * 1024 * 1024 * 1024)  // 4GB

/* Memory management */
struct ctransian_memory {
    uint8_t* data;
    size_t size;
    size_t max_size;
    bool is_shared;
    
#ifdef CTRANIAN_ENABLE_THREADS
    mtx_t mutex;
#endif
};

/* Global variable */
struct ctransian_global {
    union {
        int32_t i32_val;
        int64_t i64_val;
        float f32_val;
        double f64_val;
        void* ptr_val;
    } value;
    bool is_mutable;
    uint32_t type;
};

/* Function table */
struct ctransian_table {
    void** entries;
    size_t size;
    size_t max_size;
    uint32_t element_type;
};

/* Module instance */
struct ctransian_instance {
    struct ctransian_memory* memory;
    struct ctransian_global* globals;
    size_t num_globals;
    struct ctransian_table* tables;
    size_t num_tables;
    
    /* Import function pointers */
    void** imports;
    size_t num_imports;
    
    /* Export function pointers */
    void** exports;
    char** export_names;
    size_t num_exports;
    
    /* Runtime state */
    bool is_initialized;
    
#ifdef CTRANIAN_ENABLE_THREADS
    thrd_t* threads;
    size_t num_threads;
#endif
};

/* Global error state */
static char g_error_message[1024] = {0};

/* Error handling */
void ctransian_runtime_set_error(const char* message) {
    if (message) {
        strncpy(g_error_message, message, sizeof(g_error_message) - 1);
        g_error_message[sizeof(g_error_message) - 1] = '\0';
    }
}

const char* ctransian_runtime_get_error(void) {
    return g_error_message;
}

void ctransian_runtime_clear_error(void) {
    g_error_message[0] = '\0';
}

/* Memory management functions */
struct ctransian_memory* ctransian_memory_create(size_t initial_size, size_t max_size, bool is_shared) {
    struct ctransian_memory* memory = malloc(sizeof(struct ctransian_memory));
    if (!memory) {
        ctransian_runtime_set_error("Failed to allocate memory structure");
        return NULL;
    }
    
    /* Align to page size (64KB) */
    initial_size = (initial_size + 65535) & ~65535;
    max_size = (max_size + 65535) & ~65535;
    
    memory->data = malloc(initial_size);
    if (!memory->data) {
        free(memory);
        ctransian_runtime_set_error("Failed to allocate memory");
        return NULL;
    }
    
    memory->size = initial_size;
    memory->max_size = max_size;
    memory->is_shared = is_shared;
    
    /* Initialize memory to zero */
    memset(memory->data, 0, initial_size);
    
#ifdef CTRANIAN_ENABLE_THREADS
    if (mtx_init(&memory->mutex, mtx_plain) != thrd_success) {
        free(memory->data);
        free(memory);
        ctransian_runtime_set_error("Failed to initialize memory mutex");
        return NULL;
    }
#endif
    
    return memory;
}

void ctransian_memory_destroy(struct ctransian_memory* memory) {
    if (memory) {
        if (memory->data) {
            free(memory->data);
        }
#ifdef CTRANIAN_ENABLE_THREADS
        mtx_destroy(&memory->mutex);
#endif
        free(memory);
    }
}

size_t ctransian_memory_grow(struct ctransian_memory* memory, size_t delta_pages) {
    if (!memory) {
        ctransian_runtime_set_error("Invalid memory instance");
        return (size_t)-1;
    }
    
    const size_t page_size = 65536;  // 64KB
    size_t delta_bytes = delta_pages * page_size;
    size_t new_size = memory->size + delta_bytes;
    
    if (new_size > memory->max_size) {
        ctransian_runtime_set_error("Memory growth would exceed maximum size");
        return (size_t)-1;
    }
    
#ifdef CTRANIAN_ENABLE_THREADS
    if (memory->is_shared) {
        mtx_lock(&memory->mutex);
    }
#endif
    
    void* new_data = realloc(memory->data, new_size);
    if (!new_data) {
#ifdef CTRANIAN_ENABLE_THREADS
        if (memory->is_shared) {
            mtx_unlock(&memory->mutex);
        }
#endif
        ctransian_runtime_set_error("Failed to grow memory");
        return (size_t)-1;
    }
    
    /* Initialize new memory to zero */
    memset((uint8_t*)new_data + memory->size, 0, delta_bytes);
    
    memory->data = new_data;
    memory->size = new_size;
    
#ifdef CTRANIAN_ENABLE_THREADS
    if (memory->is_shared) {
        mtx_unlock(&memory->mutex);
    }
#endif
    
    return memory->size / page_size;
}

size_t ctransian_memory_size(const struct ctransian_memory* memory) {
    return memory ? memory->size / 65536 : 0;
}

/* Safe memory access functions */
bool ctransian_memory_check_bounds(const struct ctransian_memory* memory, uint32_t address, size_t size) {
    if (!memory || !memory->data) {
        return false;
    }
    
    return (address + size) <= memory->size;
}

/* Global variable management */
struct ctransian_global* ctransian_globals_create(size_t count) {
    if (count == 0) {
        return NULL;
    }
    
    struct ctransian_global* globals = calloc(count, sizeof(struct ctransian_global));
    if (!globals) {
        ctransian_runtime_set_error("Failed to allocate globals");
        return NULL;
    }
    
    return globals;
}

void ctransian_globals_destroy(struct ctransian_global* globals) {
    if (globals) {
        free(globals);
    }
}

/* Table management */
struct ctransian_table* ctransian_table_create(size_t initial_size, size_t max_size, uint32_t element_type) {
    struct ctransian_table* table = malloc(sizeof(struct ctransian_table));
    if (!table) {
        ctransian_runtime_set_error("Failed to allocate table structure");
        return NULL;
    }
    
    table->entries = calloc(initial_size, sizeof(void*));
    if (!table->entries) {
        free(table);
        ctransian_runtime_set_error("Failed to allocate table entries");
        return NULL;
    }
    
    table->size = initial_size;
    table->max_size = max_size;
    table->element_type = element_type;
    
    return table;
}

void ctransian_table_destroy(struct ctransian_table* table) {
    if (table) {
        if (table->entries) {
            free(table->entries);
        }
        free(table);
    }
}

/* Module instance management */
struct ctransian_instance* ctransian_instance_create(void) {
    struct ctransian_instance* instance = calloc(1, sizeof(struct ctransian_instance));
    if (!instance) {
        ctransian_runtime_set_error("Failed to allocate instance");
        return NULL;
    }
    
    /* Create default memory */
    instance->memory = ctransian_memory_create(
        CTRANIAN_DEFAULT_MEMORY_SIZE,
        CTRANIAN_MAX_MEMORY_SIZE,
        false
    );
    
    if (!instance->memory) {
        free(instance);
        return NULL;
    }
    
    instance->is_initialized = true;
    
    return instance;
}

void ctransian_instance_destroy(struct ctransian_instance* instance) {
    if (instance) {
        ctransian_memory_destroy(instance->memory);
        ctransian_globals_destroy(instance->globals);
        
        for (size_t i = 0; i < instance->num_tables; i++) {
            ctransian_table_destroy(&instance->tables[i]);
        }
        
        if (instance->tables) {
            free(instance->tables);
        }
        
        if (instance->imports) {
            free(instance->imports);
        }
        
        if (instance->exports) {
            free(instance->exports);
        }
        
        if (instance->export_names) {
            for (size_t i = 0; i < instance->num_exports; i++) {
                if (instance->export_names[i]) {
                    free(instance->export_names[i]);
                }
            }
            free(instance->export_names);
        }
        
#ifdef CTRANIAN_ENABLE_THREADS
        if (instance->threads) {
            free(instance->threads);
        }
#endif
        
        free(instance);
    }
}

/* Trap handling */
typedef enum {
    CTRANIAN_TRAP_NONE = 0,
    CTRANIAN_TRAP_UNREACHABLE,
    CTRANIAN_TRAP_DIVISION_BY_ZERO,
    CTRANIAN_TRAP_INTEGER_OVERFLOW,
    CTRANIAN_TRAP_INVALID_CONVERSION_TO_INTEGER,
    CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS,
    CTRANIAN_TRAP_OUT_OF_BOUNDS_TABLE_ACCESS,
    CTRANIAN_TRAP_INDIRECT_CALL_TYPE_MISMATCH,
    CTRANIAN_TRAP_UNINITIALIZED_ELEMENT,
    CTRANIAN_TRAP_UNALIGNED_ACCESS,
    CTRANIAN_TRAP_STACK_OVERFLOW
} ctransian_trap_t;

static thread_local ctransian_trap_t g_current_trap = CTRANIAN_TRAP_NONE;

void ctransian_trap(ctransian_trap_t trap) {
    g_current_trap = trap;
}

ctransian_trap_t ctransian_get_trap(void) {
    return g_current_trap;
}

void ctransian_clear_trap(void) {
    g_current_trap = CTRANIAN_TRAP_NONE;
}

const char* ctransian_trap_string(ctransian_trap_t trap) {
    switch (trap) {
        case CTRANIAN_TRAP_NONE:
            return "No trap";
        case CTRANIAN_TRAP_UNREACHABLE:
            return "Unreachable instruction executed";
        case CTRANIAN_TRAP_DIVISION_BY_ZERO:
            return "Integer division by zero";
        case CTRANIAN_TRAP_INTEGER_OVERFLOW:
            return "Integer overflow";
        case CTRANIAN_TRAP_INVALID_CONVERSION_TO_INTEGER:
            return "Invalid conversion to integer";
        case CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS:
            return "Out of bounds memory access";
        case CTRANIAN_TRAP_OUT_OF_BOUNDS_TABLE_ACCESS:
            return "Out of bounds table access";
        case CTRANIAN_TRAP_INDIRECT_CALL_TYPE_MISMATCH:
            return "Indirect call type mismatch";
        case CTRANIAN_TRAP_UNINITIALIZED_ELEMENT:
            return "Uninitialized element";
        case CTRANIAN_TRAP_UNALIGNED_ACCESS:
            return "Unaligned access";
        case CTRANIAN_TRAP_STACK_OVERFLOW:
            return "Stack overflow";
        default:
            return "Unknown trap";
    }
}

/* Utility functions */
uint32_t ctransian_leb128_read(const uint8_t** data, const uint8_t* end) {
    uint32_t result = 0;
    uint32_t shift = 0;
    const uint8_t* ptr = *data;
    
    while (ptr < end) {
        uint8_t byte = *ptr++;
        result |= ((byte & 0x7F) << shift);
        if ((byte & 0x80) == 0) {
            break;
        }
        shift += 7;
        if (shift >= 32) {
            ctransian_runtime_set_error("Invalid LEB128 encoding");
            *data = ptr;
            return 0;
        }
    }
    
    *data = ptr;
    return result;
}

void ctransian_leb128_write(uint8_t** data, uint32_t value) {
    uint8_t* ptr = *data;
    
    do {
        uint8_t byte = value & 0x7F;
        value >>= 7;
        if (value != 0) {
            byte |= 0x80;
        }
        *ptr++ = byte;
    } while (value != 0);
    
    *data = ptr;
}