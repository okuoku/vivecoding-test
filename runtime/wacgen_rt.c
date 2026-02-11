#include "wacgen_rt.h"
#include <stdlib.h>
#include <string.h>

#define WASM_PAGE_SIZE 65536  // 64KB

void wacgenrt_ctx_init(wacgenrt_ctx* ctx, size_t initial_memory) {
    if (!ctx) return;
    
    ctx->memory_size = initial_memory;
    ctx->memory = calloc(1, initial_memory);
    ctx->tables = NULL;
    ctx->table_size = 0;
    ctx->globals = NULL;
}

void wacgenrt_ctx_destroy(wacgenrt_ctx* ctx) {
    if (!ctx) return;
    
    free(ctx->memory);
    free(ctx->tables);
    free(ctx->globals);
    
    ctx->memory = NULL;
    ctx->tables = NULL;
    ctx->globals = NULL;
    ctx->memory_size = 0;
    ctx->table_size = 0;
}

size_t wacgenrt_memory_size(wacgenrt_ctx* ctx) {
    if (!ctx) return 0;
    return ctx->memory_size / WASM_PAGE_SIZE;
}

size_t wacgenrt_memory_grow(wacgenrt_ctx* ctx, size_t delta_pages) {
    if (!ctx || delta_pages == 0) return (size_t)-1;
    
    size_t old_size = ctx->memory_size;
    size_t new_size = old_size + delta_pages * WASM_PAGE_SIZE;
    
    void* new_memory = realloc(ctx->memory, new_size);
    if (!new_memory) return (size_t)-1;
    
    // Zero-initialize the new pages
    memset((uint8_t*)new_memory + old_size, 0, new_size - old_size);
    
    ctx->memory = new_memory;
    ctx->memory_size = new_size;
    
    return old_size / WASM_PAGE_SIZE;
}