#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wacgen_core.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-b basename] [-m map.txt] input.wasm\n", argv[0]);
        return 1;
    }
    
    const char* basename = NULL;
    const char* map_file = NULL;
    const char* input_file = NULL;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            basename = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            map_file = argv[++i];
        } else if (argv[i][0] != '-') {
            input_file = argv[i];
        }
    }
    
    if (!input_file) {
        fprintf(stderr, "Error: Input file required\n");
        return 1;
    }
    
    return wacgen_convert(input_file, basename, map_file);
}