#ifndef WACGEN_RT_H
#define WACGEN_RT_H

#include <stdint.h>
#include <stddef.h>

// Opaque context structure for WebAssembly execution
typedef struct wacgenrt_ctx wacgenrt_ctx;

// Context management functions
wacgenrt_ctx* wacgenrt_ctx_init(void);
void wacgenrt_ctx_destroy(wacgenrt_ctx* ctx);

// Memory operations
int wacgenrt_grow_memory(wacgenrt_ctx* ctx, size_t delta_pages);

// Table operations for indirect calls
void* wacgenrt_table_get(wacgenrt_ctx* ctx, uint32_t index);
int wacgenrt_table_set(wacgenrt_ctx* ctx, uint32_t index, void* value);

// Memory and safety macros
#define WACGEN_CHECK_BOUNDS(ptr, size) do {} while(0)
#define WACGEN_ABORT() do { abort(); } while(0)

#endif // WACGEN_RT_H