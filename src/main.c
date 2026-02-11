#include "wacgen_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: wacgen [-b basename] input.wasm\n");
        fprintf(stderr, "  -b basename    Specify basename for generated files\n");
        fprintf(stderr, "  input.wasm     WebAssembly binary file to convert\n");
        return 1;
    }
    
    const char* wasm_file = NULL;
    const char* basename = NULL;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: -b option requires a basename argument\n");
                return 1;
            }
            basename = argv[++i];
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option %s\n", argv[i]);
            return 1;
        } else {
            if (wasm_file) {
                fprintf(stderr, "Error: Multiple input files not supported\n");
                return 1;
            }
            wasm_file = argv[i];
        }
    }
    
    if (!wasm_file) {
        fprintf(stderr, "Error: No input WebAssembly file specified\n");
        return 1;
    }
    
    // If no basename provided, derive from filename
    char default_basename[256];
    if (!basename) {
        const char* filename = strrchr(wasm_file, '/');
        if (!filename) filename = wasm_file;
        else filename++;
        
        // Remove extension
        strncpy(default_basename, filename, sizeof(default_basename) - 1);
        default_basename[sizeof(default_basename) - 1] = '\0';
        
        char* dot = strrchr(default_basename, '.');
        if (dot) *dot = '\0';
        
        basename = default_basename;
    }
    
    // Initialize wacgen context
    wacgen_ctx ctx = {0};
    
    // Generate C code
    if (!wacgen_generate_c(&ctx, basename, wasm_file)) {
        fprintf(stderr, "Error: Failed to generate C code\n");
        return 1;
    }
    
    printf("Successfully generated C code from %s\n", wasm_file);
    return 0;
}