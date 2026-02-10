#ifndef CTRANIAN_RUNTIME_H
#define CTRANIAN_RUNTIME_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ctransian_global;
struct ctransian_table;
struct ctransian_instance;

/* Memory structure */
struct ctransian_memory {
    uint8_t* data;
    size_t size;
    size_t page_size;
    size_t max_pages;
};

/* Instance structure */
struct ctransian_instance {
    struct ctransian_memory* memory;
    struct ctransian_global* globals;
    struct ctransian_table* table;
    void* extra;
};

/* Thread type definition */
typedef pthread_t ctransian_thread_t;

/* Error handling */
void ctransian_runtime_set_error(const char* message);
const char* ctransian_runtime_get_error(void);
void ctransian_runtime_clear_error(void);

/* Memory management */
struct ctransian_memory* ctransian_memory_create(size_t initial_size, size_t max_size, bool is_shared);
void ctransian_memory_destroy(struct ctransian_memory* memory);
size_t ctransian_memory_grow(struct ctransian_memory* memory, size_t delta_pages);
size_t ctransian_memory_size(const struct ctransian_memory* memory);
bool ctransian_memory_check_bounds(const struct ctransian_memory* memory, uint32_t address, size_t size);

/* Global variable management */
struct ctransian_global* ctransian_globals_create(size_t count);
void ctransian_globals_destroy(struct ctransian_global* globals);

/* Table management */
struct ctransian_table* ctransian_table_create(size_t initial_size, size_t max_size, uint32_t element_type);
void ctransian_table_destroy(struct ctransian_table* table);

/* Module instance management */
struct ctransian_instance* ctransian_instance_create(void);
void ctransian_instance_destroy(struct ctransian_instance* instance);

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

void ctransian_trap(ctransian_trap_t trap);
ctransian_trap_t ctransian_get_trap(void);
void ctransian_clear_trap(void);
const char* ctransian_trap_string(ctransian_trap_t trap);

/* Utility functions */
uint32_t ctransian_leb128_read(const uint8_t** data, const uint8_t* end);
void ctransian_leb128_write(uint8_t** data, uint32_t value);

/* Memory access macros for generated code */
#define CTRANIAN_MEMORY_LOAD8(mem, addr) \
    (ctransian_memory_check_bounds(mem, addr, 1) ? ((int8_t*)(mem)->data)[addr] : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), 0))

#define CTRANIAN_MEMORY_LOAD16(mem, addr) \
    (ctransian_memory_check_bounds(mem, addr, 2) ? ((int16_t*)((uint8_t*)(mem)->data + addr))[0] : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), 0))

#define CTRANIAN_MEMORY_LOAD32(mem, addr) \
    (ctransian_memory_check_bounds(mem, addr, 4) ? ((int32_t*)((uint8_t*)(mem)->data + addr))[0] : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), 0))

#define CTRANIAN_MEMORY_LOAD64(mem, addr) \
    (ctransian_memory_check_bounds(mem, addr, 8) ? ((int64_t*)((uint8_t*)(mem)->data + addr))[0] : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), 0))

#define CTRANIAN_MEMORY_STORE8(mem, addr, val) \
    do { if (ctransian_memory_check_bounds(mem, addr, 1)) ((int8_t*)(mem)->data)[addr] = (val); else ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS); } while(0)

#define CTRANIAN_MEMORY_STORE16(mem, addr, val) \
    do { if (ctransian_memory_check_bounds(mem, addr, 2)) ((int16_t*)((uint8_t*)(mem)->data + addr))[0] = (val); else ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS); } while(0)

#define CTRANIAN_MEMORY_STORE32(mem, addr, val) \
    do { if (ctransian_memory_check_bounds(mem, addr, 4)) ((int32_t*)((uint8_t*)(mem)->data + addr))[0] = (val); else ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS); } while(0)

#define CTRANIAN_MEMORY_STORE64(mem, addr, val) \
    do { if (ctransian_memory_check_bounds(mem, addr, 8)) ((int64_t*)((uint8_t*)(mem)->data + addr))[0] = (val); else ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS); } while(0)

/* Atomic memory access macros (when threading is enabled) */
#ifdef CTRANIAN_ENABLE_THREADS
#include <stdatomic.h>

#define CTRANIAN_ATOMIC_LOAD8(mem, addr, order) \
    (ctransian_memory_check_bounds(mem, addr, 1) ? atomic_load_explicit((_Atomic uint8_t*)((uint8_t*)(mem)->data + addr), order) : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), 0))

#define CTRANIAN_ATOMIC_LOAD32(mem, addr, order) \
    (ctransian_memory_check_bounds(mem, addr, 4) ? atomic_load_explicit((_Atomic uint32_t*)((uint8_t*)(mem)->data + addr), order) : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), 0))

#define CTRANIAN_ATOMIC_STORE32(mem, addr, val, order) \
    do { if (ctransian_memory_check_bounds(mem, addr, 4)) atomic_store_explicit((_Atomic uint32_t*)((uint8_t*)(mem)->data + addr), val, order); else ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS); } while(0)

#define CTRANIAN_ATOMIC_CAS32(mem, addr, expected, desired, order) \
    (ctransian_memory_check_bounds(mem, addr, 4) ? atomic_compare_exchange_weak_explicit((_Atomic uint32_t*)((uint8_t*)(mem)->data + addr), expected, desired, order, order) : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), false))
#endif

/* SIMD support macros (when enabled) */
#ifdef CTRANIAN_ENABLE_SIMD
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#include <immintrin.h>

/* SIMD type definitions */
typedef __m128i ctransian_v128;
typedef __m128  ctransian_f32x4;
typedef __m128d ctransian_f64x2;
#else
/* ARM/Other architectures - placeholder types */
typedef struct { uint64_t data[2]; } ctransian_v128;
typedef struct { float data[4]; } ctransian_f32x4;
typedef struct { double data[2]; } ctransian_f64x2;
#endif

/* SIMD utility macros */
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#define CTRANIAN_V128_LOAD(mem, addr) \
    (ctransian_memory_check_bounds(mem, addr, 16) ? _mm_load_si128((__m128i*)((uint8_t*)(mem)->data + addr)) : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), _mm_setzero_si128()))

#define CTRANIAN_V128_STORE(mem, addr, val) \
    do { if (ctransian_memory_check_bounds(mem, addr, 16)) _mm_store_si128((__m128i*)((uint8_t*)(mem)->data + addr), val); else ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS); } while(0)
#else
/* ARM/Other architectures - placeholder implementations */
#define CTRANIAN_V128_LOAD(mem, addr) \
    (ctransian_memory_check_bounds(mem, addr, 16) ? (*(ctransian_v128*)((uint8_t*)(mem)->data + addr)) : (ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS), (ctransian_v128){0,0}))

#define CTRANIAN_V128_STORE(mem, addr, val) \
    do { if (ctransian_memory_check_bounds(mem, addr, 16)) *(ctransian_v128*)((uint8_t*)(mem)->data + addr) = val; else ctransian_trap(CTRANIAN_TRAP_OUT_OF_BOUNDS_MEMORY_ACCESS); } while(0)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* CTRANIAN_RUNTIME_H */
