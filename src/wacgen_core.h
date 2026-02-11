#ifndef WACGEN_CORE_H
#define WACGEN_CORE_H

#include <stdint.h>

// Result codes
typedef enum {
    WACGEN_SUCCESS = 0,
    WACGEN_ERROR_INVALID_INPUT = 1,
    WACGEN_ERROR_PARSE_FAILED = 2,
    WACGEN_ERROR_CODEGEN_FAILED = 3,
    WACGEN_ERROR_MEMORY = 4
} wacgen_result_t;

// Main conversion function
wacgen_result_t wacgen_convert(const char* input_file, 
                              const char* basename, 
                              const char* map_file);

#endif // WACGEN_CORE_H