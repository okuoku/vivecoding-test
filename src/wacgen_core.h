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
    int block_counter;
    int block_depth[16];  // Track block nesting up to 16 levels
    int current_depth;
} wacgen_ctx;

// Module loading functions
BinaryenModuleRef wacgen_load_module(const char* wasm_file);
void wacgen_dispose_module(BinaryenModuleRef module);

// Code generation functions
bool wacgen_generate_c(wacgen_ctx* ctx, const char* basename, const char* wasm_file);
bool wacgen_generate_function(wacgen_ctx* ctx, const char* func_name);

// Expression translation functions
bool wacgen_translate_expression(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_if(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_return(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_const(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_local_get(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_local_set(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_binary(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_block(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_br(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_br_if(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);
bool wacgen_translate_loop(wacgen_ctx* ctx, BinaryenExpressionRef expr, int indent_level);

// Utility functions
bool wacgen_is_nop_function(BinaryenModuleRef module, const char* func_name);
void wacgen_write_indent(FILE* file, int level);
const char* wacgen_get_type_string(BinaryenType type);
bool wacgen_generate_function_params(FILE* file, BinaryenFunctionRef func);
bool wacgen_generate_local_vars(FILE* file, BinaryenFunctionRef func);

#ifdef __cplusplus
}
#endif

#endif // WACGEN_CORE_H