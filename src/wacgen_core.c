#include "wacgen_core.h"
#include "codegen/codegen.h"
#include <binaryen-c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generate basename from input file path if not provided
static char* extract_basename(const char* input_file, const char* provided_basename) {
    if (provided_basename) {
        return strdup(provided_basename);
    }
    
    // Extract filename without extension
    const char* filename = strrchr(input_file, '/');
    if (!filename) filename = input_file;
    else filename++;
    
    char* basename = strdup(filename);
    char* dot = strrchr(basename, '.');
    if (dot) *dot = '\0';
    
    return basename;
}

// Generate C filename from basename
static char* generate_c_filename(const char* basename) {
    char* filename = malloc(strlen(basename) + 3);
    sprintf(filename, "%s.c", basename);
    return filename;
}

// Generate H filename from basename  
static char* generate_h_filename(const char* basename) {
    char* filename = malloc(strlen(basename) + 3);
    sprintf(filename, "%s.h", basename);
    return filename;
}

// Generate C function for WebAssembly NOP instruction
static int generate_nop_function(FILE* c_file, const char* basename, const char* func_name) {
    fprintf(c_file, "void __%s_%s(wacgenrt_ctx* ctx) {\n", basename, func_name);
    fprintf(c_file, "    // WebAssembly nop -> empty statement in C\n");
    fprintf(c_file, "}\n\n");
    return 0;
}

// Generate function prototype in header
static int generate_function_prototype(FILE* h_file, const char* basename, const char* func_name) {
    fprintf(h_file, "void __%s_%s(wacgenrt_ctx* ctx);\n", basename, func_name);
    return 0;
}

wacgen_result_t wacgen_convert(const char* input_file, 
                              const char* basename, 
                              const char* map_file) {
    if (!input_file) {
        return WACGEN_ERROR_INVALID_INPUT;
    }
    
    char* module_basename = extract_basename(input_file, basename);
    if (!module_basename) {
        return WACGEN_ERROR_MEMORY;
    }
    
    char* output_c_file = generate_c_filename(module_basename);
    char* output_h_file = generate_h_filename(module_basename);
    
    if (!output_c_file || !output_h_file) {
        free(module_basename);
        if (output_c_file) free(output_c_file);
        if (output_h_file) free(output_h_file);
        return WACGEN_ERROR_MEMORY;
    }
    
    printf("Converting %s to C...\n", input_file);
    printf("Basename: %s\n", module_basename);
    
    if (map_file) {
        printf("Using map file: %s\n", map_file);
    }
    
    wacgen_result_t result = WACGEN_SUCCESS;
    
    // Read WebAssembly file into memory
    FILE* file = fopen(input_file, "rb");
    if (!file) {
        fprintf(stderr, "Error: Failed to open input file: %s\n", input_file);
        result = WACGEN_ERROR_PARSE_FAILED;
        goto cleanup;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = malloc(file_size);
    if (!buffer) {
        fprintf(stderr, "Error: Failed to allocate memory for file\n");
        fclose(file);
        result = WACGEN_ERROR_MEMORY;
        goto cleanup;
    }
    
    size_t bytes_read = fread(buffer, 1, file_size, file);
    fclose(file);
    
    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error: Failed to read entire file\n");
        free(buffer);
        result = WACGEN_ERROR_PARSE_FAILED;
        goto cleanup;
    }
    
    BinaryenModuleRef module = BinaryenModuleRead(buffer, bytes_read);
    free(buffer);
    
    if (!module) {
        fprintf(stderr, "Error: Failed to read WebAssembly module\n");
        result = WACGEN_ERROR_PARSE_FAILED;
        goto cleanup;
    }
    
    // Initialize codegen
    codegen_context_t* cg_ctx = codegen_init(module_basename);
    if (!cg_ctx) {
        fprintf(stderr, "Error: Failed to initialize codegen\n");
        BinaryenModuleDispose(module);
        result = WACGEN_ERROR_MEMORY;
        goto cleanup;
    }
    
    // Open output files
    FILE* c_file = fopen(output_c_file, "w");
    FILE* h_file = fopen(output_h_file, "w");
    
    if (!c_file || !h_file) {
        fprintf(stderr, "Error: Failed to open output files\n");
        if (c_file) fclose(c_file);
        if (h_file) fclose(h_file);
        codegen_cleanup(cg_ctx);
        BinaryenModuleDispose(module);
        result = WACGEN_ERROR_CODEGEN_FAILED;
        goto cleanup;
    }
    
    // Generate header guard
    fprintf(h_file, "#ifndef __%s_H\n", module_basename);
    fprintf(h_file, "#define __%s_H\n\n", module_basename);
    fprintf(h_file, "#include \"wacgen_rt.h\"\n\n");
    
    // Generate C file includes
    fprintf(c_file, "#include \"%s.h\"\n\n", module_basename);
    
    // For Phase 3, we only handle NOP functions
    // Look for exported functions and generate NOP implementation
    int num_exports = BinaryenGetNumExports(module);
    
    for (int i = 0; i < num_exports; i++) {
        BinaryenExportRef export = BinaryenGetExportByIndex(module, i);
        if (!export) continue;
        
        const char* export_name = BinaryenExportGetName(export);
        if (!export_name) continue;
        
        // Get the function name for this export
        const char* func_name = BinaryenExportGetValue(export);
        if (!func_name) continue;
        
        // Get the function by its internal name
        BinaryenFunctionRef func = BinaryenGetFunction(module, func_name);
        if (!func) continue;
        
        // Get function body to check if it's just NOP
        BinaryenExpressionRef body = BinaryenFunctionGetBody(func);
        if (body && BinaryenExpressionGetId(body) == BinaryenNopId()) {
            printf("Generating NOP implementation for %s\n", export_name);
            generate_nop_function(c_file, module_basename, export_name);
            generate_function_prototype(h_file, module_basename, export_name);
        }
    }
    
    // Close header guard
    fprintf(h_file, "\n#endif // __%s_H\n", module_basename);
    
    // Cleanup
    fclose(c_file);
    fclose(h_file);
    codegen_cleanup(cg_ctx);
    BinaryenModuleDispose(module);
    
    printf("Generated: %s, %s\n", output_c_file, output_h_file);

cleanup:
    free(module_basename);
    free(output_c_file);
    free(output_h_file);
    
    return result;
}
