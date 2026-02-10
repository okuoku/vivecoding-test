#ifndef CTRANIAN_PUBLIC_H
#define CTRANIAN_PUBLIC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ctransian/config.h"

/* Forward declarations */
typedef struct ctransian_context ctransian_context_t;
typedef struct ctransian_config ctransian_config_t;
typedef struct ctransian_result ctransian_result_t;

/* Result codes */
typedef enum {
    CTRANIAN_SUCCESS = 0,
    CTRANIAN_ERROR_INVALID_INPUT = 1,
    CTRANIAN_ERROR_INVALID_CONFIG = 2,
    CTRANIAN_ERROR_PARSE_FAILED = 3,
    CTRANIAN_ERROR_TRANSLATION_FAILED = 4,
    CTRANIAN_ERROR_MEMORY_ALLOCATION = 5,
    CTRANIAN_ERROR_UNSUPPORTED_FEATURE = 6,
    CTRANIAN_ERROR_VALIDATION_FAILED = 7,
    CTRANIAN_ERROR_IO_ERROR = 8
} ctransian_error_t;

/* Optimization levels */
typedef enum {
    CTRANIAN_OPTIMIZE_NONE = 0,
    CTRANIAN_OPTIMIZE_BASIC = 1,
    CTRANIAN_OPTIMIZE_DEFAULT = 2,
    CTRANIAN_OPTIMIZE_AGGRESSIVE = 3
} ctransian_optimize_level_t;

/* Bounds checking modes */
typedef enum {
    CTRANIAN_BOUNDS_NONE = 0,
    CTRANIAN_BOUNDS_RELAXED = 1,
    CTRANIAN_BOUNDS_STRICT = 2
} ctransian_bounds_mode_t;

/* Target C standards */
typedef enum {
    CTRANIAN_C99 = 0,
    CTRANIAN_C11 = 1,
    CTRANIAN_GNU11 = 2
} ctransian_c_standard_t;

/* Runtime types */
typedef enum {
    CTRANIAN_RUNTIME_EMBEDDED = 0,
    CTRANIAN_RUNTIME_STANDALONE = 1
} ctransian_runtime_type_t;

/* Configuration structure */
struct ctransian_config {
    /* Optimization settings */
    ctransian_optimize_level_t optimization_level;
    
    /* Feature enablement */
    bool enable_simd;
    bool enable_threads;
    bool enable_gc;
    bool enable_wasi;
    
    /* Code generation settings */
    ctransian_bounds_mode_t bounds_checking;
    ctransian_c_standard_t target_c_standard;
    ctransian_runtime_type_t runtime_type;
    
    /* Debug and validation */
    bool generate_debug_info;
    bool validate_input;
    
    /* Output settings */
    bool generate_header;
    bool generate_comments;
    
    /* Reserved for future use */
    uint32_t reserved[8];
};

/* Translation result */
struct ctransian_result {
    ctransian_error_t error_code;
    char* error_message;
    
    /* Generated output */
    char* source_code;
    char* header_code;
    size_t source_size;
    size_t header_size;
    
    /* Statistics */
    uint32_t functions_translated;
    uint32_t instructions_translated;
    uint32_t optimization_passes_run;
    double translation_time_ms;
    
    /* Reserved for future use */
    void* reserved[8];
};

/* Callback types for streaming translation */
typedef size_t (*ctransian_read_callback_t)(void* user_data, uint8_t* buffer, size_t size);
typedef int (*ctransian_write_callback_t)(void* user_data, const char* data, size_t size);

/* Context management */
ctransian_context_t* ctransian_create(const ctransian_config_t* config);
void ctransian_destroy_context(ctransian_context_t* ctx);

/* Configuration helpers */
ctransian_config_t* ctransian_config_create(void);
void ctransian_config_destroy(ctransian_config_t* config);
void ctransian_config_set_defaults(ctransian_config_t* config);

/* Translation functions */
ctransian_error_t ctransian_translate_binary(
    ctransian_context_t* ctx,
    const uint8_t* wasm_data,
    size_t wasm_size,
    ctransian_result_t* result
);

ctransian_error_t ctransian_translate_text(
    ctransian_context_t* ctx,
    const char* wat_text,
    ctransian_result_t* result
);

ctransian_error_t ctransian_translate_stream(
    ctransian_context_t* ctx,
    ctransian_read_callback_t read_cb,
    ctransian_write_callback_t write_cb,
    void* user_data
);

/* Result management */
void ctransian_result_init(ctransian_result_t* result);
void ctransian_result_cleanup(ctransian_result_t* result);
const char* ctransian_error_string(ctransian_error_t error);

/* Version information */
const char* ctransian_version(void);
uint32_t ctransian_version_number(void);
bool ctransian_has_feature(const char* feature_name);

/* Validation utilities */
ctransian_error_t ctransian_validate_binary(
    const uint8_t* wasm_data,
    size_t wasm_size
);

ctransian_error_t ctransian_validate_text(const char* wat_text);

/* Statistics and introspection */
typedef struct {
    uint32_t num_functions;
    uint32_t num_imports;
    uint32_t num_exports;
    uint32_t num_memory_segments;
    uint32_t num_tables;
    uint32_t num_globals;
    uint32_t num_types;
    size_t code_size_estimate;
    bool has_simd;
    bool has_threads;
    bool has_gc;
    bool has_wasi_imports;
} ctransian_module_info_t;

ctransian_error_t ctransian_analyze_binary(
    const uint8_t* wasm_data,
    size_t wasm_size,
    ctransian_module_info_t* info
);

ctransian_error_t ctransian_analyze_text(
    const char* wat_text,
    ctransian_module_info_t* info
);

#ifdef __cplusplus
}
#endif

#endif /* CTRANIAN_PUBLIC_H */