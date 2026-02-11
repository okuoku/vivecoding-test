#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdint.h>
#include <stdio.h>

typedef struct codegen_context codegen_context_t;

// Initialize codegen context
codegen_context_t* codegen_init(const char* basename);

// Cleanup codegen context
void codegen_cleanup(codegen_context_t* ctx);

// Generate C source and header for a WebAssembly module
int codegen_generate_module(codegen_context_t* ctx, 
                           const char* output_c_file,
                           const char* output_h_file);

#endif // CODEGEN_H