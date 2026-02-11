#ifndef WACGEN_CORE_H
#define WACGEN_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Include Binaryen types
#include "binaryen-c.h"

// Wacgen context for code generation
typedef struct {
    BinaryenModuleRef module;
    const char* basename;
    FILE* header_file;
    FILE* source_file;
} wacgen_ctx;

// Module loading functions
BinaryenModuleRef wacgen_load_module(const char* wasm_file);
void wacgen_dispose_module(BinaryenModuleRef module);

// Code generation functions
bool wacgen_generate_c(wacgen_ctx* ctx, const char* basename, const char* wasm_file);
bool wacgen_generate_function(wacgen_ctx* ctx, const char* func_name);

// Utility functions
bool wacgen_is_nop_function(BinaryenModuleRef module, const char* func_name);

#ifdef __cplusplus
}
#endif

#endif // WACGEN_CORE_H