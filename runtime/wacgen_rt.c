#include "wacgen_rt.h"
#include <stdlib.h>
#include <string.h>

// Context structure definition
struct wacgenrt_ctx {
    uint8_t* memory;
    size_t memory_size;
    size_t memory_pages;
    void** table;
    size_t table_size;
    void** globals;
    size_t num_globals;
};

wacgenrt_ctx* wacgenrt_ctx_init(void) {
    wacgenrt_ctx* ctx = calloc(1, sizeof(wacgenrt_ctx));
    if (!ctx) return NULL;
    
    // Initialize with default memory size (64KB = 1 page)
    ctx->memory_pages = 1;
    ctx->memory_size = 64 * 1024;
    ctx->memory = calloc(ctx->memory_size, 1);
    
    if (!ctx->memory) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

void wacgenrt_ctx_destroy(wacgenrt_ctx* ctx) {
    if (!ctx) return;
    
    if (ctx->memory) free(ctx->memory);
    if (ctx->table) free(ctx->table);
    if (ctx->globals) free(ctx->globals);
    
    free(ctx);
}

int wacgenrt_grow_memory(wacgenrt_ctx* ctx, size_t delta_pages) {
    if (!ctx) return -1;
    
    size_t new_pages = ctx->memory_pages + delta_pages;
    size_t new_size = new_pages * 64 * 1024;
    
    uint8_t* new_memory = realloc(ctx->memory, new_size);
    if (!new_memory) return -1;
    
    // Zero out the newly allocated memory
    if (new_size > ctx->memory_size) {
        memset(new_memory + ctx->memory_size, 0, new_size - ctx->memory_size);
    }
    
    ctx->memory = new_memory;
    ctx->memory_size = new_size;
    ctx->memory_pages = new_pages;
    
    return 0;
}

void* wacgenrt_table_get(wacgenrt_ctx* ctx, uint32_t index) {
    if (!ctx || !ctx->table || index >= ctx->table_size) {
        return NULL;
    }
    
    return ctx->table[index];
}

int wacgenrt_table_set(wacgenrt_ctx* ctx, uint32_t index, void* value) {
    if (!ctx) return -1;
    
    // Grow table if needed
    if (index >= ctx->table_size) {
        size_t new_size = index + 1;
        void** new_table = realloc(ctx->table, new_size * sizeof(void*));
        if (!new_table) return -1;
        
        // Initialize new entries to NULL
        for (size_t i = ctx->table_size; i < new_size; i++) {
            new_table[i] = NULL;
        }
        
        ctx->table = new_table;
        ctx->table_size = new_size;
    }
    
    ctx->table[index] = value;
    return 0;
}