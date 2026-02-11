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

// Forward declarations for expression generation
static int generate_expression(FILE* c_file, const char* basename, BinaryenExpressionRef expr, int indent);

// Generate WebAssembly expression
static int generate_expression(FILE* c_file, const char* basename, BinaryenExpressionRef expr, int indent) {
    if (!expr) return -1;
    
    BinaryenExpressionId id = BinaryenExpressionGetId(expr);
    const char* indent_str = "    ";
    
    // Generate indentation
    for (int i = 0; i < indent; i++) {
        fprintf(c_file, "%s", indent_str);
    }
    
    switch (id) {
        case BinaryenNopId():
            fprintf(c_file, "// nop\n");
            break;
            
        case BinaryenBlockId(): {
            // Get block name and body
            const char* name = BinaryenBlockGetName(expr);
            int num_children = BinaryenExpressionGetNumChildren(expr);
            
            if (name && strlen(name) > 0) {
                fprintf(c_file, "// block %s\n", name);
            } else {
                fprintf(c_file, "// block\n");
            }
            
            for (int i = 0; i < indent; i++) {
                fprintf(c_file, "%s", indent_str);
            }
            fprintf(c_file, "{\n");
            
            // Generate block body
            for (int i = 0; i < num_children; i++) {
                generate_expression(c_file, basename, BinaryenExpressionGetChild(expr, i), indent + 1);
            }
            
            // Close block
            for (int i = 0; i < indent; i++) {
                fprintf(c_file, "%s", indent_str);
            }
            fprintf(c_file, "}\n");
            break;
        }
        
        case BinaryenIfId(): {
            // Get condition, then, and else clauses
            BinaryenExpressionRef condition = BinaryenExpressionGetChild(expr, 0);
            BinaryenExpressionRef if_true = BinaryenExpressionGetChild(expr, 1);
            BinaryenExpressionRef if_false = NULL;
            
            if (BinaryenExpressionGetNumChildren(expr) > 2) {
                if_false = BinaryenExpressionGetChild(expr, 2);
            }
            
            fprintf(c_file, "if (");
            generate_expression(c_file, basename, condition, 0);
            fprintf(c_file, ") {\n");
            
            // Generate then clause
            generate_expression(c_file, basename, if_true, indent + 1);
            
            if (if_false) {
                for (int i = 0; i < indent; i++) {
                    fprintf(c_file, "%s", indent_str);
                }
                fprintf(c_file, "} else {\n");
                
                // Generate else clause
                generate_expression(c_file, basename, if_false, indent + 1);
            }
            
            // Close if
            for (int i = 0; i < indent; i++) {
                fprintf(c_file, "%s", indent_str);
            }
            fprintf(c_file, "}\n");
            break;
        }
        
        case BinaryenLoopId(): {
            // Get loop name and body
            const char* name = BinaryenLoopGetName(expr);
            BinaryenExpressionRef body = BinaryenExpressionGetChild(expr, 0);
            
            if (name && strlen(name) > 0) {
                fprintf(c_file, "// loop %s\n", name);
            } else {
                fprintf(c_file, "// loop\n");
            }
            
            for (int i = 0; i < indent; i++) {
                fprintf(c_file, "%s", indent_str);
            }
            fprintf(c_file, "while (1) {\n");
            
            // Generate loop body
            generate_expression(c_file, basename, body, indent + 1);
            
            // Add implicit break at end of loop if not already present
            for (int i = 0; i < indent; i++) {
                fprintf(c_file, "%s", indent_str);
            }
            fprintf(c_file, "    break;\n");
            
            // Close loop
            for (int i = 0; i < indent; i++) {
                fprintf(c_file, "%s", indent_str);
            }
            fprintf(c_file, "}\n");
            break;
        }
        
        case BinaryenBreakId(): {
            // Get break target
            const char* name = BinaryenBreakGetName(expr);
            
            if (name && strlen(name) > 0) {
                fprintf(c_file, "// br %s\n", name);
                // For simplicity, use break for now (would need label management)
                fprintf(c_file, "break;\n");
            } else {
                fprintf(c_file, "// br\n");
                fprintf(c_file, "break;\n");
            }
            break;
        }
        
        case BinaryenSwitchId(): {
            fprintf(c_file, "// br_table (not fully implemented)\n");
            // For now, just generate a break
            fprintf(c_file, "break;\n");
            break;
        }
        
        case BinaryenReturnId(): {
            fprintf(c_file, "// return\n");
            // Check if return has a value
            if (BinaryenExpressionGetNumChildren(expr) > 0) {
                fprintf(c_file, "return ");
                generate_expression(c_file, basename, BinaryenExpressionGetChild(expr, 0), 0);
                fprintf(c_file, ";\n");
            } else {
                fprintf(c_file, "return;\n");
            }
            break;
        }
        
        case BinaryenUnreachableId(): {
            fprintf(c_file, "// unreachable\n");
            fprintf(c_file, "WACGEN_ABORT();\n");
            break;
        }
        
        case BinaryenLocalGetId(): {
            // Get local index
            int index = BinaryenLocalGetGetIndex(expr);
            fprintf(c_file, "local%d", index);
            break;
        }
        
        case BinaryenLocalSetId(): {
            // Get local index and value
            int index = BinaryenLocalSetGetIndex(expr);
            BinaryenExpressionRef value = BinaryenExpressionGetChild(expr, 0);
            
            fprintf(c_file, "local%d = ", index);
            generate_expression(c_file, basename, value, 0);
            fprintf(c_file, ";\n");
            break;
        }
        
        case BinaryenLocalTeeId(): {
            // Get local index and value
            int index = BinaryenLocalTeeGetIndex(expr);
            BinaryenExpressionRef value = BinaryenExpressionGetChild(expr, 0);
            
            fprintf(c_file, "(local%d = ", index);
            generate_expression(c_file, basename, value, 0);
            fprintf(c_file, ")");
            break;
        }
        
        case BinaryenConstId(): {
            // Get constant value
            BinaryenLiteral literal = BinaryenConstGetValue(expr);
            switch (BinaryenConstGetType(expr)) {
                case BinaryenTypeInt32():
                    fprintf(c_file, "%d", literal.i32);
                    break;
                case BinaryenTypeInt64():
                    fprintf(c_file, "%lld", (long long)literal.i64);
                    break;
                case BinaryenTypeFloat32():
                    fprintf(c_file, "%ff", literal.f32);
                    break;
                case BinaryenTypeFloat64():
                    fprintf(c_file, "%f", literal.f64);
                    break;
                default:
                    fprintf(c_file, "/* unknown const type */");
                    break;
            }
            break;
        }
        
        case BinaryenBinaryId(): {
            // Get binary operation
            BinaryenOp op = BinaryenBinaryGetOp(expr);
            BinaryenExpressionRef left = BinaryenExpressionGetChild(expr, 0);
            BinaryenExpressionRef right = BinaryenExpressionGetChild(expr, 1);
            
            fprintf(c_file, "(");
            generate_expression(c_file, basename, left, 0);
            
            // Map common WebAssembly binary ops to C operators
            switch (op) {
                case BinaryenAddInt32():
                    fprintf(c_file, " + ");
                    break;
                case BinaryenSubInt32():
                    fprintf(c_file, " - ");
                    break;
                case BinaryenMulInt32():
                    fprintf(c_file, " * ");
                    break;
                case BinaryenEqInt32():
                    fprintf(c_file, " == ");
                    break;
                case BinaryenNeInt32():
                    fprintf(c_file, " != ");
                    break;
                case BinaryenLtSInt32():
                case BinaryenLtUInt32():
                    fprintf(c_file, " < ");
                    break;
                case BinaryenLeSInt32():
                case BinaryenLeUInt32():
                    fprintf(c_file, " <= ");
                    break;
                case BinaryenGtSInt32():
                case BinaryenGtUInt32():
                    fprintf(c_file, " > ");
                    break;
                case BinaryenGeSInt32():
                case BinaryenGeUInt32():
                    fprintf(c_file, " >= ");
                    break;
                default:
                    fprintf(c_file, " /* unknown binary op %d */ ", op);
                    break;
            }
            
            generate_expression(c_file, basename, right, 0);
            fprintf(c_file, ")");
            break;
        }
        
        default:
            fprintf(c_file, "/* unsupported expression: %d */\n", id);
            break;
    }
    
    return 0;
}

// Generate function prototype in header (deprecated, now handled inline)
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
    
    // For Phase 4, handle control flow instructions
    // Look for exported functions and generate implementations
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
        
        // Get function info
        BinaryenExpressionRef body = BinaryenFunctionGetBody(func);
        if (!body) continue;
        
        // Get function type info
        BinaryenType result_type = BinaryenFunctionGetResult(func);
        BinaryenType param_types = BinaryenFunctionGetParams(func);
        int num_params = BinaryenTypeGetNumParams(param_types);
        
        printf("Generating implementation for %s\n", export_name);
        
        // Generate function signature in header
        if (result_type == BinaryenTypeInt32()) {
            fprintf(h_file, "int32_t __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else if (result_type == BinaryenTypeInt64()) {
            fprintf(h_file, "int64_t __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else if (result_type == BinaryenTypeFloat32()) {
            fprintf(h_file, "float __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else if (result_type == BinaryenTypeFloat64()) {
            fprintf(h_file, "double __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else {
            fprintf(h_file, "void __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        }
        
        // Add parameters to header
        for (int j = 0; j < num_params; j++) {
            BinaryenType param_type = BinaryenTypeGetParamType(param_types, j);
            if (param_type == BinaryenTypeInt32()) {
                fprintf(h_file, ", int32_t param%d", j);
            } else if (param_type == BinaryenTypeInt64()) {
                fprintf(h_file, ", int64_t param%d", j);
            } else if (param_type == BinaryenTypeFloat32()) {
                fprintf(h_file, ", float param%d", j);
            } else if (param_type == BinaryenTypeFloat64()) {
                fprintf(h_file, ", double param%d", j);
            }
        }
        
        fprintf(h_file, ");\n");
        
        // Generate function implementation in C file
        if (result_type == BinaryenTypeInt32()) {
            fprintf(c_file, "int32_t __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else if (result_type == BinaryenTypeInt64()) {
            fprintf(c_file, "int64_t __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else if (result_type == BinaryenTypeFloat32()) {
            fprintf(c_file, "float __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else if (result_type == BinaryenTypeFloat64()) {
            fprintf(c_file, "double __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        } else {
            fprintf(c_file, "void __%s_%s(wacgenrt_ctx* ctx", module_basename, export_name);
        }
        
        // Add parameters to function
        for (int j = 0; j < num_params; j++) {
            BinaryenType param_type = BinaryenTypeGetParamType(param_types, j);
            if (param_type == BinaryenTypeInt32()) {
                fprintf(c_file, ", int32_t param%d", j);
            } else if (param_type == BinaryenTypeInt64()) {
                fprintf(c_file, ", int64_t param%d", j);
            } else if (param_type == BinaryenTypeFloat32()) {
                fprintf(c_file, ", float param%d", j);
            } else if (param_type == BinaryenTypeFloat64()) {
                fprintf(c_file, ", double param%d", j);
            }
        }
        
        fprintf(c_file, ") {\n");
        
        // Initialize local variables from parameters
        for (int j = 0; j < num_params; j++) {
            if (num_params > 0) {  // Only declare locals if there are parameters
                fprintf(c_file, "    int32_t local%d = param%d;\n", j, j);
            }
        }
        
        // Generate function body
        if (body) {
            if (BinaryenExpressionGetId(body) == BinaryenNopId()) {
                // Handle NOP case
                fprintf(c_file, "    // WebAssembly nop -> empty statement in C\n");
            } else {
                // Generate expression
                generate_expression(c_file, module_basename, body, 1);
            }
        }
        
        // If function has return type and no explicit return in body, add one
        if (result_type != BinaryenTypeNone() && 
            BinaryenExpressionGetId(body) != BinaryenReturnId()) {
            
            fprintf(c_file,    "    // Implicit return\n");
            for (int i = 0; i < num_params; i++) {
                if (i == 0) {
                    fprintf(c_file, "    return ");
                } else {
                    fprintf(c_file, " + ");
                }
                fprintf(c_file, "local%d", i);
            }
            if (num_params == 0) {
                if (result_type == BinaryenTypeInt32()) {
                    fprintf(c_file, "    return 0;\n");
                } else if (result_type == BinaryenTypeInt64()) {
                    fprintf(c_file, "    return 0;\n");
                } else if (result_type == BinaryenTypeFloat32()) {
                    fprintf(c_file, "    return 0.0f;\n");
                } else if (result_type == BinaryenTypeFloat64()) {
                    fprintf(c_file, "    return 0.0;\n");
                }
            } else {
                fprintf(c_file, ";\n");
            }
        }
        
        fprintf(c_file, "}\n\n");
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
