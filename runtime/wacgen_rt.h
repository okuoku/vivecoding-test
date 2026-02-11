#ifndef WACGEN_RT_H
#define WACGEN_RT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Runtime context structure
typedef struct {
    void* memory;           // Linear memory pointer
    size_t memory_size;     // Current memory size in bytes
    void** tables;          // Indirect function call tables
    size_t table_size;      // Table size
    void* globals;          // Global variables storage
} wacgenrt_ctx;

// Memory and safety macros
#define WACGEN_CHECK_BOUNDS(ptr, size) ((void)0)  // No-op implementation
#define WACGEN_ABORT() do { abort(); } while(0)

// Memory access helpers
#define WACGEN_LOAD_I32(ctx, offset) (*(int32_t*)((uint8_t*)(ctx)->memory + (offset)))
#define WACGEN_STORE_I32(ctx, offset, value) (*(int32_t*)((uint8_t*)(ctx)->memory + (offset)) = (value))

// Runtime functions
void wacgenrt_ctx_init(wacgenrt_ctx* ctx, size_t initial_memory);
void wacgenrt_ctx_destroy(wacgenrt_ctx* ctx);
size_t wacgenrt_memory_size(wacgenrt_ctx* ctx);
size_t wacgenrt_memory_grow(wacgenrt_ctx* ctx, size_t delta_pages);

#ifdef __cplusplus
}
#endif

#endif // WACGEN_RT_H